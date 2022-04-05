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
    void generate_map(sf::RenderWindow& window);
    void generate_forecast(sf::RenderWindow& window);
    uint8_t display_menu(sf::RenderWindow& window);
    void simulate(sf::RenderWindow& window);
    void display_credits(sf::RenderWindow& window);
    ~Application();
};
