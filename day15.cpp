#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <queue>

class Vertex {
    public:
        uint32_t row, col;
        uint32_t vertexCost;
        uint32_t pathCost; // cost from start to this vertex
        Vertex* prev;
        bool isVisited;
    public:
        Vertex(uint32_t _row, uint32_t _col, uint32_t _vertexCost) :
            row(_row), col(_col),
            vertexCost(_vertexCost),
            pathCost(std::numeric_limits<uint32_t>::max()),
            prev(nullptr),
            isVisited(false) {
        }
};

typedef std::vector<std::vector<Vertex>> Grid;

class ProxyVertex {
    public:
        Vertex* vertex;
    private:
        uint32_t pathCost;  // keep a local immutable copy to decouple it from the vertex's pathCost, such that priority queue order is maintained for deprecated entries.
    public:
        ProxyVertex(Vertex* _vertex) :
            vertex(_vertex), pathCost(_vertex->pathCost) {
        }
        uint32_t getPathCost() const {
            return pathCost;
        }
        static bool cmp(const ProxyVertex& lhs, const ProxyVertex& rhs) {
            return lhs.pathCost > rhs.pathCost; // ascending
        }
};

void dijkstra(Grid& grid) {
    size_t grid_max_col = grid[0].size();
    size_t grid_max_row = grid.size();

    grid[0][0].pathCost = 0;

    typedef std::priority_queue<ProxyVertex, std::vector<ProxyVertex>, decltype(&ProxyVertex::cmp)> PriorityQueue;
    PriorityQueue q(ProxyVertex::cmp);

    // initialize queue
    for (int row=0; row<grid_max_row; ++row) {
        for (int col=0; col<grid_max_col; ++col) {
            q.emplace(&grid[row][col]);
        }
    }

    int lastPercent = -1;
    std::cout << "progress : ";

    uint32_t numVisited = 0;
    while (!q.empty()) {
        const ProxyVertex& proxy = q.top();
        Vertex& u = *proxy.vertex;
        bool isProxyDeprecated = (proxy.getPathCost() != u.pathCost);
        q.pop();

        if (isProxyDeprecated) {
            continue;
        }

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
            uint32_t alt = u.pathCost + v->vertexCost;
            if (alt < v->pathCost) {
                v->pathCost = alt;
                v->prev = &u;
                // std::priortiy_queue does not support decrease_priority().
                // The workaroud is to insert a new entry with the updated (smaller) pathCost.
                // Since its pathCost is smaller, the new entry will be popped sooner than the original one.
                // The deprecated entry should be ignored when its popped at a later time.
                q.emplace(v);
            }
        }

        // progress indicator
        int percent = (numVisited * 100) / grid_max_col / grid_max_row;
        if (lastPercent != percent) {
            if ((percent % 10) == 0) {
                std::cout << (percent / 10);
             } else {
                 std::cout << ".";
             }
            lastPercent = percent;
        }

        // stop when target has reached
        if ((u.row == grid_max_row - 1) && (u.col == grid_max_col - 1)) {
            break;
        }
    }
    std::cout << std::endl;
}

void displayGrid(const Grid& grid) {
    size_t grid_max_col = grid[0].size();
    size_t grid_max_row = grid.size();

    for (size_t row=0; row<grid_max_row; ++row) {
        for (size_t col=0; col<grid_max_col; ++col) {
            const Vertex& u = grid[row][col];
            std::cout << u.pathCost << " ";
        }
        std::cout << std::endl;
    }
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
                Vertex dst(row, ((n + 1) * grid_max_col) + col, (src.vertexCost < 9) ? (src.vertexCost + 1) : 1);
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
                Vertex dst(((n + 1) * grid_max_row) + row, col, (src.vertexCost < 9) ? (src.vertexCost + 1) : 1);
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
        //displayGrid(grid);

        Vertex& dst = grid[grid_max_row-1][grid_max_col-1];
        std::cout << "lowest pathCost = " << dst.pathCost << std::endl;
    }

    {
        std::cout << "--- Day 15 Part 2 ---" << std::endl;

        dijkstra(grid2);
        // displayGrid(grid2);

        Vertex& dst = grid2[grid2_max_row-1][grid2_max_col-1];
        std::cout << "lowest pathCost = " << dst.pathCost << std::endl;
    }
}
