#pragma once
#include <sfml/Graphics/Font.hpp>

class Resources
{
public:
    static Resources& get();
    
    sf::Font font;
    sf::Image windowIcon;
    
private:
    Resources();
    Resources(const Resources&) = delete;
    void operator=(const Resources&) = delete;
    
    static Resources* instance;
};