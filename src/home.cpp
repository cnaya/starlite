/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   home.cpp
 * Author: centos7
 * 
 * Created on November 14, 2016, 9:22 AM
 */

#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/URI.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/JSON/JSON.h>
#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Array.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Query.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/JSON/Template.h>
#include <fstream>



#include "home.hpp"



namespace webcppd {
    namespace blog {

        void home::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html;charset=utf-8");
            std::string cacheKey(this->create_cache_key(request, response));

            if (root_view::root_cache().has(cacheKey)) {
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }


            Poco::SharedPtr<Kainjow::Mustache::Data> data = this->tpl_ready("/blog/config.json", "home.GET");


            Poco::Data::MySQL::Connector::registerConnector();
            Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
            Poco::Data::Statement select(session);

            select << "SELECT `pid`, `uid`, `uname`,`head`, `abstract`, `created`,`visitor`,`uimage` "
                    "FROM  `get_lastest_articles` limit 5";
            select.execute();
            Poco::Data::RecordSet rs(select);

            Kainjow::Mustache::Data post_list = Kainjow::Mustache::Data::List();
            for (auto & row : rs) {
                Kainjow::Mustache::Data post;
                post.set("post_id", row["pid"].toString());
                post.set("post_title", row["head"].toString());
                post.set("post_abstract", row["abstract"].toString());
                post.set("post_author_id", row["uid"].toString());
                post.set("post_visitor", row["visitor"].toString());
                post.set("post_author_name", row["uname"].toString());
                Poco::DateTime dt;
                int tzd;
                Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, row["created"].toString(), dt, tzd);
                dt.makeLocal(tzd);
                post.set("post_created", Poco::DateTimeFormatter::format(dt, Poco::DateTimeFormat::HTTP_FORMAT));

                post.set("user_image", row["uimage"].toString());
                post_list.push_back(post);
            }
            data->set("post_list", post_list);
            if (this->session_has(request, response, "user.id")) {
                data->set("user_id", this->session_get(request, response, "user.id").convert<std::string>());
            }

            Poco::Data::MySQL::Connector::unregisterConnector();

            
            root_view::root_cache().add(cacheKey, this->render_tpl(data->get("maintpl")->stringValue(), *data));
            response.send() << *root_view::root_cache().get(cacheKey);
        }

        void home::do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            Poco::Net::HTMLForm form(request, request.stream());
            if (!form.has("page")) {
                this->error(request, response);
                return;
            }
            int page = Poco::NumberParser::parse(form.get("page"));
            if (page < 0) {
                this->error(request, response);
                return;
            }
            response.setContentType("application/json;charset=utf-8");
            std::string cacheKey(this->create_cache_key(request, response, form.get("page")));
            if (root_view::root_cache().has(cacheKey)) {
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }


            Poco::Data::MySQL::Connector::registerConnector();
            Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
            Poco::Data::Statement select(session);
            int low_limit = (page - 1)*5;
            select << "SELECT `pid`, `uid`, `uname`,`head`, `abstract`, `created`,`visitor`,`uimage` "
                    "FROM  `get_lastest_articles` limit ?,5",
                    Poco::Data::Keywords::use(low_limit);
            select.execute();
            Poco::Data::RecordSet rs(select);
            Poco::JSON::Object result;

            Poco::JSON::Array post_list;
            for (auto & row : rs) {
                Poco::JSON::Object post;
                post.set("post_id", row["pid"].toString());
                post.set("post_title", row["head"].toString());
                post.set("post_abstract", row["abstract"].toString());
                post.set("post_author_id", row["uid"].toString());
                post.set("post_visitor", row["visitor"].toString());
                post.set("post_author_name", row["uname"].toString());
                Poco::DateTime dt;
                int tzd;
                Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, row["created"].toString(), dt, tzd);
                dt.makeLocal(tzd);
                post.set("post_created", Poco::DateTimeFormatter::format(dt, Poco::DateTimeFormat::HTTP_FORMAT));

                post.set("user_image", row["uimage"].toString());
                post_list.add(post);
            }
            result.set("post_list", post_list);
            if (!post_list.size()) {
                result.set("more", false);
            } else {
                result.set("more", true);
            }

            Poco::Data::MySQL::Connector::unregisterConnector();

            std::ostringstream os;
            result.stringify(os);
            root_view::root_cache().add(cacheKey, os.str());
            response.send() << *root_view::root_cache().get(cacheKey);
        }
    }
}

