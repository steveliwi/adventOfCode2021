#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void day5() {
    class Point {
        public:
            int x;
            int y;
        public:
            void fill(std::string s) {
                std::string::size_type pos = s.find(',');
                x = std::stoi(s.substr(0, pos));
                y = std::stoi(s.substr(pos + 1));
            }
    };
    class Vector {
        public:
            Point pt1;
            Point pt2;
        public:
            void fill(std::string s) {
                std::string::size_type pos = s.find(" -> ");
                pt1.fill(s.substr(0, pos));
                pt2.fill(s.substr(pos + 4));
            }
    };

    {
        std::string line;
        std::ifstream inputStream("./data-day5.txt");

        int numOverlaps = 0;
        std::vector<std::vector<int>> grid(1000, std::vector<int>(1000, 0));
        while (getline(inputStream, line)) {
            Vector vector;
            vector.fill(line);

            if (vector.pt1.x == vector.pt2.x) {
                int x = vector.pt1.x;
                int yMin = std::min(vector.pt1.y, vector.pt2.y);
                int yMax = std::max(vector.pt1.y, vector.pt2.y);
                for (int y=yMin; y<=yMax; ++y) {
                    if (++grid[x][y] == 2) {
                        ++numOverlaps;
                    }
                }
            } else if (vector.pt1.y == vector.pt2.y) {
                int y = vector.pt1.y;
                int xMin = std::min(vector.pt1.x, vector.pt2.x);
                int xMax = std::max(vector.pt1.x, vector.pt2.x);
                for (int x=xMin; x<=xMax; ++x) {
                    if (++grid[x][y] == 2) {
                        ++numOverlaps;
                    }
                }
            }
        }

        std::cout << "--- Day 5 ---\r\n";
        std::cout << "numOverlaps=" << numOverlaps << "\r\n";
    }

    // Part 2
    {
        std::string line;
        std::ifstream inputStream("./data-day5.txt");

        int numOverlaps = 0;
        std::vector<std::vector<int>> grid(1000, std::vector<int>(1000, 0));
        while (getline(inputStream, line)) {
            Vector vector;
            vector.fill(line);

            int xInc =
                (vector.pt1.x < vector.pt2.x) ? 1 :
                (vector.pt1.x > vector.pt2.x) ? -1 : 0;

            int yInc =
                (vector.pt1.y < vector.pt2.y) ? 1 :
                (vector.pt1.y > vector.pt2.y) ? -1 : 0;

            int numIterations =
                (vector.pt1.x != vector.pt2.x) ? std::abs(vector.pt1.x - vector.pt2.x) + 1 :
                (vector.pt1.y != vector.pt2.y) ? std::abs(vector.pt1.y - vector.pt2.y) + 1 :
                1; // single point

            int x = vector.pt1.x;
            int y = vector.pt1.y;
            for (int itr=0; itr<numIterations; ++itr, x+=xInc, y+=yInc) {
                if (++grid[x][y] == 2) {
                    ++numOverlaps;
                }
            }
        }

        std::cout << "--- Day 5 Part 2 ---\r\n";
        std::cout << "numOverlaps=" << numOverlaps << "\r\n";
    }
}
