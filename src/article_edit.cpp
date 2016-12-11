/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   article_edit.cpp
 * Author: centos7
 * 
 * Created on November 28, 2016, 5:00 PM
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

#include "article_edit.hpp"

namespace webcppd {
    namespace blog {

        void article_edit::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
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
            unsigned long pid = st.count() < 3 ? 0 : Poco::NumberParser::parse(st[2]);
            unsigned long user_id = this->session_get(request, response, "user.id").convert<unsigned long>();


            Kainjow::Mustache::Data data;
            data.set("head", this->render_tpl("/blog/head.html"));
            data.set("nav", this->render_tpl("/blog/nav.html"));
            data.set("footer", this->render_tpl("/blog/footer.html"));
            data.set("script", this->render_tpl("/blog/script.html"));
            if (pid) {
                Poco::Data::MySQL::Connector::registerConnector();
                Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
                Poco::Data::Statement select(session);
                int id = 0;
                select << "select `id` from `article` where `user`=? and `id`=? limit 1;",
                        Poco::Data::Keywords::into(id),
                        Poco::Data::Keywords::use(user_id),
                        Poco::Data::Keywords::use(pid),
                        Poco::Data::Keywords::now;
                Poco::Data::MySQL::Connector::unregisterConnector();
                if (!id) {
                    data.set("message", "你无权编辑。");
                    response.send() << this->render_tpl("/blog/message.html", data);
                    return;
                }
            }
            std::string tpl_path("/blog/article.edit.html");
            root_view::root_cache().add(cacheKey, this->render_tpl(tpl_path, data));
            response.send() << *root_view::root_cache().get(cacheKey);
        }

        void article_edit::do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            response.setContentType("application/json;charset=utf-8");
            Poco::JSON::Object result;
            if (!this->session_has(request, response, "user.id")) {
                result.set("ok", false);
                result.set("login", false);
                result.set("message", "尚未登陆。");
                result.stringify(response.send());
                return;
            }

            Poco::Net::HTMLForm form(request, request.stream());

            if (form.has("inputHead") &&
                    form.has("inputAbstract") &&
                    form.has("inputBody") &&
                    form.has("inputKeywords") &&
                    form.has("inputPublish")) {

                Poco::Data::MySQL::Connector::registerConnector();
                Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
                Poco::Data::Statement select(session);
                std::string inputHead = form.get("inputHead"),
                        inputAbstract = form.get("inputAbstract"),
                        inputBody = form.get("inputBody"),
                        inputKeywords = form.get("inputKeywords"),
                        inputPublish = form.get("inputPublish");
                unsigned long pid = form.has("pid") ? Poco::NumberParser::parse(form.get("pid")) : 0;

                unsigned long user_id = this->session_get(request, response, "user.id").convert<unsigned long>();

                if (pid) {
                    select << "update `article` set `head`=?,`body`=? ,`abstract`=?,`keywords`=?,`publish`=? where `id`=? and `user`=?;"
                            , Poco::Data::Keywords::use(inputHead)
                            , Poco::Data::Keywords::use(inputBody)
                            , Poco::Data::Keywords::use(inputAbstract)
                            , Poco::Data::Keywords::use(inputKeywords)
                            , Poco::Data::Keywords::use(inputPublish)
                            , Poco::Data::Keywords::use(pid)
                            , Poco::Data::Keywords::use(user_id);

                } else {
                    select << "insert into `article`(`user`,`head`,`body`,`abstract`,`keywords`,`publish`,`created`)"
                            "values(?,?,?,?,?,?,now());"
                            , Poco::Data::Keywords::use(user_id)
                            , Poco::Data::Keywords::use(inputHead)
                            , Poco::Data::Keywords::use(inputBody)
                            , Poco::Data::Keywords::use(inputAbstract)
                            , Poco::Data::Keywords::use(inputKeywords)
                            , Poco::Data::Keywords::use(inputPublish);
                }

                if (select.execute()) {
                    if (pid) {
                        std::string tmp_uri = Poco::format("/blog/detail/%[0]lu", pid);
                        root_view::root_cache().remove(tmp_uri);
                        root_view::root_cache().remove(Poco::Net::HTTPServerRequest::HTTP_POST + tmp_uri);
                    }
                    int insertId = pid ? pid : Poco::AnyCast<Poco::UInt64>(session.getProperty("insertId"));

                    result.set("ok", true);
                    result.set("message", pid ? "更新成功。" : "发布成功。");
                    result.set("pid", insertId);
                } else {
                    result.set("ok", false);
                    result.set("message", "发布失败。");
                }

                Poco::Data::MySQL::Connector::unregisterConnector();
            } else {
                result.set("ok", false);
                result.set("message", "发布失败。");
            }


            result.stringify(response.send());
        }


    }
}

