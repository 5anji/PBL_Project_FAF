#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

class Forecast {
    std::vector<std::vector<std::pair<sf::RectangleShape, bool>>> forecast;
    std::pair<uint16_t, uint16_t> start, finish;
public:
    Forecast();
    Forecast(const Forecast&);
    Forecast(sf::RenderWindow&, const std::string, std::string);
    void operator=(const Forecast&);
    std::vector<std::vector<std::pair<sf::RectangleShape, bool>>>& get();
};