/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   user_detail.hpp
 * Author: centos7
 *
 * Created on December 7, 2016, 5:22 PM
 */

#ifndef USER_DETAIL_HPP
#define USER_DETAIL_HPP

#include <webcppd/root_view.hpp>

namespace webcppd{
    namespace blog{
        class user_detail:public root_view{

            virtual void do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

            virtual void do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response){
                this->error(request,response);
            }

            virtual void do_delete(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response){
                this->error(request,response);
            }

            virtual void do_put(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response){
                this->error(request,response);
            }




        };
    }
}

#endif /* USER_DETAIL_HPP */

