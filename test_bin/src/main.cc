#include <iostream>
#include <unordered_set>
#include <chrono>
#include <thread>
#include <deque>
#include <nlohmann/json.hpp>
#include "reddcc.h"

int main()
{
    Creds creds = reddcc::loadCreds("C:/Users/vidak/dev/utils/red/creds.ini");
    creds.user_agent = "reddcc/0.1 by u/FluidPirate646";

    reddcc::Client client(creds.client_id,
                          creds.client_secret,
                          creds.username,
                          creds.password,
                          creds.user_agent);


    // wait for new posts in the subreddit "croatia" and print them

    nlohmann::json CurrentResponse;
    nlohmann::json lastResponse;

    std::deque<std::string> lastResponseIdsDeque;
    std::unordered_set<std::string> lastResponseIdsSet;

    while (true)
    {
        std::string after = "";
        if (!lastResponse.empty()) {
            after = lastResponse["data"]["children"][0]["data"]["id"];
        }
        CurrentResponse = client.getPost("croatia", "new", 25, after);

        for (int i = 0; i < CurrentResponse["data"]["children"].size(); i++)
        {
            std::string currentId = CurrentResponse["data"]["children"][i]["data"]["id"];
            if (lastResponseIdsSet.find(currentId) == lastResponseIdsSet.end())
            {
                std::cout << "Title: " << CurrentResponse["data"]["children"][i]["data"]["title"] 
                          << " Author: " << CurrentResponse["data"]["children"][i]["data"]["author"] << std::endl;
                if (lastResponseIdsDeque.size() == 100) {
                    lastResponseIdsSet.erase(lastResponseIdsDeque.front());
                    lastResponseIdsDeque.pop_front();
                }
                lastResponseIdsDeque.push_back(currentId);
                lastResponseIdsSet.insert(currentId);
            }
        }

        lastResponse = CurrentResponse;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

return 0;

}

