#include "forecast.h"

#include <fstream>
#include <iostream>

Forecast::Forecast() {}
Forecast::Forecast(const Forecast& copy)
        : forecast(copy.forecast) {}

Forecast::Forecast(sf::RenderWindow& window, const std::string map_directory, std::string forecast_file) {
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
        forecast_file = "forecast.txt";
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
void Forecast::operator=(const Forecast& copy) {
    forecast = const_cast<Forecast&>(copy).get();
}

std::vector<std::vector<sf::RectangleShape>>& Forecast::get() {
    return forecast;
}