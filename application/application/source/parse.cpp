#include "parse.hpp"
#include <iomanip>
#include <sstream>

std::string format_time(const std::string& iso_time)
{
    // Example input: 2026-01-19T02:51:04.406517
    std::tm tm{};
    std::istringstream ss(iso_time);

    // Ignore microseconds automatically
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    if (ss.fail())
        return iso_time; // fallback, never crash UI

    std::ostringstream out;
    out << std::put_time(&tm, "%b %d, %Y · %H:%M");
    return out.str();
}