#include "app.h"
#include <cmath>

Application::Application(int argc, const char** argv) {
    // prefix check
    //    will be here
    settings.antialiasingLevel = 4;
}

int8_t Application::start() {
    sf::RenderWindow window(video_mode, title, style, settings);
    window.setVerticalSyncEnabled(true);

    sf::ConvexShape shape;
    shape.setPointCount(6);
    shape.setPoint(0, sf::Vector2f(0, 1 * 200));
    shape.setPoint(1, sf::Vector2f(0.866025 * 200, 0.5 * 200));
    shape.setPoint(2, sf::Vector2f(0.866025 * 200, -0.5 * 200));
    shape.setPoint(3, sf::Vector2f(0, -1 * 200));
    shape.setPoint(4, sf::Vector2f(-0.866025 * 200, -0.5 * 200));
    shape.setPoint(5, sf::Vector2f(-0.866025 * 200, 0.5 * 200));
    shape.setFillColor(sf::Color::Cyan);
    shape.setOutlineThickness(3);
    shape.setOutlineColor(sf::Color::Red);
    shape.setPosition(640, 360);

    int sign = 1, step = 1, k_delay = 0;
    int i = 0, count = 0;

    sf::Time timer;
    sf::Text debug;
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/TTF/Roboto-Black.ttf");
    timer = sf::milliseconds(1000 / 60);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, float(event.size.width), float(event.size.height));
                window.setView(sf::View(visibleArea));
            }
        }

        if (k_delay == 5) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0)) step = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) step++;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && step > 1) step--;

            //      if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) window.setView(sf::View(sf::Style::Fullscreen));

            debug.setPosition(15, 15);
            debug.setFillColor(sf::Color::Red);
            debug.setFont(font);
            debug.setCharacterSize(14);
            debug.setString("Rotation speed: " + std::to_string(step) + " degrees" + "/sec");

            k_delay = 0;
        }

        sf::sleep(timer);

        float scale;
        scale = float(200 - fabs(i) / 2);
        shape.setPoint(0, sf::Vector2f(0, 1 * scale));
        shape.setPoint(1, sf::Vector2f(float(0.866025) * scale, float(0.5) * scale));
        shape.setPoint(2, sf::Vector2f(float(0.866025) * scale, float(-0.5) * scale));
        shape.setPoint(3, sf::Vector2f(0, -1 * scale));
        shape.setPoint(4, sf::Vector2f(float(-0.866025) * scale, float(-0.5) * scale));
        shape.setPoint(5, sf::Vector2f(float(-0.866025) * scale, float(0.5) * scale));
        shape.setRotation(float(i));
        shape.setPosition(float(window.getSize().x / 2), float(window.getSize().y / 2));
        count++;

        window.clear();
        window.draw(shape);
        window.draw(debug);
        window.display();
        i = i + sign * (step);

        if (i >= 360 || i <= -360) {
            sign *= -1;
        }
        k_delay++;
    }
    return 0;
}

Application::~Application() {
    std::cout << "Exited the app" << std::endl;
}
