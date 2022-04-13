#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

class Forecast {
    std::vector<std::vector<sf::RectangleShape>> forecast;

public:
    Forecast();
    Forecast(const Forecast&);
    Forecast(sf::RenderWindow&, const std::string, std::string);
    void operator=(Forecast);
    std::vector<std::vector<sf::RectangleShape>>& get();
};