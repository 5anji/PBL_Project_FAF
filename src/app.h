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
    void generate_map(sf::RenderWindow& window);
    int8_t start();
    ~Application();
};
