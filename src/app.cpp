#include "app.h"

#include "button.h"

#include <cmath>
#include <fstream>

float step = 0.0015f;

sf::Vector2<float> b_scale(1.25f, 1.25f);

// Creating the object
Application::Application(uint16_t width, uint16_t height, std::string& map, std::string& forecast)
        : video_mode(width, height)
        , style(sf::Style::Close)
        , title("AeroFly")
        , map_directory("maps/")
        , map_file(map)
        , forecast_file(forecast) {

    settings.antialiasingLevel = 4;
}

// Starting the main window
int8_t Application::start() {
    sf::RenderWindow window(video_mode, title, style, settings);
    window.setFramerateLimit(48);
    window.setPosition(sf::Vector2<int>(20, 40));

    map = Map(window, map_directory, map_file);
    forecast = Forecast(window, map_directory, forecast_file);

    bool exit_signal = true;

    do {
        uint8_t entry = display_menu(window);

        switch (entry) {
        case 1:
            simulate(window);
            break;
        case 2: {
            map = Map(window, map_directory, map_file);
            forecast = Forecast(window, map_directory, forecast_file);
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

    std::vector<Button> buttons;
    const char* titles[] = {"Start Simulation", "Reload Files", "Credits", "Exit"};

    for (size_t i = 0; i < 4; i++) {
        Button button(
                sf::Vector2<float>(350, 45),
                sf::Vector2<float>(window.getSize().x / 2 - 175, 300 + (i + 1) * (window.getSize().y / 8) - 22.5f),
                font,
                titles[i],
                sf::Color(37, 114, 203),
                sf::Color(0, 0, 0, 0),
                sf::Color::White,
                36);
        buttons.push_back(button);
    }

    uint8_t return_entry = 0;

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
                    if (i.overlay.getGlobalBounds().contains(mousePosF)) {
                        i.overlay.setFillColor(sf::Color(255, 255, 255, 63.f));
                        i.overlay.setOutlineThickness(3);
                        i.overlay.setOutlineColor(sf::Color(112, 112, 112));
                    } else {
                        i.overlay.setFillColor(sf::Color(0, 0, 0, 0));
                        i.overlay.setOutlineThickness(0);
                        i.overlay.setOutlineColor(sf::Color(0, 0, 0, 0));
                    }
                }

            } break;
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                for (size_t i = 0; i < 4; i++) {
                    if (buttons[i].overlay.getGlobalBounds().contains(mousePosF)) {
                        return_entry = i + 1;
                    }
                }

            } break;
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
            window.draw(i);
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

    Button left(
            sf::Vector2<float>(50, 30),
            sf::Vector2<float>(10, 10),
            font,
            "Back",
            sf::Color(255, 255, 255, 63.f),
            sf::Color(0, 0, 0, 0.5f),
            sf::Color(255, 255, 255, 63.f),
            18);
    Button right(
            sf::Vector2<float>(70, 30),
            sf::Vector2<float>(window.getSize().x - 80, 10),
            font,
            "Reload",
            sf::Color(255, 255, 255, 63.f),
            sf::Color(0, 0, 0, 0.5f),
            sf::Color(255, 255, 255, 63.f),
            18);
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

                if (left.overlay.getGlobalBounds().contains(mousePosF)) {
                    left.overlay.setFillColor(sf::Color(255, 255, 255, 63.f));
                    left.overlay.setOutlineThickness(3);
                    left.overlay.setOutlineColor(sf::Color(112, 112, 112));
                    left.text.setFillColor(sf::Color(255, 255, 255, 255.f));
                } else {
                    left.overlay.setFillColor(sf::Color(0, 0, 0, 0));
                    left.overlay.setOutlineThickness(0);
                    left.overlay.setOutlineColor(sf::Color(0, 0, 0, 0));
                    left.text.setFillColor(sf::Color(255, 255, 255, 63.f));
                }
                if (right.overlay.getGlobalBounds().contains(mousePosF)) {
                    right.overlay.setFillColor(sf::Color(255, 255, 255, 63.f));
                    right.overlay.setOutlineThickness(3);
                    right.overlay.setOutlineColor(sf::Color(112, 112, 112));
                    right.text.setFillColor(sf::Color(255, 255, 255, 255.f));
                } else {
                    right.overlay.setFillColor(sf::Color(0, 0, 0, 0));
                    right.overlay.setOutlineThickness(0);
                    right.overlay.setOutlineColor(sf::Color(0, 0, 0, 0));
                    right.text.setFillColor(sf::Color(255, 255, 255, 63.f));
                }

            } break;
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (left.overlay.getGlobalBounds().contains(mousePosF))
                    breaker = false;
                if (right.overlay.getGlobalBounds().contains(mousePosF)) {
                    map = Map(window, map_directory, map_file);
                    forecast = Forecast(window, map_directory, forecast_file);
                }

            } break;
            default:
                break;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) breaker = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) breaker = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            map = Map(window, map_directory, map_file);
            forecast = Forecast(window, map_directory, forecast_file);
        }

        window.clear();

        for (auto&& i : map.get()) {
            for (auto&& j : i) {
                window.draw(j);
            }
        }

        for (auto&& i : forecast.get()) {
            for (auto&& j : i) {
                window.draw(j.first);
            }
        }

        window.draw(right);
        window.draw(left);

        window.display();
    }
}

// Displaying info about creators
void Application::display_credits(sf::RenderWindow& window) {
    std::wstring credits = L"AeroFly - a project that has the goal to show how the air traffic is managed\nwhen it has to pass through bad weather conditions. The creators are young\n\t\tstudents of the 1st Course at Software Engineering, FAF-213. \n\n\t\t\t\t\t\t\t\t\t\t\t\tCreators: \n\t\t\t\t\t\t\t\t\t - Gîtlan Gabriel (Architector)\n\t\t\t\t\t\t\t\t\t - Maria Afteni (Leader)\n\t\t\t\t\t\t\t\t\t - Iațco Sorin (Visual Builder)\n\t\t\t\t\t\t\t\t\t - Guțu Dinu\n\t\t\t\t\t\t\t\t\t - Golban Beatrice\n\t\t\t\t\t\t\t\t\t - Chihai Nichita";
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

    Button button(
            sf::Vector2<float>(30, 30),
            sf::Vector2<float>(credits_position.x - 40, credits_position.y - 40),
            font,
            "X",
            sf::Color(255, 255, 255, 0.f),
            sf::Color(0, 0, 0, 0.f),
            sf::Color(0, 0, 0),
            18);

    bool breaker = true;
    sf::Texture texture;
    texture.loadFromFile("textures/menu-sky.jpg");
    texture.setSmooth(true);
    sf::Sprite background(texture);

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
                if (button.overlay.getGlobalBounds().contains(mousePosF)) {
                    button.overlay.setFillColor(sf::Color(255, 255, 255, 63.f));
                    button.overlay.setOutlineThickness(3);
                    button.overlay.setOutlineColor(sf::Color(37, 114, 203));
                } else {
                    button.overlay.setFillColor(sf::Color(0, 0, 0, 0));
                    button.overlay.setOutlineThickness(0);
                    button.overlay.setOutlineColor(sf::Color(0, 0, 0, 0));
                }

            } break;
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (button.overlay.getGlobalBounds().contains(mousePosF))
                    breaker = false;

            } break;
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
        window.draw(border);
        window.draw(text);

        window.display();
    }
}

// Destroying the object
Application::~Application() {
    std::cout << "Exited the app" << std::endl;
}