/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   article_search.cpp
 * Author: centos7
 * 
 * Created on December 6, 2016, 10:25 AM
 */

#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/URI.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/NumberParser.h>
#include <Poco/NumberFormatter.h>


#include "article_search.hpp"

namespace webcppd {
    namespace blog {

        void article_search::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {

            Poco::Net::HTMLForm form(request);
            if (!form.has("key") || !form.has("page")) {
                this->error(request, response);
                return;
            }
            std::string key = form.get("key");
            int page = Poco::NumberParser::parse(form.get("page"));
            if (page < 0) {
                this->error(request, response);
                return;
            }

            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html;charset=utf-8");
            std::string cacheKey(this->create_cache_key(request, response));
            if (root_view::root_cache().has(cacheKey)) {
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }
            Poco::SharedPtr<Kainjow::Mustache::Data> data = this->tpl_ready("/blog/config.json", "article_search.GET");

            data->set("key", key);
            data->set("page", Poco::NumberFormatter::format(page));


            Poco::Data::MySQL::Connector::registerConnector();
            Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
            Poco::Data::Statement select(session);
            int low_limit = (page - 1)*5;
            select << "SELECT  A.`id`  as `pid`,B.`id` as `uid`,B.`name` as `uname`, A.`head`, A.`abstract`, A.`created`,A.`visitor`,B.`image` as `uimage` "
                    "FROM  `article` A inner join `article_user` B on A.`user`=B.`id` where  A.`permission` = TRUE AND A.`publish` = TRUE and match(`head` , `keywords` , `abstract` , `body`) AGAINST (?) limit ?,5;",
                    Poco::Data::Keywords::use(key),
                    Poco::Data::Keywords::use(low_limit);
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

            Poco::Data::MySQL::Connector::unregisterConnector();


            root_view::root_cache().add(cacheKey, this->render_tpl(data->get("maintpl")->stringValue(), *data));
            response.send() << *root_view::root_cache().get(cacheKey);
        }



    }
}
