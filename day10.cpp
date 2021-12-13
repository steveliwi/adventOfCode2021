#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <map>

void day10() {
    std::ifstream inputStream("./data-day10.txt");

    std::map<char, char> bracketMap = {
        {'(', ')'},
        {'[', ']'},
        {'{', '}'},
        {'<', '>'}
    };
    std::map<char, int> syntaxErrorPointMap = {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137}
    };
    std::map<char, int> autoCompletePointMap = {
        {'(', 1},
        {'[', 2},
        {'{', 3},
        {'<', 4}
    };

    int totalSyntaxErrorScore = 0;
    std::vector<uint64_t> autoCompleteScores;

    std::string line;
    while (getline(inputStream, line)) {
        std::stack<char> bracketStack;
        bool mismatch = false;
        for (size_t n=0; n<line.size() && !mismatch; ++n) {
            char c = line[n];
            if (bracketMap.find(c) != bracketMap.end()) {
                // open bracket
                bracketStack.push(c);
            } else {
                // expecting close bracket
                char openBracket = bracketStack.top();
                bracketStack.pop();
                if (c != bracketMap[openBracket]) {
                    totalSyntaxErrorScore += syntaxErrorPointMap[c];
                    mismatch = true;
                }
            }
        }
        if (!mismatch && !bracketStack.empty()) {
            uint64_t score = 0;
            while (!bracketStack.empty()) {
                char openBracket = bracketStack.top();
                bracketStack.pop();
                score = (score * 5) + autoCompletePointMap[openBracket];
            }
            autoCompleteScores.push_back(score);
        }
    }

    std::cout << "--- Day 10 ---" << std::endl;
    std::cout << "totalSyntaxErrorScore=" << totalSyntaxErrorScore << std::endl;

    std::cout << "--- Day 10 Part 2 ---" << std::endl;
    std::sort(autoCompleteScores.begin(), autoCompleteScores.end());
    int middleIdx = autoCompleteScores.size() / 2;
    std::cout << "middle autoComplete score=" << autoCompleteScores[middleIdx] << std::endl;
}
