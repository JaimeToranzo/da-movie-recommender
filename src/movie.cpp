#include "movie.hpp"

std::string Movie::getID()
{
    return this->name + "_" + this->year;
}