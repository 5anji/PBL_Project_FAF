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
        , forecast_file("forecast.txt")
        , map(nullptr)
        , forecast(nullptr) {
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

                    float report = static_cast<float>(temp_width) / static_cast<float>(temp_height);
                    bool rep_condition = (temp_width >= 200) && (temp_height >= 200) && (0.5f < report) && (report < 2.f);

                    if (rep_condition) {
                        video_mode.width = temp_width;
                        video_mode.height = temp_height;
                    } else {
                        std::cout << "Inacceptable window size. Using 1024x768" << std::endl;
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

    map = new Map(window, map_directory, map_file);
    forecast = new Forecast(window, map_directory, forecast_file);

    bool exit_signal = true;

    do {
        uint8_t entry = display_menu(window);

        switch (entry) {
        case 1:
            simulate(window);
            break;
        case 2: {
            map->get().clear();
            forecast->get().clear();
            map = new Map(window, map_directory, map_file);
            forecast = new Forecast(window, map_directory, forecast_file);
        } break;
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
    const char* titles[] = {"Start Simulation", "Reload Files", "Credits", "Exit"};

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

        for (auto&& i : map->get()) {
            for (auto&& j : i) {
                window.draw(j);
            }
        }

        for (auto&& i : forecast->get()) {
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
    std::wstring credits = L"AeroFly - a project that has the goal to show how the air traffic is managed\nwhen it has to pass through bad weather conditions. The creators are young\n\t\tstudents of the 1st Course at Software Engineering, FAF-213. \n\n\t\t\t\t\t\t\t\t\t\t\t\tCreators: \n\t\t\t\t\t\t\t\t\t - Gîtlan Gabriel (Architector)\n\t\t\t\t\t\t\t\t\t - Maria Afteni (Leader)\n\t\t\t\t\t\t\t\t\t - Iațco Sorin\n\t\t\t\t\t\t\t\t\t - Guțu Dinu\n\t\t\t\t\t\t\t\t\t - Golban Beatrice\n\t\t\t\t\t\t\t\t\t - Chihai Nichita";
    // std::wcout << credits << std::endl;
    sf::Font font;

    if (!font.loadFromFile("fonts/NotoSans-Regular.ttf"))
        std::cout << "Can't find the font file" << std::endl;

    sf::RectangleShape border;
    sf::Vector2<float> credits_size(750, 400);
    sf::Vector2<float> credits_position(window.getSize().x / 2 - 375, window.getSize().y / 2 - 200);

    border.setSize(credits_size);
    border.setPosition(credits_position);
    border.setOutlineThickness(3);
    border.setOutlineColor(sf::Color(37, 114, 203));
    border.setFillColor(sf::Color(255, 255, 255, 127.f));

    sf::Text text;
    text.setFont(font);
    text.setStyle(sf::Text::Bold);
    text.setString(credits);
    text.setFillColor(sf::Color(0, 0, 0));
    text.setCharacterSize(18);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
    text.setPosition(credits_position.x + 375, credits_position.y + 200);

    sf::RectangleShape button;
    sf::RectangleShape overlay;
    sf::Vector2<float> size(30, 30);
    sf::Vector2<float> position(credits_position.x - 40, credits_position.y - 40);

    button.setSize(size);
    overlay.setSize(size);
    button.setPosition(position);
    overlay.setPosition(position);
    button.setFillColor(sf::Color(255, 255, 255, 0.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 0.f));

    sf::Text title;
    title.setFont(font);
    title.setString("X");
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color(0, 0, 0));
    title.setCharacterSize(18);
    sf::FloatRect temp = title.getLocalBounds();
    title.setOrigin(temp.left + temp.width / 2, temp.top + temp.height / 2);
    title.setPosition(position.x + 15, position.y + 15);

    bool breaker = true;
    sf::Texture texture;
    texture.loadFromFile("textures/menu-sky.jpg");
    texture.setSmooth(true);
    sf::Sprite background(texture);

    float step = 0.0015f;

    sf::Vector2<float> b_scale(1.25f, 1.25f);

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
                    overlay.setOutlineColor(sf::Color(37, 114, 203));
                } else {
                    overlay.setFillColor(sf::Color(0, 0, 0, 0));
                    overlay.setOutlineThickness(0);
                    overlay.setOutlineColor(sf::Color(0, 0, 0, 0));
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
        window.draw(button);
        window.draw(overlay);
        window.draw(title);
        window.draw(border);
        window.draw(text);

        window.display();
    }
}

// Destroying the object
Application::~Application() {
    std::cout << "Exited the app" << std::endl;
    delete map;
    delete forecast;
}