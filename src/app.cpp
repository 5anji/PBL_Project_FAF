#include "app.h"

#include "isNumber.h"
#include "str2int.h"

#include <cmath>
#include <fstream>

// Creating the object
Application::Application(int argc, const char** argv)
        : video_mode(1024, 768)
        , style(sf::Style::Default)
        , title("Test")
        , map_directory("maps/")
        , map_file("default.txt") {
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
                std::string square_width = argv[i + 1];
                std::string square_height = argv[i + 2];
                if (isNumber(square_width) && isNumber(square_height)) {
                    uint16_t temp_width = std::stoi(square_width);
                    uint16_t temp_height = std::stoi(square_height);

                    if ((temp_width >= 200) && (temp_height >= 200)) {
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
            if ((i + 1) < argc) {
                map_file = argv[i + 1];
            } else {
                std::cout << "Invalid expression. Using default map" << std::endl;
            }
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
    window.setFramerateLimit(48);
    generate_map(window);

    sf::Texture texture;
    texture.loadFromFile("textures/background.jpg");
    sf::Sprite background(texture);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) window.close();

        window.clear();
        window.draw(background);

        for (auto&& i : main_map) {
            for (auto&& j : i) {
                window.draw(j);
            }
        }

        window.display();
    }

    return 0;
}

// Generating map from file
void Application::generate_map(sf::RenderWindow& window) {
loop:
    std::ifstream infile;
    infile.open(map_directory + map_file, std::ios::out);
    if (infile.is_open()) {
        std::string temp;
        uint8_t i = 0;
        while (getline(infile, temp)) {
            map.push_back(temp);
            i++;
        }
    } else {
        std::cout << "Error loading map. Using default one" << std::endl;
        map_file = "default.txt";
        infile.close();
        goto loop;
    }

    infile.close();
    // parsing characters to figures:
    sf::Vector2<float> pos(0, 0);
    float square_width = static_cast<float>(window.getSize().x) / static_cast<float>(map[0].size());
    float square_height = static_cast<float>(window.getSize().y) / static_cast<float>(map.size());

    for (size_t i = 0; i < map.size(); i++) {
        std::vector<sf::RectangleShape> temp;
        pos.x = -20;
        for (size_t j = 0; j < map[i].size(); j++) {
            sf::RectangleShape new_square;
            /// @todo: make available for other characters on the map
            if (map[i][j] == '*') {
                new_square.setFillColor(sf::Color(58, 46, 39));
            } else {
                new_square.setFillColor(sf::Color(0, 0, 0, 0));
            }
            new_square.setPosition(pos);
            new_square.setSize(sf::Vector2<float>(square_width, square_height));

            pos.x += square_width;
            temp.push_back(new_square);
        }
        pos.y += square_height;
        main_map.push_back(temp);
    }
}

// Destroying the object
Application::~Application() {
    std::cout << "Exited the app" << std::endl;
}