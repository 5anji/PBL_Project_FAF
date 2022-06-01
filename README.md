# AeroFly

## Description

This app is made by a group of FAF Students. Our app is made for showing, in 2D view, how civil planes can avoid storms and other restricted areas. Works for up to 10 airports(the number can be increased). The default map (default.txt) is based on Europe physical map, and restrictions (forecast.txt) is randomly generated. Remember that those 2 files are created as example, so they are not on 100% right. Of course, the map and restriction files are changeable, just type ```./Aerofly.out --help``` and see other _secret_ features of the applications. If you have ideas how to improve this project feel free to write in **issues**.

## Installation

* Clone the repository

* Download official SFML Libraries (ex. on Ubuntu: ```sudo apt-get install libsfml-dev build-essential -y```)

* Enter the directory using terminal

* Type ```make build``` or ```make```

* Run the ```./AeroFly.out```

## Note for Windows Users

* Install through MSYS2 the necessary packages:

```bash
pacman -Syu --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-sfml
```

* Don't forget to add ```C:\msys64\mingw64\bin``` to PATH from enviroment variables

* Open terminal where you cloned the project and type:

```bash
g++ -c src/main.cpp src/app.cpp src/map.cpp src/forecast.cpp
g++ main.o app.o map.o forecast.o -std=c++17 -Wall -Wextra -O3 -march=native -lsfml-graphics -lsfml-window -lsfml-system -o Aerofly.exe
./Aerofly.exe
```
