#include "movie.hpp"
#include <iostream>

std::string Movie::getID() const
{
    return this->name + "_" + this->year;
}

void Movie::print() const
{
    std::cout << this->name << std::endl;
    std::cout << '\t' << "Year: " << this->year << std::endl;
    std::cout << '\t' << "Genres: " << vectorToString(this->genres) << std::endl;
    std::cout << '\t' << "Avg. Rating: " << this->avgRating << std::endl;
    std::cout << '\t' << "Number of Ratings: " << this->ratings << std::endl;
}

std::string Movie::vectorToString(const std::vector<std::string>& vect) const
{
    std::string str; 

    for(size_t i = 0; i < this->genres.size(); i++)
    {
        str += genres[i];
        if(i != this->genres.size()-1)
            str += ", ";
    }

    return str;
}