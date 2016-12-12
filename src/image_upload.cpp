/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   image_upload.cpp
 * Author: centos7
 * 
 * Created on December 8, 2016, 3:53 PM
 */

#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>

#include <opencv2/opencv.hpp>

#include <webcppd/upload_handler.hpp>

#include "image_upload.hpp"


namespace webcppd {
    namespace blog {

        void image_upload::do_post(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            response.setContentType("application/json;charset=utf-8");
            Poco::JSON::Object image;
            if (!this->session_has(request, response, "user.id")) {
                image.set("success", 0);
                image.set("message", "尚未登陆。");
                image.stringify(response.send());
                return;
            }

            webcppd::upload_handler handler("editormd-image-file|uploadImage"
                    , this->app.config().getString("http.uploadAllowType", "image/png|image/gif|image/webp|image/jpeg")
                    , this->app.config().getString("http.uploadDirectory", "/var/webcppd/www/upload")
                    , this->app.config().getDouble("http.uploadMaxSize", 1048576)
                    );
            Poco::Net::HTMLForm form(request, request.stream(), handler);
            auto result = handler.get_data();
            if (result[0].ok) {
                if (result[0].name == "uploadImage") {
                    cv::Mat src = cv::imread(result[0].savepath), dst;
                    cv::resize(src, dst, cv::Size(64, 64));
                    cv::imwrite(result[0].savepath, dst);

                    Poco::Data::MySQL::Connector::registerConnector();
                    Poco::Data::Session session(Poco::Data::MySQL::Connector::KEY, root_view::mysql_connection_string());
                    Poco::Data::Statement update(session);

                    std::string image;
                    unsigned long uid = this->session_get(request, response, "user.id").convert<unsigned long>();

                    update << "select `image` from `article_user` where `id`=? ;",
                            Poco::Data::Keywords::into(image),
                            Poco::Data::Keywords::use(uid),
                            Poco::Data::Keywords::now;
                    if (!image.empty() && image.find("upload") == std::string::npos) {
                        Poco::File old_image(this->app.config().getString("http.docroot", "/var/webcppd/www") + image);
                        if (old_image.exists()) {
                            old_image.remove();
                        }
                    }
                    update.reset(session);
                    image = "/upload" + result[0].webpath;
                    update << "update `article_user` set `image`=? where `id`=? ;",
                            Poco::Data::Keywords::use(image),
                            Poco::Data::Keywords::use(uid),
                            Poco::Data::Keywords::now;
                    Poco::Data::MySQL::Connector::unregisterConnector();
                }
                image.set("success", 1);
                image.set("message", result[0].message);
                image.set("url", "/upload" + result[0].webpath);
            } else {
                image.set("success", 0);
                image.set("message", result[0].message);
            }

            image.stringify(response.send());
        }

    }
}
