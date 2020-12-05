#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include "movie.hpp"

const std::string TITLE_BASICS = "datasets/title.basics.tsv";

bool loadMovieBasics(std::map<std::string, Movie *> &movies, std::map<std::string, Movie *> &ids)
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

        if (movies.count(movie->getID()) != 0)
        {
            std::cout << "Potential conflict with title: " << movie->getID() << std::endl;
            delete movies[movie->getID()];
        }
            
        movies[movie->getID()] = movie;
        ids[movie->movieId] = movie;
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
    if (!loadMovieBasics(moviesByName, moviesById))
    {
        std::cout << "Missing file: " << TITLE_BASICS << std::endl;
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

    return 0;
}