/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   user_edit.cpp
 * Author: centos7
 * 
 * Created on December 7, 2016, 6:36 PM
 */


#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/NumberFormatter.h>
#include <Poco/NumberParser.h>
#include <Poco/URI.h>

#include "user_edit.hpp"


namespace webcppd {
    namespace blog {

        void user_edit::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            if (!this->session_has(request, response, "user.id")) {
                response.redirect("/blog/login");
                return;
            }
            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html;charset=utf-8");

            std::string cacheKey(this->create_cache_key(request, response));

            if (root_view::root_cache().has(cacheKey)) {
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }
            std::string uri_path = Poco::URI(request.getURI()).getPath();

            Poco::StringTokenizer st(uri_path, "/", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
            unsigned long uid = Poco::NumberParser::parse(st[2]);
            if (uid != this->session_get(request, response, "user.id").convert<unsigned long>()) {
                Poco::SharedPtr<Kainjow::Mustache::Data> data = this->tpl_ready("/blog/config.json", "error.GET");
                (*data)["message"] = "你无权编辑。";
                response.send() << this->render_tpl(data->get("maintpl")->stringValue(), *data);
                return;
            }

            Poco::SharedPtr<Kainjow::Mustache::Data> data = this->tpl_ready("/blog/config.json", "user_edit.GET");


            Poco::Data::MySQL::Connector::registerConnector();
            Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
            Poco::Data::Statement select(session);

            std::string name, about, image;
            select << "select `name`,`about`,`image` from `article_user` where `id`=?  limit 1;",
                    Poco::Data::Keywords::into(name),
                    Poco::Data::Keywords::into(about),
                    Poco::Data::Keywords::into(image),
                    Poco::Data::Keywords::use(uid),
                    Poco::Data::Keywords::now;
            Poco::Data::MySQL::Connector::unregisterConnector();
            data->set("name", name);
            data->set("about", about);
            data->set("image", image);

            root_view::root_cache().add(cacheKey, this->render_tpl(data->get("maintpl")->stringValue(), *data));
            response.send() << *root_view::root_cache().get(cacheKey);
        }

        void user_edit::do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            response.setContentType("application/json;charset=utf-8");
            Poco::JSON::Object result;
            if (!this->session_has(request, response, "user.id")) {
                result.set("ok", false);
                result.set("message", "尚未登陆。");
                result.stringify(response.send());
                return;
            }
            Poco::Net::HTMLForm form(request, request.stream());
            unsigned long user_id = this->session_get(request, response, "user.id").convert<unsigned long>();
            Poco::Data::MySQL::Connector::registerConnector();
            Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
            Poco::Data::Statement update(session);
            if (form.has("oldPassword") && form.has("newPassword")) {
                std::string oldPassword = form.get("oldPassword"), newPassword = form.get("newPassword");
                update << "update `article_user`  set `password`=md5(?) where `password`=md5(?) and  `id`=? ;",
                        Poco::Data::Keywords::use(newPassword),
                        Poco::Data::Keywords::use(oldPassword),
                        Poco::Data::Keywords::use(user_id);
                if (update.execute()) {
                    result.set("ok", true);
                    result.set("message", "更新成功。");
                } else {
                    result.set("ok", false);
                    result.set("message", "更新失败。");
                }
            } else if (form.has("name") && form.has("about")) {
                std::string name = form.get("name"), about = form.get("about");

                update << "update `article_user` set `name`=?, `about`=? where `id`=? ;",
                        Poco::Data::Keywords::use(name),
                        Poco::Data::Keywords::use(about),
                        Poco::Data::Keywords::use(user_id);
                if (update.execute()) {
                    result.set("ok", true);
                    result.set("message", "更新成功。");
                } else {
                    result.set("ok", false);
                    result.set("message", "更新失败。");
                }

            } else {
                result.set("ok", false);
                result.set("message", "更新失败。");
            }
            Poco::Data::MySQL::Connector::unregisterConnector();
            result.stringify(response.send());
        }
    }
}
