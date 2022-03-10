#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Application {
    sf::ContextSettings settings;
    sf::Clock clock;
    sf::VideoMode video_mode{1024, 768};
    uint8_t style = sf::Style::Default;
    const char* title = "Test";
public:
    Application(int, const char**);
    int8_t start();
    ~Application();
};
