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
    inputStream.close();

    std::cout << "--- Day 2 ---\r\n";
    std::cout << "horz=" << horz << ", depth=" << depth << "\r\n";
    std::cout << "horz x depth=" << (horz * depth) << "\r\n";
}
