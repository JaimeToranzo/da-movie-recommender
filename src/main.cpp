#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <chrono>

#include "movie.hpp"

const std::string TITLE_AKAS = "datasets/title.akas.tsv";
const std::string TITLE_BASICS = "datasets/title.basics.tsv";
const std::string TITLE_RATINGS = "datasets/title.ratings.tsv";
const std::string TITLE_PRINCIPALS = "datasets/title.principals.tsv";
const std::string NAME_BASICS = "datasets/name.basics.tsv";

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
        if (values[1] != "movie" && values[1] != "tvMovie")
            continue;
        if (values[4] == "1")
            continue;

        Movie *movie = new Movie();
        movie->movieId = values[0];
        movie->name = values[2];

        std::istringstream genreBuffer(values[8]);
        std::string genre;
        while (std::getline(genreBuffer, genre, ','))
            movie->genres.insert(genre);

        if (values[5] == "\\N")
            movie->year = "UNKNOWN";
        else
            movie->year = values[5];

        if (moviesByName.count(movie->getID()) != 0)
        {
            //std::cout << "Potential conflict with title: " << movie->getID() << std::endl;

            // Remove the old duplicate from the map
            std::string oldId = moviesByName[movie->getID()]->movieId;
            delete moviesById[oldId];
            moviesById.erase(oldId);
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

bool LoadCastIds(std::map<std::string, Movie *> &moviesById)
{
    std::ifstream file(TITLE_PRINCIPALS);
    if (!file.is_open())
        return false;

    // Skip the first line
    std::string line;
    std::getline(file, line);
    // File headings:
    // tconst(0) ordering(1) nconst(2) category(3) job(4) characters(5)
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

        //std::cout << values[0] << " : " << values[3] << std::endl;

        if (values[3] == "actor" || values[3] == "actress")
            moviesById[values[0]]->actorIds.insert(values[2]);
        else if (values[3] == "director")
            moviesById[values[0]]->directorIds.insert(values[2]);
        else if (values[3] == "writer")
            moviesById[values[0]]->writerIds.insert(values[2]);
    }

    return true;
}

bool LoadCastNames(std::map<std::string, Movie *> &moviesById)
{
    std::ifstream file(NAME_BASICS);
    if (!file.is_open())
        return false;

    // nconst:name
    std::map<std::string, std::string> cast;
    // Skip the first line
    std::string line;
    std::getline(file, line);
    // File headings:
    // nconst(0) primaryName(1) birthYear(2) deathYear(3) primaryProfession(4) knownForTitles(5)
    while (std::getline(file, line))
    {
        std::istringstream buffer(line);
        std::string temp;
        std::vector<std::string> values;

        // Store the first value, the ncount, to use as a key
        std::getline(buffer, temp, '\t');
        std::string nconst = temp;

        std::getline(buffer, temp, '\t');
        std::string name = temp;

        cast[nconst] = name;
    }

    for (auto movie : moviesById)
    {
        for (auto id : movie.second->actorIds)
            movie.second->actors.insert(cast[id]);

        for (auto id : movie.second->directorIds)
            movie.second->directors.insert(cast[id]);

        for (auto id : movie.second->writerIds)
            movie.second->writers.insert(cast[id]);
    }

    return true;
}
bool LoadLanguages(std::map<std::string, Movie *> &moviesById)
{
    std::ifstream file(TITLE_AKAS);
    if (!file.is_open())
        return false;

    // nconst:name
    std::map<std::string, std::string> cast;
    // Skip the first line
    std::string line;
    std::getline(file, line);
    // File headings:
    // titleId(0) ordering(1) title(2) region(3) language(4) types(5) attributes(6) isOriginalTitle(7)
    while (std::getline(file, line))
    {
        std::istringstream buffer(line);
        std::string temp;
        std::vector<std::string> values;

        std::getline(buffer, temp, '\t');
        std::string titleId = temp;

        if(moviesById.count(titleId) == 0)
            continue;

        // Only iterate 3 times so we can get the language
        for(int i = 0; i < 3; i++)
            std::getline(buffer, temp, '\t');
        
        if(temp != "\\N")
            moviesById[titleId]->languages.insert(temp);
    }

    return true;
}

int main()
{
    // Key: NAME_YEAR
    std::map<std::string, Movie *> moviesByName;

    // Stores a ID:NAME pair, so we can have fast lookups by both names and ID
    std::map<std::string, Movie *> moviesById;

    auto startTime = std::chrono::high_resolution_clock::now();
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

    std::cout << "Loading cast..." << std::endl;
    if (!LoadCastIds(moviesById))
    {
        std::cout << "Missing file: " << TITLE_PRINCIPALS << std::endl;
        return 1;
    }

    std::cout << "Processing names..." << std::endl;
    if (!LoadCastNames(moviesById))
    {
        std::cout << "Missing file: " << NAME_BASICS << std::endl;
        return 1;
    }

    std::cout << "Loading languages..." << std::endl;
    if (!LoadLanguages(moviesById))
    {
        std::cout << "Missing file: " << TITLE_AKAS << std::endl;
        return 1;
    }

    auto stopTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
    std::cout << "Loading time: " << duration.count() << "ms" << std::endl;
    
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