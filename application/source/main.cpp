#include <iostream>
#include <string>
#include <limits>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Callback to store HTTP response
size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}

void get_threads()
{
    CURL* curl = curl_easy_init();
    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/threads");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "GET failed\n";
        return;
    }

    json threads = json::parse(response);

    /*
    for (const auto& t : threads)
    {
        std::cout << "ID: " << t["id"] << "\n";
        std::cout << "Title: " << t["title"] << "\n";
        std::cout << "Summary: " << t["summary"] << "\n";
        std::cout << "Time: " << t["time"] << "\n\n";
    }
    */
}

void create_thread(const std::string& title, const std::string& summary)
{
    CURL* curl = curl_easy_init();
    std::string response;

    json payload;
    payload["title"] = title;
    payload["summary"] = summary;

    std::string data = payload.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/threads");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "THREAD failed\n";
        return;
    }

    std::cout << "Created thread:\n" << response << "\n";
}

int main() 
{
    int view = 0;
    bool done = false;
    curl_global_init(CURL_GLOBAL_DEFAULT);

    while(!done)
    {
        switch(view)
        {
            case 0: // Title
            {
                int x;
                std::cout << "TITLE!\n\n1) View Threads\n2) Create Threads\n3) Exit\n\n";
                std::cin >> x;
                if (x == 1) view = 1;
                if (x == 2) view = 2;
                if (x == 3) done = true;
                break;
            }
            case 1: // View threads
            {
                break;
            }
            case 2: // Create thread
            {
                std::string title;
                std::string summary;

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Enter title: ";
                std::getline(std::cin, title);

                std::cout << "\nEnter text: ";
                std::getline(std::cin, summary);
                create_thread(title, summary);
                view = 0;
                break;
            }
        }
    }

    curl_global_cleanup();
    return 0;
}