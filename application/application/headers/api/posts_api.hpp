#pragma once
#include <vector>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "models/post.hpp"

std::vector<Post> get_posts();
bool create_thread(const std::string& title, const std::string& summary);