/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   error.cpp
 * Author: centos7
 * 
 * Created on December 7, 2016, 1:18 PM
 */

#include "error.hpp"

namespace webcppd {
    namespace blog {

        void error::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html;charset=utf-8");
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
            Kainjow::Mustache::Data data;
            data.set("head", this->render_tpl("/blog/head.html"));
            data.set("nav", this->render_tpl("/blog/nav.html"));
            data.set("footer", this->render_tpl("/blog/footer.html"));
            data.set("script", this->render_tpl("/blog/script.html"));
            data.set("message", "错误访问");
            response.send() << this->render_tpl("/blog/message.html", data);
        }

    }
}
