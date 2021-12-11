#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

int computeFuelCost(const std::vector<int>& crabPositions, int alignPosition) {
    int cost = 0;
    for (size_t n=0; n<crabPositions.size(); ++n) {
        cost += std::abs(crabPositions[n] - alignPosition);
    }
    return cost;
}

int computeFuelCostPart2(const std::vector<int>& crabPositions, int alignPosition) {
    int totalCost = 0;
    for (size_t n=0; n<crabPositions.size(); ++n) {
        int steps = std::abs(crabPositions[n] - alignPosition);
        int cost = 0;
        for (int step=1; step<=steps; ++step) {
            cost += step;
        }
        totalCost += cost;
    }
    return totalCost;
}

void day7() {
    std::ifstream inputStream("./data-day7.txt");

    int minPos = std::numeric_limits<int>::max();
    int maxPos = 0;
    std::vector<int> crabPositions;

    std::string s;
    while (getline(inputStream, s, ',')) {
        int pos = std::stoi(s);
        minPos = std::min(minPos, pos);
        maxPos = std::max(maxPos, pos);
        crabPositions.push_back(pos);
    }

    // A less brute force way is to calculate the cost at the end points and the middle point
    // and bisect the list.

    {
        int cheapestCost = std::numeric_limits<int>::max();
        int cheapestPos = -1;
        for (size_t pos=minPos; pos<=maxPos; ++pos) {
            int cost = computeFuelCost(crabPositions, pos);
            if (cheapestCost > cost) {
                cheapestCost = cost;
                cheapestPos = pos;
            }
        }

        std::cout << "--- Day 7 ---\r\n";
        std::cout << "cheapestCost=" << cheapestCost << " cheapestPos=" << cheapestPos << std::endl;
    }

    {
        int cheapestCost = std::numeric_limits<int>::max();
        int cheapestPos = -1;
        for (size_t pos=minPos; pos<=maxPos; ++pos) {
            int cost = computeFuelCostPart2(crabPositions, pos);
            if (cheapestCost > cost) {
                cheapestCost = cost;
                cheapestPos = pos;
            }
        }

        std::cout << "--- Day 7 Part 2 ---\r\n";
        std::cout << "cheapestCost=" << cheapestCost << " cheapestPos=" << cheapestPos << std::endl;
    }
}

