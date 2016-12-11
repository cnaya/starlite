/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   image_upload.hpp
 * Author: centos7
 *
 * Created on December 8, 2016, 3:53 PM
 */

#ifndef IMAGE_UPLOAD_HPP
#define IMAGE_UPLOAD_HPP
#include <webcppd/root_view.hpp>

namespace webcppd {
    namespace blog {

        class image_upload : public root_view {

            virtual void do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                this->error(request, response);
            }

            virtual void do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

            virtual void do_delete(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                this->error(request, response);
            }

            virtual void do_put(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                this->error(request, response);
            }

        };
    }
}



#endif /* IMAGE_UPLOAD_HPP */

