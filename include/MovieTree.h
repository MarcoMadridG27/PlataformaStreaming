#ifndef MOVIETREE_H
#define MOVIETREE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Movie.h"

class MovieTree {
private:
    struct TreeNode {
        char key;
        std::unordered_map<char, TreeNode*> children;
        std::unordered_set<std::string> movieIDs;
        bool isEndOfWord;

        TreeNode(char key);
        ~TreeNode();
    };

    TreeNode* root;
    std::unordered_map<std::string, Movie> movieMap;
    std::vector<std::string> likedMovies;
    std::vector<std::string> watchLaterMovies;

    void insertWord(TreeNode* node, const std::string& word, const std::string& movieID);
    void deleteTree(TreeNode* node);
    void collectMovieIDs(TreeNode* node, std::vector<std::string>& results) const;

public:
    MovieTree();
    ~MovieTree();

    void loadFromCSV(const std::string& filename);
    std::vector<std::pair<std::string, int>> searchWithRelevance(const std::string& keyword) const;
    std::vector<std::pair<std::string, int>> searchByTag(const std::string& tag) const;
    Movie getMovie(const std::string& movieID) const;
    std::string getSynopsis(const std::string& movieID) const;

    void addToLiked(const std::string& movieID);
    std::vector<std::string> getLikedMovies() const;
    void addToWatchLater(const std::string& movieID);
    std::vector<std::string> getWatchLaterMovies() const;
};

#endif // MOVIETREE_H
