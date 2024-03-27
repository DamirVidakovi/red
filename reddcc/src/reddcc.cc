#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>


#include "../include/reddcc.h"
#include "../include/http_client.h"

void reddcc::Client::getAccessToken()
{
  http_client::HttpClient httpClient("www.reddit.com", "443", user_agent);

  std::string body = "grant_type=password&username=" + username + "&password=" + password;
  std::string content_type = "application/x-www-form-urlencoded";
  std::string auth = "Basic " + base64_encode(client_id + ":" + client_secret) + "=";

  nlohmann::json response = httpClient.post("/api/v1/access_token", body, content_type, auth);

  if (response.contains("access_token"))
  {
    access_token = response["access_token"];
    expires_at = std::chrono::system_clock::now() + std::chrono::seconds(response["expires_in"].get<int>());
    scope = response["scope"].get<std::string>().at(0);
  }
  else
  {
    std::cout << response.dump(4) << std::endl;
    throw std::runtime_error("Failed to get access token");
  }
}

Creds reddcc::loadCreds(const std::string& path)
{
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(path, pt);

  Creds creds;
  creds.client_id = pt.get<std::string>("RedditAuth.client_id");
  creds.client_secret = pt.get<std::string>("RedditAuth.client_secret");
  creds.username = pt.get<std::string>("RedditAuth.username");
  creds.password = pt.get<std::string>("RedditAuth.password");

  return creds;
}

void reddcc::Client::checkExpiredToken()
{
  if (std::chrono::system_clock::now() >= expires_at)
  {
    getAccessToken();
  }
}

nlohmann::json reddcc::Client::getPost(const std::string& subreddit, const std::string& sort, int limit, const std::string& after)
{
  assert(limit > 0 && limit <= 100);
  assert(sort == "hot" || sort == "new" || sort == "top");

  checkExpiredToken();

  // Append the sort type and limit as query parameters to the URL
  std::string url = "/r/" + subreddit + "/" + sort + ".json?limit=" + std::to_string(limit) + "&raw_json=1" + "&show=all";

  if (!after.empty())
  {
    url += "&after=" + after;
  }

  auto response = httpClient.get(url);
  return response;
}
