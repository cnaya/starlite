/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   article_detail.cpp
 * Author: centos7
 * 
 * Created on November 30, 2016, 4:51 PM
 */

#include <sstream>
#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/NumberParser.h>
#include <Poco/StringTokenizer.h>
#include <Poco/URI.h>
#include <Poco/JSON/Object.h>



#include "article_detail.hpp"


namespace webcppd {
    namespace blog {

        void article_detail::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            std::string uri_path = Poco::URI(request.getURI()).getPath();
            Poco::StringTokenizer st(uri_path, "/", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
            int pid = Poco::NumberParser::parse(st[2]);

            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html;charset=utf-8");
            std::string cacheKey(this->create_cache_key(request, response));

            if (root_view::root_cache().has(cacheKey)) {
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }
            Poco::SharedPtr<Kainjow::Mustache::Data> data = this->tpl_ready("/blog/config.json", "article_detail.GET");



            Poco::Data::MySQL::Connector::registerConnector();
            Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
            Poco::Data::Statement select(session);

            select << "SELECT  a.`id`,a.`head`,a.`abstract`,a.`keywords`,a.`visitor`, a.`created`,b.`name` AS `uname`, b.`id` AS `uid` ,b.`image` AS `uimage`"
                    "FROM `article` a "
                    "INNER JOIN `article_user` b ON a.`user` = b.`id` "
                    "WHERE a.`id` = ? AND a.`permission` = TRUE AND a.`publish` = TRUE "
                    "LIMIT 1;", Poco::Data::Keywords::use(pid);
            if (select.execute()) {
                Poco::Data::RecordSet rs(select);
                Poco::Data::Row& row = rs.row(0);
                data->set("pid", row["id"].toString());
                data->set("title", row["head"].toString());
                data->set("abstract", row["abstract"].toString());
                data->set("keywords", row["keywords"].toString());
                data->set("author_id", row["uid"].toString());
                data->set("author_name", row["uname"].toString());
                data->set("author_image", row["uimage"].toString());
                data->set("post_url", (this->app.config().getBool("http.enableSSL", true) ? "https://" : "http://") + request.getHost() + request.getURI());
                Poco::DateTime dt;
                int tzd;
                Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, row["created"].toString(), dt, tzd);
                dt.makeLocal(tzd);
                data->set("created", Poco::DateTimeFormatter::format(dt, Poco::DateTimeFormat::HTTP_FORMAT));
                Poco::StringTokenizer tag_st(row["keywords"].toString(), ",", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
                Kainjow::Mustache::Data tag_list = Kainjow::Mustache::Data::List();
                for (auto &item : tag_st) {
                    Kainjow::Mustache::Data tag;
                    tag.set("tag", item);
                    tag_list.push_back(tag);
                }
                data->set("tag_list", tag_list);
                data->set("visitor", row["visitor"].toString());


                Poco::Data::MySQL::Connector::unregisterConnector();
            } else {
                (*data)["title"] = "消息";
                (*data)["message"] = "没有找到。";
                (*data)["maintpl"] = "/blog/message.html";
                response.send() << this->render_tpl(data->get("maintpl")->stringValue(), *data);
                return;
            }


            root_view::root_cache().add(cacheKey, this->render_tpl(data->get("maintpl")->stringValue(), *data));
            response.send() << *root_view::root_cache().get(cacheKey);
        }

        void article_detail::do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            response.setContentType("application/json;charset=utf-8");

            std::string uri_path = Poco::URI(request.getURI()).getPath();
            Poco::StringTokenizer st(uri_path, "/", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
            int pid = Poco::NumberParser::parse(st[2]);

            Poco::Data::MySQL::Connector::registerConnector();
            Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
            Poco::Data::Statement select(session);

            std::string cacheKey(this->create_cache_key(request,response));

            if (root_view::root_cache().has(cacheKey)) {
                select << "update `article` set `visitor`=`visitor`+1 where `id`=?; ",
                        Poco::Data::Keywords::use(pid),
                        Poco::Data::Keywords::now;
                Poco::Data::MySQL::Connector::unregisterConnector();
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }


            Poco::JSON::Object result;


            std::string body, head, abstract, keywords;
            bool publish;
            select << "select `body`,`head`,`abstract`,`keywords`,`publish` from `article` where `id`=? limit 1;",
                    Poco::Data::Keywords::use(pid),
                    Poco::Data::Keywords::into(body),
                    Poco::Data::Keywords::into(head),
                    Poco::Data::Keywords::into(abstract),
                    Poco::Data::Keywords::into(keywords),
                    Poco::Data::Keywords::into(publish),
                    Poco::Data::Keywords::now;

            if (body.empty()) {
                result.set("ok", false);
            } else {
                result.set("ok", true);

                result.set("body", body);
                result.set("head", head);
                result.set("abstract", abstract);
                result.set("keywords", keywords);
                result.set("publish", publish);
                select.reset(session);
                select << "update `article` set `visitor`=`visitor`+1 where `id`=?; ",
                        Poco::Data::Keywords::use(pid),
                        Poco::Data::Keywords::now;
            }
            Poco::Data::MySQL::Connector::unregisterConnector();
            std::ostringstream os;
            result.stringify(os);
            root_view::root_cache().add(cacheKey, os.str());
            response.send() << *root_view::root_cache().get(cacheKey);
        }


    }
}