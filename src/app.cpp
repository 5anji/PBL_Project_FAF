#include "app.h"

#include "../libs/isNumber.h"
#include "../libs/str2int.h"

#include <cmath>
#include <fstream>

// Creating the object
Application::Application(int argc, const char** argv)
        : video_mode(1024, 768)
        , style(sf::Style::Close)
        , title("AeroFly")
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
    window.setPosition(sf::Vector2<int>(20, 40));

    generate_map(window);

    uint8_t entry = display_menu(window);

    switch (entry) {
    case 1:
        simulate(window);
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
    default:
        break;
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

// Display the startup menu
uint8_t Application::display_menu(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("fonts/DisposableDroidBB.ttf"))
        std::cout << "Can't find the font file" << std::endl;

    sf::Texture texture;
    texture.loadFromFile("textures/menu-sky.jpg");
    texture.setSmooth(true);
    sf::Sprite background(texture);

    std::vector<std::tuple<sf::RectangleShape, sf::RectangleShape, sf::Text>> buttons;
    const char* titles[] = {"Start Simulation", "Options", "Credits", "Exit"};

    for (size_t i = 0; i < 4; i++) {
        sf::RectangleShape button;
        sf::RectangleShape overlay;
        sf::Vector2<float> size(350, 45);
        sf::Vector2<float> position(window.getSize().x / 2 - 175, 300 + (i + 1) * (window.getSize().y / 8) - 22.5f);

        button.setSize(size);
        overlay.setSize(button.getSize());
        button.setPosition(position);
        overlay.setPosition(button.getPosition());
        button.setFillColor(sf::Color(0, 191, 255));
        overlay.setFillColor(sf::Color(0, 0, 0, 0));

        sf::Text text;
        text.setFont(font);
        text.setStyle(sf::Text::Bold);
        text.setString(titles[i]);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(48);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
        text.setPosition(position.x + 175, position.y + 22.5f);

        buttons.push_back(std::make_tuple(button, overlay, text));
    }

    uint8_t return_entry = 0;
    float step = 0.0015f;

    sf::Vector2<float> b_scale(1.25f, 1.25f);

    while (window.isOpen() && !return_entry) {
        sf::Event Event;
        while (window.pollEvent(Event)) {
            switch (Event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseMoved: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                for (auto&& i : buttons) {
                    if (std::get<1>(i).getGlobalBounds().contains(mousePosF)) {
                        std::get<1>(i).setFillColor(sf::Color(255, 255, 255, 85.33f));
                        std::get<1>(i).setOutlineThickness(3);
                        std::get<1>(i).setOutlineColor(sf::Color(255, 0, 0));
                    } else {
                        std::get<1>(i).setFillColor(sf::Color(0, 0, 0, 0));
                        std::get<1>(i).setOutlineThickness(0);
                        std::get<1>(i).setOutlineColor(sf::Color(0, 0, 0, 0));
                    }
                }

                break;
            }
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (std::get<1>(buttons[0]).getGlobalBounds().contains(mousePosF)) {
                    return_entry = 1;
                } else if (std::get<1>(buttons[1]).getGlobalBounds().contains(mousePosF)) {
                    return_entry = 2;
                } else if (std::get<1>(buttons[2]).getGlobalBounds().contains(mousePosF)) {
                    return_entry = 3;
                } else if (std::get<1>(buttons[3]).getGlobalBounds().contains(mousePosF)) {
                    return_entry = 4;
                }

                break;
            }
            default:
                break;
            }
        }

        window.clear();

        // pulsating effect of backgound
        if (b_scale.x < 1.0f || b_scale.x > 1.5f) step *= -1;

        b_scale.x += step;
        b_scale.y += step;

        background.setScale(b_scale);

        float X = window.getSize().x;
        float Y = window.getSize().y;

        background.setPosition((X - X * (b_scale.x)) / 2, (Y - Y * (b_scale.y)) / 2);
        // END

        window.draw(background);

        for (auto&& i : buttons) {
            window.draw(std::get<0>(i));
            window.draw(std::get<1>(i));
            window.draw(std::get<2>(i));
        }

        window.display();
    }

    return return_entry;
}

// Starting simulation
void Application::simulate(sf::RenderWindow& window) {
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
}

// Destroying the object
Application::~Application() {
    std::cout << "Exited the app" << std::endl;
}