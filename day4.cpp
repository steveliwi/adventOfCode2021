#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
            bool bingo() const {
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
    std::ifstream inputStream("./data-day4.txt");

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
            if (board.bingo()) {
                int sum = board.computeSumOfUnmarkedNumbers();

                std::cout << "--- Day 4 ---\r\n";
                std::cout << "boardIndex=" << bi << "\r\n";
                std::cout << "sum=" << sum << "\r\n";
                std::cout << "winning number=" << draw << "\r\n";
                std::cout << "sum x draw=" << (sum * draw) << "\r\n";
                winnerFound = true;
                break;
            }
        }
    }

    // Part 2
    int numWinners = 0;
    std::vector<bool> winners(boards.size(), false);

    for (int di=0; di<draws.size() && (numWinners<boards.size()); ++di) {
        int draw = draws[di];
        for (int bi=0; bi<boards.size(); ++bi) {
            if (!winners[bi]) {
                Board &board = boards[bi];
                board.markNumber(draw);
                if (board.bingo()) {
                    winners[bi] = true;
                    ++numWinners;
                    if (numWinners == boards.size()) {
                        int sum = board.computeSumOfUnmarkedNumbers();

                        std::cout << "--- Day 4 Part 2 ---\r\n";
                        std::cout << "boardIndex=" << bi << "\r\n";
                        std::cout << "sum=" << sum << "\r\n";
                        std::cout << "winning number=" << draw << "\r\n";
                        std::cout << "sum x draw=" << (sum * draw) << "\r\n";
                        break;
                    }
                }
            }
        }
    }
}
