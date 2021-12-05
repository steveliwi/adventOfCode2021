#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void day2() {
    std::string line;
    std::ifstream inputStream("./data-day2.txt");

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

    std::cout << "--- Day 2 ---\r\n";
    std::cout << "horz=" << horz << ", depth=" << depth << "\r\n";
    std::cout << "horz x depth=" << (horz * depth) << "\r\n";
}

void day2_part2() {
    std::string line;
    std::ifstream inputStream("./data-day2.txt");

    int aim = 0;
    int horz = 0;
    int depth = 0;
    while (getline(inputStream, line)) {
        switch (line[0]) {
            case 'f': {
                int x = std::stoi(line.substr(8));
                horz += x;
                depth += (x * aim);
                break;
            }
            case 'd':
                aim += std::stoi(line.substr(5));
                break;
            case 'u':
                aim -= std::stoi(line.substr(3));
                break;
        }
    }

    std::cout << "--- Day 2 Part 2 ---\r\n";
    std::cout << "horz=" << horz << ", depth=" << depth << "\r\n";
    std::cout << "horz x depth=" << (horz * depth) << "\r\n";
}
