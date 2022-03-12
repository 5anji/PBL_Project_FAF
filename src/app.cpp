#include "app.h"

#include "isNumber.h"
#include "str2int.h"

#include <cmath>

// Creating the object
Application::Application(int argc, const char** argv)
        : video_mode(1024, 768)
        , style(sf::Style::Default)
        , title("Test") {
    if (argc > 0) {
        modify_args(argc, argv);
    }
    settings.antialiasingLevel = 4;
}

// Modify the default prefix
void Application::modify_args(int argc, const char** argv) {
    for (uint8_t i = 1; i < static_cast<int8_t>(argc); i++) {
        switch (str2int(argv[i])) {
        case str2int("-d"): {
            if ((i + 2) < argc) {
                std::string width = argv[i + 1];
                std::string height = argv[i + 2];
                if (isNumber(width) && isNumber(height)) {
                    uint8_t temp_width = std::stoi(width);
                    uint8_t temp_height = std::stoi(height);

                    if (temp_width >= 200 && temp_height >= 200) {
                        video_mode.width = temp_width;
                        video_mode.height = temp_height;
                    } else {
                        std::cout << "Too small window size. Using 1024x768" << std::endl;
                    }

                } else {
                    std::cout << "Invalid expression. Using 1024x768" << std::endl;
                }
            } else {
                std::cout << "Invalid expression. Using 1024x768" << std::endl;
            }
            break;
        }
        case str2int("-m"):
            // std::cout << argv[i] << std::endl;
            break;
        case str2int("-f"):
            // std::cout << argv[i] << std::endl;
            break;
        case str2int("-h"):
        default:
            /// @todo: display help
            break;
        }
    }
}

// Starting the main window
int8_t Application::start() {
    sf::RenderWindow window(video_mode, title, style, settings);
    window.setVerticalSyncEnabled(true);

    // test code:
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
    font.loadFromFile("fonts/Roboto-Black.ttf");
    timer = sf::milliseconds(1000 / 60);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        if (k_delay == 5) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0)) step = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) step++;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && step > 1) step--;

            debug.setPosition(15, 15);
            debug.setFillColor(sf::Color::Red);
            debug.setFont(font);
            debug.setCharacterSize(14);
            debug.setString("Rotation speed: " + std::to_string(step) + " degrees" + "/sec");

            k_delay = 0;
        }

        sf::sleep(timer);

        float scale;
        scale = 200 - fabs(i) / 2;
        shape.setPoint(0, sf::Vector2f(0, 1 * scale));
        shape.setPoint(1, sf::Vector2f(0.866025 * scale, 0.5 * scale));
        shape.setPoint(2, sf::Vector2f(0.866025 * scale, -0.5 * scale));
        shape.setPoint(3, sf::Vector2f(0, -1 * scale));
        shape.setPoint(4, sf::Vector2f(-0.866025 * scale, -0.5 * scale));
        shape.setPoint(5, sf::Vector2f(-0.866025 * scale, 0.5 * scale));
        shape.setRotation(i);
        shape.setPosition(window.getSize().x / 2, window.getSize().y / 2);
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
    // end

    return 0;
}

// Destroying the object
Application::~Application() {
    std::cout << "Exited the app" << std::endl;
}
