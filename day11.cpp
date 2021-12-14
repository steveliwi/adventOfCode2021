#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h> 

void incrementCell(std::vector<std::vector<int>>& grid, int row, int col, std::vector<std::vector<bool>>& flashed, int& numFlashes) {
    const size_t numRows = grid.size();
    const size_t numCols = grid[0].size();

    if ((row < 0) || (col < 0) || (row >= numRows) || (col >= numCols)) {
        return;
    }

    if (flashed[row][col]) {
        assert(grid[row][col] == 0);
        return;
    }

    int val = ++grid[row][col];
    if (val == 10) {
        grid[row][col] = 0;
        flashed[row][col] = true;
        ++numFlashes;

        incrementCell(grid, row-1, col-1, flashed, numFlashes);
        incrementCell(grid, row-1, col,   flashed, numFlashes);
        incrementCell(grid, row-1, col+1, flashed, numFlashes);
        incrementCell(grid, row,   col-1, flashed, numFlashes);
        incrementCell(grid, row,   col+1, flashed, numFlashes);
        incrementCell(grid, row+1, col-1, flashed, numFlashes);
        incrementCell(grid, row+1, col,   flashed, numFlashes);
        incrementCell(grid, row+1, col+1, flashed, numFlashes);
    }
}

void day11() {
    std::ifstream inputStream("./data-day11.txt");

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

    int totalFlashes = 0;
    int stepWithSyncFlashes = -1;
    for (int step=1; /*step<=100*/; ++step) {
        // create flashed grid to make sure a cell can only flash once per step.
        std::vector<std::vector<bool>> flashed(numRows, std::vector<bool>(numCols, false));

        int numFlashes = 0;
        for (size_t row=0; row<numRows; ++row) {
            for (size_t col=0; col<numCols; ++col) {
                incrementCell(grid, row, col, flashed, numFlashes);
            }
        }
        if (numFlashes == (numRows * numCols)) {
            stepWithSyncFlashes = step;
            break;
        }
        if (step <= 100) {
            totalFlashes += numFlashes;
        }
    }

    std::cout << "--- Day 11 ---" << std::endl;
    std::cout << "totalFlashes=" << totalFlashes << std::endl;

    std::cout << "--- Day 11 Part 2 ---" << std::endl;
    std::cout << "stepWithSyncFlashes=" << stepWithSyncFlashes << std::endl;
}
