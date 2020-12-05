#include <string>
#include <vector>
#include <map>

class Movie
{
private:
    std::string vectorToString(const std::vector<std::string>& vect) const; 
public:
    std::string movieId;
    std::string name;
    std::string year;
    std::string language;
    
    float avgRating;
    int ratings; 
    
    std::vector<std::string> genres;
    std::vector<std::string> directors;
    std::vector<std::string> actors;
    int score = -1; 

    std::vector<Movie> getRecommendations(std::map<std::string, Movie> movies) const;
    void scoreMovies(std::map<std::string, Movie> movies) const;
    void print() const;
    // Returns the ID used as the key to this movie in an std::map
    std::string getID() const;
};