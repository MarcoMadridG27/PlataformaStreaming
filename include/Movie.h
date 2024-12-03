#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>

class Movie {
public:
    Movie();

    std::string movieID;
    std::string title;
    std::string synopsis;
    std::vector<std::string> tags;

    Movie(const std::string& id, const std::string& title, const std::string& synopsis, const std::vector<std::string>& tags)
            : movieID(id), title(title), synopsis(synopsis), tags(tags) {}

};

#endif // MOVIE_H
