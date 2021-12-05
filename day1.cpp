#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include "utils.h"

void day1()  {
    std::string line;
    std::ifstream inputStream("./data-day1.txt");

    int lastDepth;
    getLineAsInt(inputStream, lastDepth);

    int depth;
    int deeperThanLastCounter=0;
    while (getLineAsInt(inputStream, depth)) {
        if (depth > lastDepth) {
            ++deeperThanLastCounter;
        }
        lastDepth = depth;
    }

    std::cout << "--- Day 1 ---\r\n";
    std::cout << "deeperThanLastCounter=" << deeperThanLastCounter << "\r\n";
}

void day1_part2()  {
    std::string line;
    std::ifstream inputStream("./data-day1.txt");

    class SlidingWindow {
    private:
        std::queue<int> _queue;
        int _sum;
    public:
        bool push(std::ifstream& stream) {
            int value;
            if (!getLineAsInt(stream, value)) {
                return false;
            }
            if (_queue.size() == 3) { // window size of 3
                _sum -= _queue.front();
                _queue.pop();
            }
            _queue.push(value);
            _sum += value;
            return true;
        }
        int sum() const {
            return _sum;
        }
    };

    SlidingWindow sw;
    sw.push(inputStream);
    sw.push(inputStream);
    sw.push(inputStream);

    int largerThanLastCounter = 0;
    for (;;) {
        int lastSum = sw.sum();
        if (!sw.push(inputStream)) {
            break;
        }
        if (sw.sum() > lastSum) {
            ++largerThanLastCounter;
        }
    }

    std::cout << "--- Day 1 Part 2---\r\n";
    std::cout << "largerThanLastCounter=" << largerThanLastCounter << "\r\n";
}
