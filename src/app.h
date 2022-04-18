#pragma once
#include "forecast.h"
#include "map.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

class Application {
    sf::ContextSettings settings;
    sf::Clock clock;
    sf::VideoMode video_mode;
    uint8_t style;
    const char* title;
    const std::string map_directory;
    std::string map_file;
    std::string forecast_file;
    std::string args;
    Map map;
    Forecast forecast;

public:
    Application(int, const char**);
    void modify_args(int, const char**);
    int8_t start();
    uint8_t display_menu(sf::RenderWindow&);
    void simulate(sf::RenderWindow&);
    void display_credits(sf::RenderWindow&);
    ~Application();
};
