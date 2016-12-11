/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   login.cpp
 * Author: centos7
 * 
 * Created on November 14, 2016, 10:36 AM
 */

#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/URI.h>
#include <map>


#include "login.hpp"

namespace webcppd {
    namespace blog {

        void login::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            if (this->session_has(request, response, "user.id")) {
                response.redirect("/user/detail/" + this->session_get(request, response, "user.id").convert<std::string>());
                return;
            }
            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html;charset=utf-8");

            std::string cacheKey(this->create_cache_key(request, response));
            if (root_view::root_cache().has(cacheKey)) {
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }

            Kainjow::Mustache::Data data;
            data.set("head", this->render_tpl("/blog/head.html"));
            data.set("nav", this->render_tpl("/blog/nav.html"));
            data.set("footer", this->render_tpl("/blog/footer.html"));
            data.set("script", this->render_tpl("/blog/script.html"));

            std::string tpl_path("/blog/article.user.login.html");
            root_view::root_cache().add(cacheKey, this->render_tpl(tpl_path, data));
            response.send() << *root_view::root_cache().get(cacheKey);
        }

        void login::do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            response.setContentType("application/json;charset=utf-8");
            Poco::JSON::Object result;
            if (this->session_has(request, response, "user.id")) {
                result.set("ok", false);
                result.set("message", "已经登陆。");
                result.set("login", true);
                result.stringify(response.send());
                return;
            }


            Poco::Net::HTMLForm form(request, request.stream());


            if (form.has("inputEmail") && form.has("inputPassword")) {

                Poco::Data::MySQL::Connector::registerConnector();
                Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
                Poco::Data::Statement select(session);
                std::string inputEmail = form.get("inputEmail"), inputPassword = form.get("inputPassword");
                if (session.isConnected()) {
                    unsigned long id = 0;
                    select << "SELECT `id` FROM `article_user` where `permission`=TRUE and `email`=? and `password`=md5(?) limit 1;"
                            , Poco::Data::Keywords::into(id)
                            , Poco::Data::Keywords::use(inputEmail)
                            , Poco::Data::Keywords::use(inputPassword)
                            , Poco::Data::Keywords::now;
                    if (id) {
                        result.set("ok", true);
                        result.set("message", "登陆成功。");
                        this->session_set(request, response, "user.id", id);
                    } else {
                        result.set("ok", false);
                        result.set("message", "登陆失败。");
                    }
                }
                Poco::Data::MySQL::Connector::unregisterConnector();
            } else {
                result.set("ok", false);
                result.set("message", "登陆失败。");
            }

            result.stringify(response.send());
        }

    }
}

