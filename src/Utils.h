// src/Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unordered_map>

/**
 * @file Utils.h
 * @brief Small helpers (config file reader and string utils).
 */

/**
 * @brief Load a simple key=value config file into a map.
 * Lines starting with '#' are comments. Blank lines are ignored.
 * @param path path to config file
 * @return unordered_map of key -> value; empty map if file not found.
 */
std::unordered_map<std::string, std::string> loadConfig(const std::string& path);

/**
 * @brief Get value from config map or default if not present.
 * @param cfg config map
 * @param key key to lookup
 * @param def default to return if key not found
 * @return value associated with key or def
 */
std::string configGet(const std::unordered_map<std::string, std::string>& cfg,
                      const std::string& key,
                      const std::string& def = "");

#endif // UTILS_H
