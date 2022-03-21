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
    std::string args;
    std::vector<std::string> map;
    std::vector<std::vector<sf::RectangleShape>> main_map;

public:
    Application(int, const char**);
    void modify_args(int, const char**);
    int8_t start();
    void generate_map(sf::RenderWindow& window);
    uint8_t display_menu(sf::RenderWindow& window);
    void simulate(sf::RenderWindow& window);
    ~Application();
};
