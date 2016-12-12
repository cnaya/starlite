/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   article_qrcode.cpp
 * Author: centos7
 * 
 * Created on December 10, 2016, 9:06 PM
 */

#include <Poco/FileStream.h>
#include <Poco/NumberParser.h>
#include <Poco/StringTokenizer.h>
#include <Poco/URI.h>
#include <Poco/JSON/Object.h>

#include <webcppd/cv_qrcode.hpp>

#include "article_qrcode.hpp"

namespace webcppd {
    namespace blog {

        void article_qrcode::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            std::string uri_path = Poco::URI(request.getURI()).getPath();
            Poco::StringTokenizer st(uri_path, "/", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
 

            Poco::File image_dir_path(this->app.config().getString("http.docroot", "/var/webcppd/www") + "/assets/qrcode");
            if (!image_dir_path.exists()) {
                image_dir_path.createDirectories();
            }
            std::string image_name = st[2] + ".png", image_path = image_dir_path.path() + "/" + image_name;
            if (Poco::File(image_path).exists()) {
                response.redirect("/assets/qrcode/" + image_name, Poco::Net::HTTPResponse::HTTP_FOUND);
                return;
            }


            response.setContentType("image/png");
            std::string post_url = (this->app.config().getBool("http.enableSSL", true) ? "https://" : "http://") + request.getHost() + "/blog/detail/" + st[2];

            webcppd::cv_qrcode image(post_url);
            if (image.created()) {
                Poco::FileOutputStream os(image_path);
                os << image;
                response.redirect("/assets/qrcode/" + image_name, Poco::Net::HTTPResponse::HTTP_FOUND);
                return;
            }
            this->error(request, response);
        }

    }
}
