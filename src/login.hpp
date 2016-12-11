/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   login.hpp
 * Author: centos7
 *
 * Created on November 14, 2016, 10:36 AM
 */

#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <webcppd/root_view.hpp>

namespace webcppd {
    namespace blog {

        class login : public root_view {
            virtual void do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

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
#endif /* LOGIN_HPP */

