#include "Resources.hpp"
#include <iostream>

Resources* Resources::instance;

Resources& Resources::getResources()
{
    if(instance == nullptr)
        instance = new Resources();
    
    return *instance;
}

void fatalp(const std::string& msg)
{
    std::cerr << "ERROR: " << msg << std::endl;
    exit(EXIT_FAILURE);
}

Resources::Resources()
{
    if(!font.loadFromFile("res/Roboto-Thin.ttf"))
        fatalp("\"res/Roboto-Thin.ttf\" failed to load, aborting!");
}