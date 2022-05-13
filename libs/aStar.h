#pragma once

#include <SFML/Graphics.hpp>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <iostream>
#include <random>
#include <set>
#include <stack>
#include <vector>

struct Cell {
    uint64_t parent_i, parent_j;
    float f, g, h;
    Cell()
            : parent_i(0)
            , parent_j(0)
            , f(0.0f)
            , g(0.0f)
            , h(0) {}
};

class A_Star {
    const uint64_t ROW, COL;
    std::vector<std::vector<std::pair<sf::RectangleShape, bool>>>* grid;
    std::random_device generator;
    std::uniform_int_distribution<int> distribution;
    sf::Color path_color;

    bool isValid(uint64_t row, uint64_t col) {
        return (row < ROW) && (col < COL);
    }

    bool isUnBlocked(uint64_t row, uint64_t col) {
        return (*grid)[row][col].second ? true : false;
    }

    bool isDestination(uint64_t row, uint64_t col, std::pair<uint64_t, uint64_t>& dest) {
        return row == dest.first && col == dest.second ? true : false;
    }

    float calculateHValue(uint64_t row, uint64_t col, std::pair<uint64_t, uint64_t>& dest) {
        return sqrtf((row - dest.first) * (row - dest.first) + (col - dest.second) * (col - dest.second));
    }

    void tracePath(std::vector<std::vector<Cell>>& cellDetails, std::pair<uint64_t, uint64_t>& dest) {
        uint64_t row = dest.first;
        uint64_t col = dest.second;

        std::stack<std::pair<uint64_t, uint64_t>> Path;

        while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
            Path.push(std::make_pair(row, col));
            uint64_t temp_row = cellDetails[row][col].parent_i;
            uint64_t temp_col = cellDetails[row][col].parent_j;
            row = temp_row;
            col = temp_col;
        }

        Path.push(std::make_pair(row, col));
        while (!Path.empty()) {
            std::pair<uint64_t, uint64_t> p = Path.top();
            Path.pop();
            (*grid)[p.first][p.second].first.setFillColor(path_color);

            for (int8_t i = -1; i < 1; i++) {
                for (int8_t j = -1; j < 1; j++) {
                    (*grid)[p.first + i][p.second + j].second = false;
                }
            }
        }

        return;
    }

