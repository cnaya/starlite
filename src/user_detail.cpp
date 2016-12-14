/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   user_detail.cpp
 * Author: centos7
 * 
 * Created on December 7, 2016, 5:22 PM
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



#include "user_detail.hpp"


namespace webcppd {
    namespace blog {

        void user_detail::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            std::string uri_path = Poco::URI(request.getURI()).getPath();
            Poco::StringTokenizer st(uri_path, "/", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
            int uid = Poco::NumberParser::parse(st[2]);

            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html;charset=utf-8");
            std::string cacheKey(this->create_cache_key(request, response));

            if (root_view::root_cache().has(cacheKey)) {
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }

            Poco::SharedPtr<Kainjow::Mustache::Data> data = this->tpl_ready("/blog/config.json", "user_detail.GET");


            Poco::Data::MySQL::Connector::registerConnector();
            Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
            Poco::Data::Statement select(session);

            select << "select `id`, `name`,`about`,`image`,`created`,(select count(*) from `article` where `user`=?) as len from `article_user` where `id`=? limit 1;"
                    , Poco::Data::Keywords::use(uid)
                    , Poco::Data::Keywords::use(uid);
            if (select.execute()) {
                Poco::Data::RecordSet rs(select);
                Poco::Data::Row& row = rs.row(0);
                data->set("user_id", row["id"].toString());
                data->set("user_name", row["name"].toString());
                data->set("user_about", row["about"].toString());
                data->set("user_image", row["image"].toString());
                data->set("user_article_len", row["len"].toString());
                Poco::DateTime dt;
                int tzd;
                Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, row["created"].toString(), dt, tzd);
                dt.makeLocal(tzd);
                data->set("user_created", Poco::DateTimeFormatter::format(dt, Poco::DateTimeFormat::HTTP_FORMAT));


                Poco::Data::MySQL::Connector::unregisterConnector();
            } else {
                data->set("title", "消息");
                data->set("message", "没有找到。");
                response.send() << this->render_tpl("/blog/message.html", *data);
                return;
            }


            root_view::root_cache().add(cacheKey, this->render_tpl(data->get("maintpl")->stringValue(), *data));
            response.send() << *root_view::root_cache().get(cacheKey);
        }

    }
}
