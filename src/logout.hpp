/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   logout.hpp
 * Author: centos7
 *
 * Created on November 15, 2016, 9:32 PM
 */

#ifndef LOGOUT_HPP
#define LOGOUT_HPP

#include <webcppd/root_view.hpp>

namespace webcppd {
    namespace blog {

        class logout : public root_view {
            virtual void do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

            virtual void do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                this->error(request, response);
            }

            virtual void do_delete(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                this->error(request, response);
            }

            virtual void do_put(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                this->error(request, response);
            }





        };

    }
}

#endif /* LOGOUT_HPP */

