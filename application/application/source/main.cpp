#define NOMINMAX

#include <curl/curl.h>
#include "ui/app.hpp"

int main() 
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    RunApp();
    curl_global_cleanup();
    return 0;
}
