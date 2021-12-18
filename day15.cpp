#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

class VertexProxy;

class Vertex {
    public:
        uint32_t row, col;
        uint32_t cost;
        uint32_t costFromStart;
        // uint32_t prev_row, prev_col;
        bool isVisited;
    public:
        Vertex(uint32_t _row, uint32_t _col, uint32_t _cost) :
            row(_row), col(_col),
            cost(_cost),
            costFromStart(std::numeric_limits<uint32_t>::max()),
            // prev_row(-1), prev_col(-1),
            isVisited(false) {
        }
};

typedef std::vector<std::vector<Vertex>> Grid;

Vertex* minCostFromStart(Grid& grid) {
    Vertex* minVertex = nullptr;
    uint32_t minCostFromStart = std::numeric_limits<uint32_t>::max();
 
    for (size_t row = 0; row < grid.size(); ++row) {
        for (size_t col = 0; col < grid[row].size(); ++col) {
            Vertex* v = &grid[row][col];
            if (!v->isVisited && v->costFromStart <= minCostFromStart) {
                minCostFromStart = v->costFromStart;
                minVertex = v;
            }
        }
    }
    return minVertex;
}

void dijkstra(Grid& grid) {
    size_t grid_max_col = grid[0].size();
    size_t grid_max_row = grid.size();

    grid[0][0].costFromStart = 0;

    int lastPercent = -1;
    std::cout << "percent=";

    uint32_t numVisited = 0;
    for (; ;) {
        // can be optimized with a priority queue
        Vertex* uPtr = minCostFromStart(grid);
        if (uPtr == nullptr) {
            break; // all visited
        }
        Vertex& u = *uPtr;
        u.isVisited = true;
        ++numVisited;

        std::vector<Vertex*> neighbours;
        if (u.col + 1 < grid_max_col) { // right
            Vertex& v = grid[u.row][u.col + 1];
            if (!v.isVisited) {
                neighbours.push_back(&v);
            }
        }
        if (u.col > 0) { // left
            Vertex& v = grid[u.row][u.col - 1];
            if (!v.isVisited) {
                neighbours.push_back(&v);
            }
        }
        if (u.row + 1 < grid_max_row) { // bottom
            Vertex& v = grid[u.row + 1][u.col];
            if (!v.isVisited) {
                neighbours.push_back(&v);
            }
        }
        if (u.row > 0) { // top
            Vertex& v = grid[u.row - 1][u.col];
            if (!v.isVisited) {
                neighbours.push_back(&v);
            }
        }

        for (auto v : neighbours) {
            uint32_t alt = u.costFromStart + v->cost;
            if (alt < v->costFromStart) {
                v->costFromStart = alt;
                // v->prev_row = u.row;
                // v->prev_col = u.col;
            }
        }

        // stop when target has reached
        if ((u.row == grid_max_row - 1) && (u.col == grid_max_col - 1)) {
            break;
        }

        // serves as an indicator for part 2
        int percent = (numVisited * 100) / grid_max_col / grid_max_row;
        if (lastPercent != percent) {
            std::cout << percent << " ";
            lastPercent = percent;
        }
    }
    std::cout << std::endl;
}

void day15() {
    std::ifstream inputStream("./data-day15.txt");

    Grid grid;

    std::string line;
    uint32_t row = 0;
    while (getline(inputStream, line)) {
        int col = 0;
        std::vector<Vertex> rowOfCosts;
        for (char c : line) {
            uint32_t cost = c - '0';
            rowOfCosts.emplace_back(row, col++, cost);
        }
        grid.emplace_back(rowOfCosts);
        ++row;
    }
    size_t grid_max_col = grid[0].size();
    size_t grid_max_row = grid.size();

    Grid grid2(grid);
    // duplicate columns 5 times with incremented cost
    for (int n=0; n<4; ++n) {
        for (int row=0; row<grid_max_row; ++row) {
            std::vector<Vertex>& rowOfVertices = grid2[row];
            for (int col=0; col<grid_max_col; ++col) {
                const Vertex& src = rowOfVertices[(n * grid_max_col) + col];
                Vertex dst(row, ((n + 1) * grid_max_col) + col, (src.cost < 9) ? (src.cost + 1) : 1);
                rowOfVertices.push_back(dst);
            }
        }
    }
    // duplicate rows 5 times with incremented cost
    for (int n=0; n<4; ++n) {
        for (int row=0; row<grid_max_row; ++row) {
            std::vector<Vertex>& srcRow = grid2[(n * grid_max_row) + row];
            std::vector<Vertex> dstRow;
            for (int col=0; col<srcRow.size(); ++col) {
                const Vertex& src = srcRow[col];
                Vertex dst(((n + 1) * grid_max_row) + row, col, (src.cost < 9) ? (src.cost + 1) : 1);
                dstRow.push_back(dst);
            }
            grid2.emplace_back(dstRow);
        }
    }
    size_t grid2_max_col = grid2[0].size();
    size_t grid2_max_row = grid2.size();
    
    {
        std::cout << "--- Day 15 ---" << std::endl;

        dijkstra(grid);
        Vertex& dst = grid[grid_max_row-1][grid_max_col-1];

        std::cout << "lowest costFromStart = " << dst.costFromStart << std::endl;
    }

    {
        std::cout << "--- Day 15 Part 2 ---" << std::endl;

        dijkstra(grid2);
        Vertex& dst = grid2[grid2_max_row-1][grid2_max_col-1];

        std::cout << "lowest costFromStart = " << dst.costFromStart << std::endl;
    }
}
