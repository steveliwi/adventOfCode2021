#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <limits>

typedef std::map<std::string, uint64_t> Polymer;
typedef std::map<std::string, std::string> Rules;
typedef std::map<std::string, uint64_t> Counter;

void processPolymer(const Polymer& src, const Rules& rules, Polymer& dst, Counter& counter) {
    for (auto const& entry : src) {
        std::string elements = entry.first;
        std::string element1 = elements.substr(0, 1);
        std::string element2 = elements.substr(1);
        std::string insert = rules.find(elements)->second;

        dst.insert(std::make_pair(element1 + insert, 0)).first->second += entry.second;
        dst.insert(std::make_pair(insert + element2, 0)).first->second += entry.second;

        counter.insert(std::make_pair(insert, 0)).first->second += entry.second;
    }
}

void getCounterMinMax(const Counter& counter, uint64_t& minValue, uint64_t& maxValue) {
    minValue = std::numeric_limits<uint64_t>::max();
    maxValue = std::numeric_limits<uint64_t>::min();
    for (auto const& entry : counter) {
        if (minValue > entry.second) {
            minValue = entry.second;
        }
        if (maxValue < entry.second) {
            maxValue = entry.second;
        }
    }
}

void day14() {
    std::ifstream inputStream("./data-day14.txt");

    Polymer polymer;
    Rules rules;
    Counter counter;

    std::string line;
    while (getline(inputStream, line)) {
        if (line.empty()) {
            break;
        }
        std::string element1 = line.substr(0, 1);
        ++counter.insert(std::make_pair(element1, 0)).first->second;
        for (size_t n=1; n<line.size(); ++n) {
            std::string element2 = line.substr(n, 1);
            std::string elements = element1 + element2;
            ++polymer.insert(std::make_pair(elements, 0)).first->second;
            ++counter.insert(std::make_pair(element2, 0)).first->second;
            element1 = element2;
        }
    }
    while (getline(inputStream, line)) {
        int pos = line.find(" -> ");
        std::string key(line.substr(0, pos));
        std::string val = line.substr(pos+4);
        rules[key] = val;
    }

    {
        Polymer src = polymer;
        for (int step=0; step<10; ++step) {
            Polymer dst;
            processPolymer(src, rules, dst, counter);
            src = dst;
        }

        std::cout << "--- Day 14 ---" << std::endl;
        uint64_t minValue, maxValue;
        getCounterMinMax(counter, minValue, maxValue);
        std::cout << "max-min=" << (maxValue - minValue) << std::endl;
    }

    {
        Polymer src = polymer;
        for (int step=0; step<40; ++step) {
            Polymer dst;
            processPolymer(src, rules, dst, counter);
            src = dst;
        }

        std::cout << "--- Day 14 Part 2 ---" << std::endl;
        uint64_t minValue, maxValue;
        getCounterMinMax(counter, minValue, maxValue);
        std::cout << "max-min=" << (maxValue - minValue) << std::endl;
    }
}
