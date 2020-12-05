#include "movie.hpp"
#include <iostream>

std::string Movie::getID()
{
    return this->name + "_" + this->year;
}

void Movie::print()
{
    std::cout << this->name << std::endl;
    std::cout << '\t' << "Year: " << this->year << std::endl;
    std::cout << '\t' << "Genres: ";
    for(size_t i = 0; i < this->genres.size(); i++)
    {
        std::cout << genres[i];
        if(i != this->genres.size()-1)
            std::cout << ", ";
    }
    std::cout << std::endl;
}