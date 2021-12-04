#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void day1();
void day2();
void day3();
void day4();

const std::string folder("C:\\Dev\\adventOfCode2021\\");

int main() {
    // day1();
    // day2();
    // day3();
    day4();
    return 0;
}

void day1()  {
    std::string line;
    std::ifstream inputStream(std::string(folder).append("data-day1.txt"));

    int totalMeasurements = 0;
    bool isLastDepthDefined = false;
    int lastDepth;
    int totalDeeperThanLast=0;
    while (getline(inputStream, line)) {
        int depth = std::stoi(line);
        if (!isLastDepthDefined) {
            lastDepth = depth;
            isLastDepthDefined = true;
        } else if (depth > lastDepth) {
            ++totalDeeperThanLast;
        }
        lastDepth = depth;
        ++totalMeasurements;
    }
    inputStream.close();

    std::cout << "totalMeasurements=" << totalMeasurements << "\r\n";
    std::cout << "totalDeeperThanLast=" << totalDeeperThanLast << "\r\n";
}

void day2() {
    std::string line;
    std::ifstream inputStream(std::string(folder).append("data-day2.txt"));

    int horz = 0;
    int depth = 0;
    while (getline(inputStream, line)) {
        switch (line[0]) {
            case 'f':
                horz += std::stoi(line.substr(8));
                break;
            case 'd':
                depth += std::stoi(line.substr(5));
                break;
            case 'u':
                depth -= std::stoi(line.substr(3));
                break;
        }
    }
    inputStream.close();

    std::cout << "horz=" << horz << ", depth=" << depth << "\r\n";
    std::cout << "horz x depth=" << (horz * depth) << "\r\n";
}

void day3() {
    std::string line;
    std::ifstream inputStream(std::string(folder).append("data-day3.txt"));

    const int busWidth = 12;
    const unsigned int mask = 0x0000000000000fff;

    int oneCounters[busWidth] = {0};
    int totalEntries = 0;

    while (getline(inputStream, line)) {
        for (int pos=0; pos<busWidth; ++pos) {
            if (line[pos] == '1') {
                int bit = busWidth - 1 - pos;
                ++oneCounters[bit];
            }
        }
        ++totalEntries;
    }
    inputStream.close();

    int gamma = 0;
    for (int bit=0; bit<busWidth; ++bit) {
        if ((oneCounters[bit] * 2) > totalEntries) {
            gamma = gamma | (1 << bit);
        }
    }
    int epsilon = (int)(((unsigned int)~gamma) & mask);

    std::cout << "totalEntries=" << totalEntries << "\r\n";
    std::cout << "gamma=" << gamma << ", epsilon=" << epsilon << "\r\n";
    std::cout << "gamma x epsilon=" << (gamma * epsilon) << "\r\n";
}

void day4() {
    class Board {
        private:
            int numbers[5][5] = {0};
            bool marks[5][5] = {false};
        public:
            bool fillBoard(std::ifstream& inputStream) {
                for (int row=0; row<5; ++row) {
                    std::string line;
                    if (!getline(inputStream, line)) {
                        return false;
                    }
                    fillRow(row, line);
                }
                return true;
            }
            void fillRow(int row, std::string line) {
                std::istringstream lineStream(line);
                int number;
                int col = 0;
                while (lineStream >> number) {
                    numbers[row][col++] = number;
                }
            }
            void markNumber(int number) {
                for (int row=0; row<5; ++row) {
                    for (int col=0; col<5; ++col) {
                        if (numbers[row][col] == number) {
                            marks[row][col] = true;
                        }
                    }
                }
            }
            bool hasWon() const {
                for (int n=0; n<5; n++) {
                    if (isColConnected(n) || isRowConnected(n)) {
                        return true;
                    }
                }
                return false;
            }
            bool isColConnected(int col) const {
                for (int row=0; row<5; ++row) {
                    if (!marks[row][col]) {
                        return false;
                    }
                }
                return true;
            }
            bool isRowConnected(int row) const {
                for (int col=0; col<5; ++col) {
                    if (!marks[row][col]) {
                        return false;
                    }
                }
                return true;
            }
            int computeSumOfUnmarkedNumbers() {
                int sum = 0;
                for (int row=0; row<5; ++row) {
                    for (int col=0; col<5; ++col) {
                        if (!marks[row][col]) {
                            sum += numbers[row][col];
                        }
                    }
                }
                return sum;
            }
    };

    std::string line;
    std::ifstream inputStream(std::string(folder).append("data-day4.txt"));

    std::vector<int> draws;
    getline(inputStream, line);
    std::stringstream lineStream(line);
    while (lineStream.good()) {
        std::string draw;
        getline(lineStream, draw, ',');
        draws.push_back(std::stoi(draw));
    }
    getline(inputStream, line); // blank line

    std::vector<Board> boards;
    for (; ;) {
        Board board;
        if (!board.fillBoard(inputStream)) {
            break;
        }
        boards.push_back(board); // not optimized
        getline(inputStream, line); // blank line
    }

    bool winnerFound = false;
    for (int di=0; di<draws.size() && !winnerFound; ++di) {
        int draw = draws[di];
        for (int bi=0; bi<boards.size(); ++bi) {
            Board &board = boards[bi];
            board.markNumber(draw);
            if (board.hasWon()) {
                int sum = board.computeSumOfUnmarkedNumbers();

                std::cout << "boardIndex=" << bi << "\r\n";
                std::cout << "sum=" << sum << "\r\n";
                std::cout << "winning number=" << draw << "\r\n";
                std::cout << "sum x draw=" << (sum * draw) << "\r\n";
                winnerFound = true;
                break;
            }
        }
    }
    inputStream.close();
}
