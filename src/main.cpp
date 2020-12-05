#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include "movie.hpp"

const std::string TITLE_BASICS = "datasets/title.basics.tsv";
const std::string TITLE_RATINGS = "datasets/title.ratings.tsv";

bool LoadMovieBasics(std::map<std::string, Movie *> &moviesByName, std::map<std::string, Movie *> &moviesById)
{
    std::ifstream file(TITLE_BASICS);
    if (!file.is_open())
        return false;

    // Skip the first line
    std::string line;
    std::getline(file, line);
    // File headings:
    // tconst(0) titleType(1) primaryTitle(2) originalTitle(3) isAdult(4) startYear(5) endYear(6) runtimeMinutes(7) genres(8)
    while (std::getline(file, line))
    {
        std::istringstream buffer(line);
        std::string temp;
        std::vector<std::string> values;

        while (std::getline(buffer, temp, '\t'))
            values.push_back(temp);

        // Don't add this entry if it isn't a movie or is an adult movie
        if (values[1] != "movie" && values[1] != "tMovie" && values[1] != "short")
            continue;
        if (values[4] == "1")
            continue;

        Movie *movie = new Movie();
        movie->movieId = values[0];
        movie->name = values[2];

        std::istringstream genreBuffer(values[8]);
        std::string genre;
        while (std::getline(genreBuffer, genre, ','))
            movie->genres.push_back(genre);

        if (values[5] == "\\N")
            movie->year = "UNKNOWN";
        else
            movie->year = values[5];

        if (moviesByName.count(movie->getID()) != 0)
        {
            std::cout << "Potential conflict with title: " << movie->getID() << std::endl;

            // Point the old duplicate to the new movie
            std::string oldId = moviesByName[movie->getID()]->movieId;
            delete moviesById[oldId];
            moviesById[oldId] = movie;
        }
            
        moviesByName[movie->getID()] = movie;
        moviesById[movie->movieId] = movie;
    }

    return true;
}

bool LoadRatings(std::map<std::string, Movie *> &moviesById)
{
    std::ifstream file(TITLE_RATINGS);
    if (!file.is_open())
        return false;

    // Skip the first line
    std::string line;
    std::getline(file, line);
    // File headings:
    // tconst(0) averageRating(1) numVotes(2)
    while (std::getline(file, line))
    {
        std::istringstream buffer(line);
        std::string temp;
        std::vector<std::string> values;

        while (std::getline(buffer, temp, '\t'))
            values.push_back(temp);
        
        // Skip over movies not already added to the map
        if (moviesById.count(values[0]) == 0)
            continue;

        moviesById[values[0]]->avgRating = std::stof(values[1]);
        moviesById[values[0]]->ratings = std::stoi(values[2]);
    }

    return true;
}

int main()
{
    // Key: NAME_YEAR
    std::map<std::string, Movie *> moviesByName;

    // Stores a ID:NAME pair, so we can have fast lookups by both names and ID
    std::map<std::string, Movie *> moviesById;

    std::cout << "Loading database..." << std::endl;
    if (!LoadMovieBasics(moviesByName, moviesById))
    {
        std::cout << "Missing file: " << TITLE_BASICS << std::endl;
        return 1;
    }

    std::cout << "Loading ratings..." << std::endl;
    if (!LoadRatings(moviesById))
    {
        std::cout << "Missing file: " << TITLE_RATINGS << std::endl;
        return 1;
    }

    std::string title;
    std::string year;

    std::cout << "Enter a Movie Name: ";
    getline(std::cin, title);
    std::cout << "Enter the year published (type UNKNOWN if the movie has no known date): ";
    std::cin >> year;

    std::string key = title + "_" + year;
    if (moviesByName.count(key) == 0)
    {
        std::cout << "Movie not found!" << std::endl;
        return 2;
    }

    moviesByName[key]->print();
    std::string imdbId = moviesByName[key]->movieId;

    return 0;
}