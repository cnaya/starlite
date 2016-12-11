/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   error.hpp
 * Author: centos7
 *
 * Created on December 7, 2016, 1:18 PM
 */

#ifndef ERROR_HPP
#define ERROR_HPP

#include <webcppd/root_view.hpp>

namespace webcppd {
    namespace blog {

        class error : public root_view {
            virtual void do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

            virtual void do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                root_view::error(request, response);
            }

            virtual void do_delete(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                root_view::error(request, response);
            }

            virtual void do_put(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                root_view::error(request, response);
            }




        };
    }
}


#endif /* ERROR_HPP */

