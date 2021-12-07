#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void day6() {
    std::ifstream inputStream("./data-day6.txt");

    // array index is a latern
    // array value is the latern's timer
    std::vector<int> laterns;

    std::string timer;
    while (getline(inputStream, timer, ',')) {
        laterns.push_back(std::stoi(timer));
    }

    for (int day=0; day<80; ++day) {
        int numZeros = 0;
        for (size_t n=0; n<laterns.size(); ++n) {
            if (laterns[n] == 0) {
                ++numZeros;
                laterns[n] = 6;
            } else {
                --laterns[n];
            }
        }
        for (int n=0; n<numZeros; ++n) {
            laterns.push_back(8);
        }
    }

    std::cout << "--- Day 6 ---\r\n";
    std::cout << "numLaterns=" << laterns.size() << "\r\n";
}

uint64_t process(int day, const std::vector<uint64_t>& laterns) {
    if (day == 0) {
        uint64_t numLaterns = 0;
        for (int n=0; n<laterns.size(); ++n) {
            numLaterns += laterns[n];
        }
        return numLaterns;
    }

    std::vector<uint64_t> newLaterns(9, 0);
    newLaterns[8] = laterns[0];
    newLaterns[7] = laterns[8];
    newLaterns[6] = laterns[7] + laterns[0];
    newLaterns[5] = laterns[6];
    newLaterns[4] = laterns[5];
    newLaterns[3] = laterns[4];
    newLaterns[2] = laterns[3];
    newLaterns[1] = laterns[2];
    newLaterns[0] = laterns[1];

    return process(--day, newLaterns);
}

void day6_part2() {
    std::ifstream inputStream("./data-day6.txt");

    // array index represents the timer
    // array value is the number of laterns with that timer
    std::vector<uint64_t> laterns(9, 0); // timer range from 0 to 8

    std::string s;
    while (getline(inputStream, s, ',')) {
        int timer = std::stoi(s);
        ++laterns[timer];
    }

    uint64_t numLaterns = process(256, laterns);
    
    std::cout << "--- Day 6 Part 2 ---\r\n";
    std::cout << "numLaterns=" << numLaterns << "\r\n";
}
