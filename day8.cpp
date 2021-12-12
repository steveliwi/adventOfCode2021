#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

std::string computeCommonSegments(std::string s1, std::string s2) {
    std::string common;
    for (int n=0; n<s1.size(); ++n) {
        char c = s1.at(n);
        if (s2.find(c) != std::string::npos) {
            common += c;
        }
    }
    return common;
}

std::string computeExclusiveSegments(std::string s1, std::string s2) {
    std::string longer = (s1.size() > s2.size()) ? s1 : s2;
    std::string other  = (s1.size() > s2.size()) ? s2 : s1;

    std::string exclusive;
    for (int n=0; n<longer.size(); ++n) {
        char c = longer.at(n);
        if (other.find(c) == std::string::npos) {
            exclusive += c;
        }
    }
    return exclusive;
}

void clearValueInList(std::vector<std::string>& list, std::string val) {
    auto itr = std::find(list.begin(), list.end(), val);
    itr->clear();
}

void day8() {
    class Entry {
        public:
            std::vector<std::string> patterns;
            std::vector<std::string> outputs;
        private:
            static void fillList(std::vector<std::string>& list, std::string s) {
                std::stringstream ss(s);
                std::string pattern;
                list.clear();
                while (getline(ss, pattern, ' ')) {
                    list.push_back(pattern);
                }
            }
        public:
            void fill(std::string s) {
                auto pos = s.find(" | ");
                fillList(patterns, s.substr(0, pos));
                fillList(outputs, s.substr(pos + 3));
            }
    };

    std::ifstream inputStream("./data-day8.txt");
    std::vector<Entry> entries;

    std::string line;
    while (getline(inputStream, line)) {
        Entry entry;
        entry.fill(line);
        entries.push_back(entry);
    }

    {
        int countOf1478 = 0;
        for (size_t entryIndex=0; entryIndex<entries.size(); ++entryIndex) {
            const Entry &entry = entries[entryIndex];
            for (size_t outputIndex=0; outputIndex<entry.outputs.size(); outputIndex++) {
                const std::string& output = entry.outputs[outputIndex];
                switch (output.size()) {
                    case 2: // digit 1
                    case 4: // digit 4
                    case 3: // digit 7
                    case 7: // digit 8
                        ++countOf1478;
                        break;
                }
            }
        }

        std::cout << "--- Day 8 ---\r\n";
        std::cout << "countOf1478=" << countOf1478 << std::endl;
    }

    //   0:      1:      2:      3:      4:
    //  aaaa    ....    aaaa    aaaa    ....
    // b    c  .    c  .    c  .    c  b    c
    // b    c  .    c  .    c  .    c  b    c
    //  ....    ....    dddd    dddd    dddd
    // e    f  .    f  e    .  .    f  .    f
    // e    f  .    f  e    .  .    f  .    f
    //  gggg    ....    gggg    gggg    ....

    //   5:      6:      7:      8:      9:
    //  aaaa    aaaa    aaaa    aaaa    aaaa
    // b    .  b    .  .    c  b    c  b    c
    // b    .  b    .  .    c  b    c  b    c
    //  dddd    dddd    ....    dddd    dddd
    // .    f  e    f  .    f  e    f  .    f
    // .    f  e    f  .    f  e    f  .    f
    //  gggg    gggg    ....    gggg    gggg

    // Segments 'cf' are common between digit 1 and 7
    // Digit 3 : has 5 segments and contains segments 'cf'
    // Digit 6 : has 6 segments and does not contain segments 'cf'

    // Segments 'be' : difference between digit 3 and 8
    // Digit 9 : has 6 segments and does not contain segements 'be'

    // Digit 0: only digit left with 6 segments

    // Segment c : differece between 6 and 8
    // Digit 2 : contains segment c
    // Digit 5 : last digit remaining

    {
        int sumOfAllOutputs = 0;
        for (size_t entryIndex=0; entryIndex<entries.size(); ++entryIndex) {
            const Entry &entry = entries[entryIndex];

            // knownPatterns[0] contains the signal pattern to represent digit 0,
            // knownPatterns[1] contains the signal pattern to represent digit 1, and so on.
            std::vector<std::string> knownPatterns(10); 

            // Keep track of unknown patterns. Known patterns are eliminated from the list as we go.
            std::vector<std::string> unknownPatterns;
            for (size_t patIdx=0; patIdx<entry.patterns.size(); patIdx++) {
                unknownPatterns.push_back(entry.patterns[patIdx]);
            }

            // Digits 1, 4, 7, 8 have unique number of segments
            for (size_t patIdx=0; patIdx<entry.patterns.size(); patIdx++) {
                const std::string& pattern = entry.patterns[patIdx];
                switch (pattern.size()) {
                    case 2: // digit 1
                        knownPatterns[1] = pattern;
                        clearValueInList(unknownPatterns, pattern);
                        break;
                    case 3: // digit 7
                        knownPatterns[7] = pattern;
                        clearValueInList(unknownPatterns, pattern);
                        break;
                    case 4: // digit 4
                        knownPatterns[4] = pattern;
                        clearValueInList(unknownPatterns, pattern);
                        break;
                    case 7: // digit 8
                        knownPatterns[8] = pattern;
                        clearValueInList(unknownPatterns, pattern);
                        break;
                }
            }

            // Segments 'cf' are common between digit 1 and 7
            std::string segment_cf = computeCommonSegments(knownPatterns[1], knownPatterns[7]);

            // Digit 3 : has 5 segments and contains segments 'cf'
            // Digit 6 : has 6 segments and does not contain segments 'cf'
            for (size_t patIdx=0; patIdx<unknownPatterns.size(); patIdx++) {
                const std::string& pattern = unknownPatterns[patIdx];
                if ((pattern.size() == 5) && (computeCommonSegments(pattern, segment_cf).size() == segment_cf.size())) {
                    knownPatterns[3] = pattern;
                    clearValueInList(unknownPatterns, pattern);
                }
                else if ((pattern.size() == 6) && (computeCommonSegments(pattern, segment_cf).size() != segment_cf.size())) {
                    knownPatterns[6] = pattern;
                    clearValueInList(unknownPatterns, pattern);
                }
            }

            // Segments 'be' : difference between digit 3 and 8
            std::string segment_be = computeExclusiveSegments(knownPatterns[3], knownPatterns[8]);

            // Digit 9 : has 6 segments and does not contain segements 'be'
            for (size_t patIdx=0; patIdx<unknownPatterns.size(); patIdx++) {
                const std::string& pattern = unknownPatterns[patIdx];
                if ((pattern.size() == 6) && (computeCommonSegments(pattern, segment_be).size() != segment_cf.size())) {
                    knownPatterns[9] = pattern;
                    clearValueInList(unknownPatterns, pattern);
                    break;
                }
            }

            // Digit 0: only digit left with 6 segments
            for (size_t patIdx=0; patIdx<unknownPatterns.size(); patIdx++) {
                const std::string& pattern = unknownPatterns[patIdx];
                if (pattern.size() == 6) {
                    knownPatterns[0] = pattern;
                    clearValueInList(unknownPatterns, pattern);
                    break;
                }
            }

            // Segment c : differece between 6 and 8
            std::string segment_c = computeExclusiveSegments(knownPatterns[6], knownPatterns[8]);

            // Digit 2 : contains segment c
            for (size_t patIdx=0; patIdx<unknownPatterns.size(); patIdx++) {
                const std::string& pattern = unknownPatterns[patIdx];
                if (pattern.find(segment_c) != std::string::npos) {
                    knownPatterns[2] = pattern;
                    clearValueInList(unknownPatterns, pattern);
                    break;
                }
            }

            // Digit 5 : last digit remaining
            for (size_t patIdx=0; patIdx<unknownPatterns.size(); patIdx++) {
                const std::string& pattern = unknownPatterns[patIdx];
                if (!pattern.empty()) {
                    knownPatterns[5] = pattern;
                    clearValueInList(unknownPatterns, pattern);
                    break;
                }
            }

            // Compute the output
            int outputNumber = 0;
            for (int outputIdx=0; outputIdx<entry.outputs.size(); ++outputIdx) {
                const std::string& output = entry.outputs[outputIdx];

                int digit;
                for (digit=0; digit<knownPatterns.size(); ++digit) {
                    const std::string& knownPattern = knownPatterns[digit];
                    if ((output.size() == knownPattern.size()) && computeCommonSegments(output, knownPattern).size() == output.size()) {
                        break;
                    }
                }

                int factor = std::pow(10, entry.outputs.size() - outputIdx - 1);
                outputNumber += (digit * factor);
            }

            sumOfAllOutputs += outputNumber;
        }

        std::cout << "--- Day 8 Part 2 ---\r\n";
        std::cout << "sumOfAllOutputs: " << sumOfAllOutputs << std::endl;
    }
}
