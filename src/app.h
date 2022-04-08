#pragma once
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
    std::vector<std::vector<sf::RectangleShape>> map;
    std::vector<std::vector<sf::RectangleShape>> forecast;

public:
    Application(int, const char**);
    void modify_args(int, const char**);
    int8_t start();
    void generate_map(sf::RenderWindow&);
    void generate_forecast(sf::RenderWindow&);
    uint8_t display_menu(sf::RenderWindow&);
    void simulate(sf::RenderWindow&);
    void display_credits(sf::RenderWindow&);
    ~Application();
};
