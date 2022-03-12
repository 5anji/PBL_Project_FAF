#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Application {
    sf::ContextSettings settings;
    sf::Clock clock;
    sf::VideoMode video_mode;
    uint8_t style;
    const char* title;
    std::string args;
public:
    void modify_args(int, const char**);
    Application(int, const char**);
    int8_t start();
    ~Application();
};
