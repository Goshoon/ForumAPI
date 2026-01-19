#pragma once
#include <vector>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "models/thread.hpp"
#include "models/post.hpp"

std::vector<Post> get_posts(Thread& current_thread);
bool create_post(Thread& current_thread, const std::string& comment);