#include <string>
#include <vector>
#include <map>

class Movie
{
private:

public:
    std::string movieId;
    std::string name;
    std::string year;
    std::string language;
    int avgRating;
    std::vector<std::string> genres;
    int score = -1; 

    std::vector<Movie> getRecommendations(std::map<std::string, Movie> movies);
    void scoreMovies(std::map<std::string, Movie> movies);
    void print();
    // Returns the ID used as the key to this movie in an std::map
    std::string getID();
};