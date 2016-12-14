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
            
            Poco::SharedPtr<Kainjow::Mustache::Data> data = this->tpl_ready("/blog/config.json", "error.GET");

            response.send() << this->render_tpl(data->get("maintpl")->stringValue(), *data);
        }

    }
}
