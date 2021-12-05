#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "utils.h"

void day3() {
    std::string line;
    std::ifstream inputStream("./data-day3.txt");

    // Number of bits used by all data.
    const int dataWidth = 12;
    const unsigned int dataMask = 0x0000000000000fff;

    // Counts the number of ones in all the data for each bit position.
    // For example, counters[0] is the number of ones occurred in all the data for bit 0.
    int counters[dataWidth] = {0};

    // Number of data entries.
    int totalEntries = 0;

    while (getline(inputStream, line)) {
        for (int charPos=0; charPos<dataWidth; ++charPos) {
            if (line[charPos] == '1') {
                int bitPos = dataWidth - 1 - charPos;
                ++counters[bitPos];
            }
        }
        ++totalEntries;
    }

    unsigned int gamma = 0;
    for (int bitPos=0; bitPos<dataWidth; ++bitPos) {
        if ((counters[bitPos] * 2) > totalEntries) {
            gamma = gamma | (1 << bitPos);
        }
    }
    unsigned int epsilon = ~gamma & dataMask;

    std::cout << "--- Day 3 ---\r\n";
    std::cout << "totalEntries=" << totalEntries << "\r\n";
    std::cout << "gamma=" << gamma << ", epsilon=" << epsilon << "\r\n";
    std::cout << "gamma x epsilon=" << (gamma * epsilon) << "\r\n";
}

int computeRating(const std::vector<int>& dataList, int bitPos, std::vector<int>& zeroList, std::vector<int>& oneList, bool isOxygenRating) {
    if ((dataList.size() == 1) || (bitPos < 0)) {
        return dataList[0];
    }
    // Separate dataList into zeroList and oneList.
    zeroList.clear();
    oneList.clear();
    for (int n=0; n<dataList.size(); ++n) {
        int data = dataList[n];
        if ((data & (1 << bitPos)) != 0) {
            oneList.push_back(data);
        } else {
            zeroList.push_back(data);
        }
    }
    // Determine new data list and call itself recursively.
    std::vector<int> newZerolist, newOneList;
    if (isOxygenRating) {
        std::vector<int>& newDataList = oneList.size() >= zeroList.size() ? oneList : zeroList;
        return computeRating(newDataList, --bitPos, newZerolist, newOneList, isOxygenRating);
    } else {
        std::vector<int>& newDataList = oneList.size() >= zeroList.size() ? zeroList : oneList;
        return computeRating(newDataList, --bitPos, newZerolist, newOneList, isOxygenRating);
    }
}

void day3_part2() {
    std::string line;
    std::ifstream inputStream("./data-day3.txt");

    // Number of bits used by all data.
    const int dataWidth = 12;

    // Read all data entries.
    std::vector<int> dataList;
    int data;
    while (getLineAsInt(inputStream, data, /*base*/2)) {
        dataList.push_back(data);
    }

    // Compute ratings.
    std::vector<int> zeroList, oneList;
    int oxygen = computeRating(dataList, dataWidth - 1, zeroList, oneList, /*isOxygenRating*/true);
    int co2 = computeRating(dataList, dataWidth - 1, zeroList, oneList, /*isOxygenRating*/false);

    std::cout << "--- Day 3 Part 2---\r\n";
    std::cout << "oxygen=" << oxygen << "\r\n";
    std::cout << "co2=" << co2 << "\r\n";
    std::cout << "oxygen x co2=" << (oxygen * co2) << "\r\n";
}
