#ifndef REDDCC_H
#define REDDCC_H

#include <string>
#include <nlohmann/json.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include "http_client.h"
//#include "comment.h"

struct Creds {
  std::string client_id;
  std::string client_secret;
  std::string username;
  std::string password;
  std::string user_agent;
};

namespace reddcc
{
  Creds loadCreds(const std::string& path);

  class Client
  {
    public:
      Client(const std::string& client_id, const std::string& client_secret, 
             const std::string& username, const std::string& password, const std::string& user_agent) 
        : client_id(client_id), client_secret(client_secret), username(username), password(password),
          user_agent(user_agent), httpClient("oauth.reddit.com", "443", user_agent)
      {
        getAccessToken();
      }
      //Comment comment;
    nlohmann::json getPost(const std::string& subreddit, const std::string& sort = "hot", int limit = 25, const std::string& after = "");
	private:
		std::string client_id;
		std::string client_secret;
		std::string username;
		std::string password;
		std::string user_agent;
		std::string access_token;
    std::chrono::system_clock::time_point expires_at;
    char scope;

    http_client::HttpClient httpClient;

    void getAccessToken();
    void checkExpiredToken();


    std::string base64_encode(const std::string& input)
    {
        using namespace boost::archive::iterators;

        std::stringstream os;
        typedef base64_from_binary<transform_width<std::string::const_iterator, 6, 8>> base64_enc; // compose all the above operations in to a new iterator

        std::copy(base64_enc(input.begin()), base64_enc(input.end()), std::ostream_iterator<char>(os));

        return os.str();
    }
  };
}

#endif
