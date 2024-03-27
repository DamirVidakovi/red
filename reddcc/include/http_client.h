#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

// http_client.h
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/rfc2818_verification.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace http_client
{
    class HttpClient
    {
    public:
        HttpClient(const std::string& host, const std::string& port, const std::string& user_agent);

        nlohmann::json post(const std::string& target, const std::string& body, const std::string& content_type, const std::string& auth);
        nlohmann::json get(const std::string& target);

    private:
        std::string host_;
        std::string port_;
        std::string user_agent_;

        nlohmann::json sendRequest(boost::beast::http::verb method, const std::string& target, const std::string& body = "", const std::string& content_type = "", const std::string& auth = "");
    };
}

#endif