public:
    A_Star(uint64_t r, uint64_t c, std::vector<std::vector<std::pair<sf::RectangleShape, bool>>>* grid)
            : ROW(r)
            , COL(c)
            , grid(grid)
            , distribution(0, 1)
            , path_color(distribution(generator) * 255, distribution(generator) * 255, distribution(generator) * 255, 191) {}

    void aStarSearch(std::pair<uint64_t, uint64_t>& src, std::pair<uint64_t, uint64_t>& dest) {
        bool condition(!isValid(src.first, src.second) || !isValid(dest.first, dest.second) || !isUnBlocked(src.first, src.second) || !isUnBlocked(dest.first, dest.second) || isDestination(src.first, src.second, dest));

        if (condition) return;

        std::vector<std::vector<bool>> closedList(ROW, std::vector<bool>(COL, false));

        std::vector<std::vector<Cell>> cellDetails(ROW, std::vector<Cell>(COL, Cell()));

        uint64_t i, j;

        for (i = 0; i < ROW; i++) {
            for (j = 0; j < COL; j++) {
                cellDetails[i][j].f = FLT_MAX;
                cellDetails[i][j].g = FLT_MAX;
                cellDetails[i][j].h = FLT_MAX;
                cellDetails[i][j].parent_i = -1;
                cellDetails[i][j].parent_j = -1;
            }
        }

        i = src.first, j = src.second;
        cellDetails[i][j].f = 0.0;
        cellDetails[i][j].g = 0.0;
        cellDetails[i][j].h = 0.0;
        cellDetails[i][j].parent_i = i;
        cellDetails[i][j].parent_j = j;

        std::set<std::pair<float, std::pair<uint64_t, uint64_t>>> openList;

        openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

        bool foundDest = false;

        while (!openList.empty()) {
            std::pair<float, std::pair<uint64_t, uint64_t>> p = *openList.begin();

            openList.erase(openList.begin());

            i = p.second.first;
            j = p.second.second;
            closedList[i][j] = true;

            float gNew, hNew, fNew;

            if (isValid(i - 1, j)) {

                if (isDestination(i - 1, j, dest)) {

                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
                }

                else if (!closedList[i - 1][j] && isUnBlocked(i - 1, j)) {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i - 1, j, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew) {
                        openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j)));

                        cellDetails[i - 1][j].f = fNew;
                        cellDetails[i - 1][j].g = gNew;
                        cellDetails[i - 1][j].h = hNew;
                        cellDetails[i - 1][j].parent_i = i;
                        cellDetails[i - 1][j].parent_j = j;
                    }
                }
            }

            if (isValid(i + 1, j)) {

                if (isDestination(i + 1, j, dest)) {

                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
                }

                else if (!closedList[i + 1][j] && isUnBlocked(i + 1, j)) {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i + 1, j, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i + 1][j].f == FLT_MAX || cellDetails[i + 1][j].f > fNew) {
                        openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j)));

                        cellDetails[i + 1][j].f = fNew;
                        cellDetails[i + 1][j].g = gNew;
                        cellDetails[i + 1][j].h = hNew;
                        cellDetails[i + 1][j].parent_i = i;
                        cellDetails[i + 1][j].parent_j = j;
                    }
                }
            }

            if (isValid(i, j + 1)) {

                if (isDestination(i, j + 1, dest)) {

                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
                }

                else if (!closedList[i][j + 1] && isUnBlocked(i, j + 1)) {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i, j + 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].f > fNew) {
                        openList.insert(std::make_pair(fNew, std::make_pair(i, j + 1)));

                        cellDetails[i][j + 1].f = fNew;
                        cellDetails[i][j + 1].g = gNew;
                        cellDetails[i][j + 1].h = hNew;
                        cellDetails[i][j + 1].parent_i = i;
                        cellDetails[i][j + 1].parent_j = j;
                    }
                }
            }

            if (isValid(i, j - 1)) {

                if (isDestination(i, j - 1, dest)) {

                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
                }

                else if (!closedList[i][j - 1] && isUnBlocked(i, j - 1)) {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i, j - 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i][j - 1].f == FLT_MAX || cellDetails[i][j - 1].f > fNew) {
                        openList.insert(std::make_pair(fNew, std::make_pair(i, j - 1)));

                        cellDetails[i][j - 1].f = fNew;
                        cellDetails[i][j - 1].g = gNew;
                        cellDetails[i][j - 1].h = hNew;
                        cellDetails[i][j - 1].parent_i = i;
                        cellDetails[i][j - 1].parent_j = j;
                    }
                }
            }

            if (isValid(i - 1, j + 1)) {

                if (isDestination(i - 1, j + 1, dest)) {

                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
                }

                else if (!closedList[i - 1][j + 1] && isUnBlocked(i - 1, j + 1)) {
                    gNew = cellDetails[i][j].g + 1.414;
                    hNew = calculateHValue(i - 1, j + 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i - 1][j + 1].f == FLT_MAX || cellDetails[i - 1][j + 1].f > fNew) {
                        openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j + 1)));

                        cellDetails[i - 1][j + 1].f = fNew;
                        cellDetails[i - 1][j + 1].g = gNew;
                        cellDetails[i - 1][j + 1].h = hNew;
                        cellDetails[i - 1][j + 1].parent_i = i;
                        cellDetails[i - 1][j + 1].parent_j = j;
                    }
                }
            }

            if (isValid(i - 1, j - 1)) {

                if (isDestination(i - 1, j - 1, dest)) {

                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
                }

                else if (!closedList[i - 1][j - 1] && isUnBlocked(i - 1, j - 1)) {
                    gNew = cellDetails[i][j].g + 1.414;
                    hNew = calculateHValue(i - 1, j - 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i - 1][j - 1].f == FLT_MAX || cellDetails[i - 1][j - 1].f > fNew) {
                        openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j - 1)));

                        cellDetails[i - 1][j - 1].f = fNew;
                        cellDetails[i - 1][j - 1].g = gNew;
                        cellDetails[i - 1][j - 1].h = hNew;
                        cellDetails[i - 1][j - 1].parent_i = i;
                        cellDetails[i - 1][j - 1].parent_j = j;
                    }
                }
            }

            if (isValid(i + 1, j + 1)) {

                if (isDestination(i + 1, j + 1, dest)) {

                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
                }

                else if (!closedList[i + 1][j + 1] && isUnBlocked(i + 1, j + 1)) {
                    gNew = cellDetails[i][j].g + 1.414;
                    hNew = calculateHValue(i + 1, j + 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i + 1][j + 1].f == FLT_MAX || cellDetails[i + 1][j + 1].f > fNew) {
                        openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j + 1)));
                        cellDetails[i + 1][j + 1].f = fNew;
                        cellDetails[i + 1][j + 1].g = gNew;
                        cellDetails[i + 1][j + 1].h = hNew;
                        cellDetails[i + 1][j + 1].parent_i = i;
                        cellDetails[i + 1][j + 1].parent_j = j;
                    }
                }
            }

            if (isValid(i + 1, j - 1)) {

                if (isDestination(i + 1, j - 1, dest)) {

                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                    tracePath(cellDetails, dest);
                    foundDest = true;
                    return;
                }

                else if (!closedList[i + 1][j - 1] && isUnBlocked(i + 1, j - 1)) {
                    gNew = cellDetails[i][j].g + 1.414;
                    hNew = calculateHValue(i + 1, j - 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i + 1][j - 1].f == FLT_MAX || cellDetails[i + 1][j - 1].f > fNew) {
                        openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j - 1)));

                        cellDetails[i + 1][j - 1].f = fNew;
                        cellDetails[i + 1][j - 1].g = gNew;
                        cellDetails[i + 1][j - 1].h = hNew;
                        cellDetails[i + 1][j - 1].parent_i = i;
                        cellDetails[i + 1][j - 1].parent_j = j;
                    }
                }
            }
        }
        if (!foundDest) return;
    }
};