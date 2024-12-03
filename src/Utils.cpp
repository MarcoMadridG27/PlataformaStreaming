#include "../include/Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::unordered_set<std::string> stopWords;

std::unordered_set<std::string> Utils::loadStopWords(const std::string& filename) {
    std::unordered_set<std::string> stopWordsSet;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo de stopWords: " + filename);
    }

    std::string word;
    while (std::getline(file, word)) {
        stopWordsSet.insert(trim(word));
    }

    file.close();
    return stopWordsSet;
}

std::string Utils::filterText(const std::string& text) {
    std::stringstream ss(text);
    std::string word;
    std::string filteredText;

    while (ss >> word) {
        if (stopWords.find(word) == stopWords.end()) {
            filteredText += word + " ";
        }
    }

    return trim(filteredText);
}

std::string Utils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}
