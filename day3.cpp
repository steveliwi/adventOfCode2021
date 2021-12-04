#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void day3() {
    std::string line;
    std::ifstream inputStream("./data-day3.txt");

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

    std::cout << "--- Day 3 ---\r\n";
    std::cout << "totalEntries=" << totalEntries << "\r\n";
    std::cout << "gamma=" << gamma << ", epsilon=" << epsilon << "\r\n";
    std::cout << "gamma x epsilon=" << (gamma * epsilon) << "\r\n";
}
