#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

class Forecast {
    std::vector<std::vector<std::pair<sf::RectangleShape, bool>>> forecast;
    std::vector<std::pair<std::pair<uint16_t, uint16_t>, std::pair<uint16_t, uint16_t>>> paths;
public:
    Forecast();
    Forecast(const Forecast&);
    Forecast(sf::RenderWindow&, const std::string, std::string);
    void operator=(const Forecast&);
    std::vector<std::vector<std::pair<sf::RectangleShape, bool>>>& get();
};