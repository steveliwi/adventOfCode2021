#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

bool isLowPoint(const std::vector<std::vector<int>>& grid, int row, int col) {
    const size_t numRows = grid.size();
    const size_t numCols = grid[0].size();
    int centerVal = grid[row][col];

    if ((col+1) < numCols) { // right
        if (grid[row][col+1] <= centerVal) {
            return false;
        }
    }
    if ((col-1) >= 0) { // left
        if (grid[row][col-1] <= centerVal) {
            return false;
        }
    }
    if ((row+1) < numRows) { // bottom
        if (grid[row+1][col] <= centerVal) {
            return false;
        }
    }
    if ((row-1) >= 0) { // top
        if (grid[row-1][col] <= centerVal) {
            return false;
        }
    }
    return true;
}

int computeBasinSize(const std::vector<std::vector<int>>& grid, std::vector<std::vector<bool>>& marked, int row, int col) {
    const size_t numRows = grid.size();
    const size_t numCols = grid[0].size();
    int centerVal = grid[row][col];

    if (marked[row][col]) {
        // this grid position has been accounted for.
        return 0;
    }

    int basinSize = 1;
    marked[row][col] = true; // mark grid position to avoid counting this again.

    if ((col+1) < numCols) { // right
        int rightVal = grid[row][col+1];
        if ((rightVal != 9) && (rightVal > centerVal)) {
            basinSize += computeBasinSize(grid, marked, row, col+1);
        }
    }
    if ((col-1) >= 0) { // left
        int leftVal = grid[row][col-1];
        if ((leftVal != 9) && (leftVal > centerVal)) {
            basinSize += computeBasinSize(grid, marked, row, col-1);
        }
    }
    if ((row+1) < numRows) { // bottom
        int bottomVal = grid[row+1][col];
        if ((bottomVal != 9) && (bottomVal > centerVal)) {
            basinSize += computeBasinSize(grid, marked, row+1, col);
        }
    }
    if ((row-1) >= 0) { // top
        int topVal = grid[row-1][col];
        if ((topVal != 9) && (topVal > centerVal)) {
            basinSize += computeBasinSize(grid, marked, row-1, col);
        }
    }
    return basinSize;
}

void day9() {
    std::ifstream inputStream("./data-day9.txt");

    std::vector<std::vector<int>> grid;

    std::string line;
    while (getline(inputStream, line)) {
        std::vector<int> row;
        for (size_t n=0; n<line.size(); ++n) {
            int val = line[n] - '0';
            row.push_back(val);
        }
        grid.push_back(row);
    }
    const size_t numRows = grid.size();
    const size_t numCols = grid[0].size();

    int riskLevel = 0;
    std::vector<int> basinSizes;
    for (size_t row=0; row < numRows; ++row) {
        for (size_t col=0; col < numCols; ++col) {
            if (isLowPoint(grid, row, col)) {
                riskLevel += (grid[row][col] + 1);

                // create marked grid to avoid counting the same location twice.
                std::vector<std::vector<bool>> marked(numRows, std::vector<bool>(numCols, false));
                int basinSize = computeBasinSize(grid, marked, row, col);
                basinSizes.push_back(basinSize);
            }
        }
    }

    std::cout << "--- Day 9 ---\r\n";
    std::cout << "riskLevel=" << riskLevel << std::endl;

    std::cout << "--- Day 9 Part 2 ---\r\n";

    std::sort(basinSizes.begin(), basinSizes.end());
    auto ritr = basinSizes.rbegin();
    int basinSizesMultiplied = (ritr[0] * ritr[1] * ritr[2]);

    std::cout << "basinSizesMultiplied=" << basinSizesMultiplied << std::endl;
}