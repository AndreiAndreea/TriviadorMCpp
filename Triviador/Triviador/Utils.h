#pragma once
#include <unordered_map>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, const std::string& delim);

std::unordered_map<std::string, std::string> parseUrlArgs(const std::string& urlArgs);