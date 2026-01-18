#pragma once
#include <vector>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "models/thread.hpp"

std::vector<Thread> get_threads();
bool create_thread(const std::string& title, const std::string& summary);