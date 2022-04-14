#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

class Map {
    std::vector<std::vector<sf::RectangleShape>> map;

public:
    Map();
    Map(const Map&);
    Map(sf::RenderWindow&, const std::string, std::string);
    void operator=(Map*);
    std::vector<std::vector<sf::RectangleShape>>& get();
};