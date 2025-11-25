// src/Utils.cpp
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

static inline std::string trim_copy(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) --end;
    return s.substr(start, end - start);
}

std::unordered_map<std::string, std::string> loadConfig(const std::string& path) {
    std::unordered_map<std::string, std::string> out;
    std::ifstream ifs(path);
    if (!ifs) return out; // empty map on failure; caller should handle defaults
    std::string line;
    while (std::getline(ifs, line)) {
        // strip comments
        auto commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        // find key=value
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = trim_copy(line.substr(0, eq));
        std::string val = trim_copy(line.substr(eq + 1));
        if (!key.empty()) out[key] = val;
    }
    return out;
}

std::string configGet(const std::unordered_map<std::string, std::string>& cfg,
                      const std::string& key,
                      const std::string& def) {
    auto it = cfg.find(key);
    if (it == cfg.end()) return def;
    return it->second;
}
