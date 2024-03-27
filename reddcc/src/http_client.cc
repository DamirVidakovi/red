// http_client.cpp
#include "../include/http_client.h"
#include <iostream>

namespace http_client
{
    HttpClient::HttpClient(const std::string& host, const std::string& port, const std::string& user_agent)
        : host_(host), port_(port), user_agent_(user_agent)
    {
    }

    nlohmann::json HttpClient::post(const std::string& target, const std::string& body, const std::string& content_type, const std::string& auth)
    {
        return sendRequest(boost::beast::http::verb::post, target, body, content_type, auth);
    }

    nlohmann::json HttpClient::get(const std::string& target) 
    {
        return sendRequest(boost::beast::http::verb::get, target);
    }

    nlohmann::json HttpClient::sendRequest(boost::beast::http::verb method, const std::string& target, const std::string& body, const std::string& content_type, const std::string& auth)
    {
        boost::asio::io_context ioc;
        boost::asio::ssl::context ctx{boost::asio::ssl::context::sslv23_client};

        boost::asio::ip::tcp::resolver resolver{ioc};
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream{ioc, ctx};

        auto const results = resolver.resolve(host_, port_);
 
        boost::asio::connect(stream.next_layer(), results.begin(), results.end());
        stream.handshake(boost::asio::ssl::stream_base::client);

        boost::beast::http::request<boost::beast::http::string_body> req{method, target, 11};
        req.set(boost::beast::http::field::host, host_);
        req.set(boost::beast::http::field::user_agent, user_agent_);
        req.set(boost::beast::http::field::authorization, auth); 

        if (method == boost::beast::http::verb::post) {
            req.set(boost::beast::http::field::content_type, content_type);
            req.body() = body;
            req.prepare_payload();
        }

        boost::beast::http::write(stream, req);

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> res;
        boost::beast::http::read(stream, buffer, res);

        auto responseBody = boost::beast::buffers_to_string(res.body().data());

        nlohmann::json jsonResponse = nlohmann::json::parse(responseBody);

        stream.shutdown();

        return jsonResponse;
    }
}