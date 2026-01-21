#define NOMINMAX
#include "api/threads_api.hpp"

using json = nlohmann::json;

// Callback to store HTTP response
static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}

std::vector<Thread> get_threads() 
{
    std::vector<Thread> result;
    CURL* curl = curl_easy_init();
    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/threads");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    if (curl_easy_perform(curl) != CURLE_OK) 
    {
        curl_easy_cleanup(curl);
        return result;
    }

    curl_easy_cleanup(curl);

    auto json_threads = json::parse(response);
    for (auto& t : json_threads) 
    {
        result.push_back({
            t["id"],
            t["title"],
            t["summary"],
            format_time(t["time"])
        });
    }

    return result;
}

bool create_thread(const std::string& title, const std::string& summary)
{
    CURL* curl = curl_easy_init();
    std::string response;

    json payload = {
        {"title", title},
        {"summary", summary},
    };

    std::string data = payload.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/threads");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    bool ok = curl_easy_perform(curl) == CURLE_OK;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return ok;
}