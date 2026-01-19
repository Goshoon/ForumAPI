#define NOMINMAX
#include "api/posts_api.hpp"
#include "api/threads_api.hpp"

using json = nlohmann::json;

static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}

std::vector<Post> get_posts(Thread& current_thread)
{
    std::string url =
        "http://127.0.0.1:8000/threads/" +
        std::to_string(current_thread.id) +
        "/posts";

    std::vector<Post> result;
    CURL* curl = curl_easy_init();
    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    if (curl_easy_perform(curl) != CURLE_OK)
    {
        curl_easy_cleanup(curl);
        return result;
    }

    curl_easy_cleanup(curl);

    auto json_posts = json::parse(response);
    for (auto& p : json_posts)
    {
        result.push_back({
            p["id"],
            p["comment"],
            p["time"]
            });
    }

    return result;
}

bool create_post(Thread& current_thread, const std::string& comment)
{
    CURL* curl = curl_easy_init();
    std::string response;

    json payload = {
        {"comment", comment}
    };

    std::string data = payload.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    std::string url =
        "http://127.0.0.1:8000/threads/" +
        std::to_string(current_thread.id) +
        "/posts";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    bool ok = curl_easy_perform(curl) == CURLE_OK;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return ok;
}