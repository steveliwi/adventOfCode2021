#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void day1()  {
    std::string line;
    std::ifstream inputStream("./data-day1.txt");

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

    std::cout << "--- Day 1 ---\r\n";
    std::cout << "totalMeasurements=" << totalMeasurements << "\r\n";
    std::cout << "totalDeeperThanLast=" << totalDeeperThanLast << "\r\n";
}
