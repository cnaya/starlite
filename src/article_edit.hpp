/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   article_edit.hpp
 * Author: centos7
 *
 * Created on November 28, 2016, 5:00 PM
 */

#ifndef ARTICLE_EDIT_HPP
#define ARTICLE_EDIT_HPP


#include <webcppd/root_view.hpp>

namespace webcppd {
    namespace blog {
        class article_edit:public root_view{

            virtual void do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

            virtual void do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

            virtual void do_delete(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response){
                this->error(request, response);
            }

            virtual void do_put(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response){
                this->error(request,response);
            }




        };
    }
}

#endif /* ARTICLE_EDIT_HPP */

