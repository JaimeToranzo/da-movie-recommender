#include <string>
#include <vector>
#include <map>

class Movie
{
private:

public:
    std::string movieId;
    std::string language;
    int avgRating;
    std::vector<std::string> genres;
    int score = -1; 

    std::map<std::string, Movie> getRecommendations(std::map<std::string, Movie> movies);
    void scoreMovies(std::map<std::string, Movie> movies);
};