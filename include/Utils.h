#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unordered_set>

extern std::unordered_set<std::string> stopWords;

class Utils {
public:
    static std::unordered_set<std::string> loadStopWords(const std::string& filename);
    static std::string filterText(const std::string& text);
    static std::string trim(const std::string& str);
};

#endif // UTILS_H
