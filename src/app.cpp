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
        , map_file("default.txt")
        , forecast_file("forecast.txt") {
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
            if ((i + 1) < argc) {
                forecast_file = argv[i + 1];
            } else {
                std::cout << "Invalid expression. Using default map" << std::endl;
            }
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
    generate_forecast(window);

    bool exit_signal = true;

    do {
        uint8_t entry = display_menu(window);

        switch (entry) {
        case 1:
            simulate(window);
            break;
        case 2:
            break;
        case 3:
            display_credits(window);
            break;
        case 4:
            exit_signal = false;
            break;
        default:
            break;
        }
    } while (exit_signal);

    return 0;
}

// Generating map from file
void Application::generate_map(sf::RenderWindow& window) {
    std::vector<std::string> from_file;
loop:
    std::ifstream infile;
    infile.open(map_directory + map_file, std::ios::out);
    if (infile.is_open()) {
        std::string temp;
        uint8_t i = 0;
        while (getline(infile, temp)) {
            from_file.push_back(temp);
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
    float square_width = static_cast<float>(window.getSize().x) / static_cast<float>(from_file[0].size());
    float square_height = static_cast<float>(window.getSize().y) / static_cast<float>(from_file.size());

    for (size_t i = 0; i < from_file.size(); i++) {
        std::vector<sf::RectangleShape> temp;
        pos.x = 0;
        for (size_t j = 0; j < from_file[i].size(); j++) {
            sf::RectangleShape new_square;

            switch (from_file[i][j]) {
            case '*':
                new_square.setFillColor(sf::Color(34, 139, 34));
                break;
            case '^':
                new_square.setFillColor(sf::Color(192, 192, 192));
                break;
            default:
                new_square.setFillColor(sf::Color(0, 68, 148));
                break;
            }

            new_square.setPosition(pos);
            new_square.setSize(sf::Vector2<float>(square_width, square_height));

            pos.x += square_width;
            temp.push_back(new_square);
        }
        pos.y += square_height;
        map.push_back(temp);
    }
}

// Generating forecast from file
void Application::generate_forecast(sf::RenderWindow& window) {
loop:
    std::vector<std::string> from_file;
    std::ifstream infile;
    infile.open(map_directory + forecast_file, std::ios::out);
    if (infile.is_open()) {
        std::string temp;
        uint8_t i = 0;
        while (getline(infile, temp)) {
            from_file.push_back(temp);
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
    float square_width = static_cast<float>(window.getSize().x) / static_cast<float>(from_file[0].size());
    float square_height = static_cast<float>(window.getSize().y) / static_cast<float>(from_file.size());

    for (size_t i = 0; i < from_file.size(); i++) {
        std::vector<sf::RectangleShape> temp;
        pos.x = 0;
        for (size_t j = 0; j < from_file[i].size(); j++) {
            sf::RectangleShape new_square;
            switch (from_file[i][j]) {
            case '#':
                new_square.setFillColor(sf::Color(0, 0, 0, 127));
                break;
            default:
                new_square.setFillColor(sf::Color(0, 0, 0, 0));
                break;
            }
            new_square.setPosition(pos);
            new_square.setSize(sf::Vector2<float>(square_width, square_height));

            pos.x += square_width;
            temp.push_back(new_square);
        }
        pos.y += square_height;
        forecast.push_back(temp);
    }
}

// Display the startup menu
uint8_t Application::display_menu(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("fonts/Corbel.ttf"))
        std::cout << "Can't find the font file" << std::endl;

    sf::Texture texture;
    texture.loadFromFile("textures/menu-sky.jpg");
    texture.setSmooth(true);
    sf::Sprite background(texture);

    sf::RectangleShape menu_title;
    menu_title.setSize(sf::Vector2<float>(500, 250));
    menu_title.setPosition(window.getSize().x / 2 - 250, 50);
    sf::Texture icon;
    icon.loadFromFile("textures/title.png");
    icon.setSmooth(true);
    menu_title.setTexture(&icon);

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
        button.setFillColor(sf::Color(37, 114, 203));
        overlay.setFillColor(sf::Color(0, 0, 0, 0));

        sf::Text text;
        text.setFont(font);
        text.setStyle(sf::Text::Bold);
        text.setString(titles[i]);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(36);
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
                return_entry = 4;
                break;
            case sf::Event::MouseMoved: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                for (auto&& i : buttons) {
                    if (std::get<1>(i).getGlobalBounds().contains(mousePosF)) {
                        std::get<1>(i).setFillColor(sf::Color(255, 255, 255, 63.f));
                        std::get<1>(i).setOutlineThickness(3);
                        std::get<1>(i).setOutlineColor(sf::Color(112, 112, 112));
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

                for (size_t i = 0; i < 4; i++) {
                    if (std::get<1>(buttons[i]).getGlobalBounds().contains(mousePosF)) {
                        return_entry = i + 1;
                    }
                }

                break;
            }
            default:
                break;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) return_entry = 4;

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
        window.draw(menu_title);
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
    sf::Font font;
    if (!font.loadFromFile("fonts/NotoSans-Regular.ttf"))
        std::cout << "Can't find the font file" << std::endl;
    sf::RectangleShape button;
    sf::RectangleShape overlay;
    sf::Vector2<float> size(50, 30);
    sf::Vector2<float> position(10, 10);

    button.setSize(size);
    overlay.setSize(size);
    button.setPosition(position);
    overlay.setPosition(position);
    button.setFillColor(sf::Color(255, 255, 255, 63.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 0.5f));
    
    sf::Text text;
    text.setFont(font);
    text.setString("Back");
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(255, 255, 255, 63.f));
    text.setCharacterSize(18);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
    text.setPosition(position.x + 25, position.y + 15);

    bool breaker = true;

    while (window.isOpen() && breaker) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                breaker = false;
                break;
            case sf::Event::MouseMoved: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (overlay.getGlobalBounds().contains(mousePosF)) {
                    overlay.setFillColor(sf::Color(255, 255, 255, 63.f));
                    overlay.setOutlineThickness(3);
                    overlay.setOutlineColor(sf::Color(112, 112, 112));
                    text.setFillColor(sf::Color(255, 255, 255, 255.f));
                } else {
                    overlay.setFillColor(sf::Color(0, 0, 0, 0));
                    overlay.setOutlineThickness(0);
                    overlay.setOutlineColor(sf::Color(0, 0, 0, 0));
                    text.setFillColor(sf::Color(255, 255, 255, 63.f));
                }

                break;
            }
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (overlay.getGlobalBounds().contains(mousePosF))
                    breaker = false;

                break;
            }
            default:
                break;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) breaker = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) breaker = false;

        window.clear();

        for (auto&& i : map) {
            for (auto&& j : i) {
                window.draw(j);
            }
        }

        for (auto&& i : forecast) {
            for (auto&& j : i) {
                window.draw(j);
            }
        }

        window.draw(button);
        window.draw(overlay);
        window.draw(text);

        window.display();
    }
}

// Displaying info about creators
void Application::display_credits(sf::RenderWindow& window) {
    std::wstring credits = L"AeroFly - a project that has the goal to show how the air traffic is managed when it have to pass through bad weather conditions. The creators are young students of the 1st Course at Software Engineering, FAF-213. \n\n\tCreators: \nGîtlan Gabriel (Architector)\nMaria Afteni (Leader)\nIațco Sorin\nGuțu Dinu\nGolban Beatrice\nChihai Nichita";
    // std::cout << credits << std::endl;
    sf::Font font;

    if (!font.loadFromFile("fonts/NotoSans-Regular.ttf"))
        std::cout << "Can't find the font file" << std::endl;

    sf::Text text;
    text.setFont(font);
    text.setString(credits);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(0, 0, 0));
    text.setCharacterSize(18);

    bool breaker = true;

    while (window.isOpen() && breaker) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                breaker = false;
                break;
            default:
                break;
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) breaker = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) breaker = false;

        window.clear(sf::Color::White);

        window.draw(text);

        window.display();
    }
}

// Destroying the object
Application::~Application() {
    std::cout << "Exited the app" << std::endl;
}