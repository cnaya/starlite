#include <Poco/ClassLibrary.h>
#include <Poco/Net/HTTPRequestHandler.h>

#include "main.hpp"
#include "user_edit.hpp"

POCO_BEGIN_MANIFEST(Poco::Net::HTTPRequestHandler)

POCO_EXPORT_CLASS(webcppd::blog::home)
POCO_EXPORT_CLASS(webcppd::blog::login)
POCO_EXPORT_CLASS(webcppd::blog::logout)
POCO_EXPORT_CLASS(webcppd::blog::article_edit)
POCO_EXPORT_CLASS(webcppd::blog::article_detail)
POCO_EXPORT_CLASS(webcppd::blog::article_search)
POCO_EXPORT_CLASS(webcppd::blog::user_detail)
POCO_EXPORT_CLASS(webcppd::blog::user_edit)
POCO_EXPORT_CLASS(webcppd::blog::image_upload)
POCO_EXPORT_CLASS(webcppd::blog::article_qrcode)
POCO_EXPORT_CLASS(webcppd::blog::error);

POCO_END_MANIFEST