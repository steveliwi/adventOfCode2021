#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

void day8() {
    class Segments {
        private:
            // probably better if we use a std::set
            std::string _val;
        public:
            Segments() : _val() {
            }
            Segments(const Segments& segment) {
                this->_val = segment._val;
            }
            Segments(const std::string& val) {
                _val = val;
            }
        public:
            size_t size() const {
                return _val.size();
            }
            bool contains(const Segments& other) const {
                for (int n=0; n<other._val.size(); ++n) {
                    char c = other._val.at(n);
                    if (this->_val.find(c) == std::string::npos) {
                        return false;
                    }
                }
                return true;
            }
            Segments exclude(const Segments& other) const {
                std::string result;
                for (int n=0; n<this->_val.size(); ++n) {
                    char c = this->_val.at(n);
                    if (other._val.find(c) == std::string::npos) {
                        result += c;
                    }
                }
                return result;
            }
    };
    class Entry {
        public:
            std::vector<Segments> patterns;
            std::vector<Segments> outputs;
        private:
            static void setupList(std::vector<Segments>& list, const std::string& s) {
                std::stringstream ss(s);
                std::string pattern;
                list.clear();
                while (getline(ss, pattern, ' ')) {
                    list.push_back(pattern);
                }
            }
        public:
            Entry(const std::string& s) {
                auto pos = s.find(" | ");
                setupList(patterns, s.substr(0, pos));
                setupList(outputs, s.substr(pos + 3));
            }
    };

    std::ifstream inputStream("./data-day8.txt");
    std::vector<Entry> entries;

    std::string line;
    while (getline(inputStream, line)) {
        entries.emplace_back(line);
    }

    {
        int countOf1478 = 0;
        for (size_t entryIdx=0; entryIdx<entries.size(); ++entryIdx) {
            const Entry &entry = entries[entryIdx];
            for (size_t outputIdx=0; outputIdx<entry.outputs.size(); outputIdx++) {
                const Segments& output = entry.outputs[outputIdx];
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

    // Segments 'cf' : digit 1
    // Digit 3 : has 5 segments and contains segments 'cf'
    // Digit 6 : has 6 segments and does not contain segments 'cf'

    // Segments 'be' : difference between digit 3 and 8
    // Digit 9 : has 6 segments and does not contain segements 'be'

    // Digit 0: only digit left with 6 segments

    // Segment c : difference between 6 and 8
    // Digit 2 : contains segment c
    // Digit 5 : last digit remaining

    {
        int sumOfAllOutputs = 0;
        for (size_t entryIdx=0; entryIdx<entries.size(); ++entryIdx) {
            const Entry &entry = entries[entryIdx];

            // knownPatterns[n] contains the signal pattern to represent digit n
            // e.g. knownPatterns[0] contains the signal pattern to represent digit 0
            std::vector<Segments> knownPatterns(10); 

            // Keep track of unknown patterns. Known patterns are eliminated from the list as we go.
            // Probably not the most efficient using vector.
            std::vector<Segments> unknownPatterns;
            for (size_t patIdx=0; patIdx<entry.patterns.size(); patIdx++) {
                unknownPatterns.push_back(entry.patterns[patIdx]);
            }

            // Digits 1, 4, 7, 8 have unique number of segments
            for (auto itr = unknownPatterns.begin(); itr != unknownPatterns.end(); ) {
                const Segments& pattern = *itr;
                switch (pattern.size()) {
                    case 2: // digit 1
                        knownPatterns[1] = pattern;
                        itr = unknownPatterns.erase(itr);
                        break;
                    case 3: // digit 7
                        knownPatterns[7] = pattern;
                        itr = unknownPatterns.erase(itr);
                        break;
                    case 4: // digit 4
                        knownPatterns[4] = pattern;
                        itr = unknownPatterns.erase(itr);
                        break;
                    case 7: // digit 8
                        knownPatterns[8] = pattern;
                        itr = unknownPatterns.erase(itr);
                        break;
                    default:
                        ++itr;
                        break;
                }
            }

            // Segments 'cf' : digit 1
            const Segments& segments_cf = knownPatterns[1];

            // Digit 3 : has 5 segments and contains segments 'cf'
            // Digit 6 : has 6 segments and does not contain segments 'cf'
            for (auto itr = unknownPatterns.begin(); itr != unknownPatterns.end(); ) {
                const Segments& pattern = *itr;
                if ((pattern.size() == 5) && pattern.contains(segments_cf)) {
                    knownPatterns[3] = pattern;
                    itr = unknownPatterns.erase(itr);
                }
                else if ((pattern.size() == 6) && !pattern.contains(segments_cf)) {
                    knownPatterns[6] = pattern;
                    itr = unknownPatterns.erase(itr);
                } else {
                    ++itr;
                }
            }

            // Segments 'be' : difference between digit 3 and 8
            const Segments& segments_be = knownPatterns[8].exclude(knownPatterns[3]);

            // Digit 9 : has 6 segments and does not contain segements 'be'
            for (auto itr = unknownPatterns.begin(); itr != unknownPatterns.end(); ++itr) {
                const Segments& pattern = *itr;
                if ((pattern.size() == 6) && !pattern.contains(segments_be)) {
                    knownPatterns[9] = pattern;
                    unknownPatterns.erase(itr);
                    break;
                }
            }

            // Digit 0: only digit left with 6 segments
            for (auto itr = unknownPatterns.begin(); itr != unknownPatterns.end(); ++itr) {
                const Segments& pattern = *itr;
                if (pattern.size() == 6) {
                    knownPatterns[0] = pattern;
                    unknownPatterns.erase(itr);
                    break;
                }
            }

            // Segment c : difference between 6 and 8
            const Segments& segment_c = knownPatterns[8].exclude(knownPatterns[6]);

            // Digit 2 : contains segment c
            for (auto itr = unknownPatterns.begin(); itr != unknownPatterns.end(); ++itr) {
                const Segments& pattern = *itr;
                if (pattern.contains(segment_c)) {
                    knownPatterns[2] = pattern;
                    unknownPatterns.erase(itr);
                    break;
                }
            }

            // Digit 5 : last digit remaining
            knownPatterns[5] = unknownPatterns[0];

            // Compute the output
            int outputNumber = 0;
            for (int outputIdx=0; outputIdx<entry.outputs.size(); ++outputIdx) {
                const Segments& output = entry.outputs[outputIdx];

                int digit;
                for (digit=0; digit<knownPatterns.size(); ++digit) {
                    const Segments& knownPattern = knownPatterns[digit];
                    if ((output.size() == knownPattern.size()) && output.contains(knownPattern)) {
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
