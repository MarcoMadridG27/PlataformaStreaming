#include "../include/MovieTree.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

MovieTree::TreeNode::TreeNode(char key) : key(key), isEndOfWord(false) {}

MovieTree::TreeNode::~TreeNode() {
    for (auto& child : children) {
        delete child.second;
    }
}

MovieTree::MovieTree() : root(new TreeNode('\0')) {}

MovieTree::~MovieTree() {
    deleteTree(root);
}

void MovieTree::deleteTree(TreeNode* node) {
    if (!node) return;
    for (auto& child : node->children) {
        deleteTree(child.second);
    }
    delete node;
}

void MovieTree::insertWord(TreeNode* node, const std::string& word, const std::string& movieID) {
    for (char c : word) {
        c = tolower(c);
        if (!node->children.count(c)) {
            node->children[c] = new TreeNode(c);
        }
        node = node->children[c];
    }
    node->isEndOfWord = true;
    node->movieIDs.insert(movieID);
}

void MovieTree::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo CSV: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, title, synopsis, tags;

        std::getline(ss, id, ',');

        std::getline(ss, title, ',');

        if (line.find('"') != std::string::npos) {
            std::size_t firstQuote = line.find('"');
            std::size_t secondQuote = line.find('"', firstQuote + 1);
            synopsis = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
            line = line.substr(secondQuote + 2);
        } else {
            std::getline(ss, synopsis, ',');
        }

        std::getline(ss, tags, ',');

        id = Utils::trim(id);
        title = Utils::trim(title);
        synopsis = Utils::trim(synopsis);
        tags = Utils::trim(tags);

        Movie movie{id, title, synopsis, {}};
        movieMap[id] = movie;

        insertWord(root, title, id);
        insertWord(root, synopsis, id);

    }
    file.close();
}

std::vector<std::pair<std::string, int>> MovieTree::searchWithRelevance(const std::string& keyword) const {
    std::vector<std::pair<std::string, int>> results;

    std::string lowerKeyword = Utils::trim(keyword);
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

    for (const auto& [id, movie] : movieMap) {
        int relevance = 0;

        std::string lowerTitle = movie.title;
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        if (lowerTitle.find(lowerKeyword) != std::string::npos) {
            relevance += 3;
        }

        std::string lowerSynopsis = movie.synopsis;
        std::transform(lowerSynopsis.begin(), lowerSynopsis.end(), lowerSynopsis.begin(), ::tolower);
        if (lowerSynopsis.find(lowerKeyword) != std::string::npos) {
            relevance += 2;
        }

        if (relevance > 0) {
            results.emplace_back(id, relevance);
        }
    }

    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    return results;
}

std::vector<std::pair<std::string, int>> MovieTree::searchByTag(const std::string& tag) const {
    return searchWithRelevance(tag);
}

Movie MovieTree::getMovie(const std::string& movieID) const {
    auto it = movieMap.find(movieID);
    if (it != movieMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Pelicula no encontrada con el ID: " + movieID);
}

std::string MovieTree::getSynopsis(const std::string& movieID) const {
    auto it = movieMap.find(movieID);
    if (it != movieMap.end()) {
        return it->second.synopsis;
    }
    return "Sinopsis no encontrada.";
}

void MovieTree::addToLiked(const std::string& movieID) {
    if (movieMap.find(movieID) != movieMap.end()) {
        likedMovies.push_back(movieID);
    } else {
        throw std::runtime_error("Pelicula no encontrada con el ID: " + movieID);
    }
}

std::vector<std::string> MovieTree::getLikedMovies() const {
    return likedMovies;
}

void MovieTree::addToWatchLater(const std::string& movieID) {
    if (movieMap.find(movieID) != movieMap.end()) {
        watchLaterMovies.push_back(movieID);
    } else {
        throw std::runtime_error("Pelicula no encontrada con el ID: " + movieID);
    }
}

std::vector<std::string> MovieTree::getWatchLaterMovies() const {
    return watchLaterMovies;
}
