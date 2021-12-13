#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

void day10() {
    std::ifstream inputStream("./data-day10.txt");

    int totalSyntaxErrorScore = 0;
    std::vector<uint64_t> autoCompleteScores;

    std::string line;
    while (getline(inputStream, line)) {
        std::stack<char> bracketStack;
        bool mismatch = false;
        for (size_t n=0; n<line.size(); ++n) {
            char c = line[n];
            switch (c) {
                case '(':
                case '[':
                case '{':
                case '<':
                    bracketStack.push(c);
                    break;
                case ')':
                    if (bracketStack.top() != '(') {
                        mismatch = true;
                        totalSyntaxErrorScore += 3;
                    }
                    bracketStack.pop();
                    break;
                case ']':
                    if (bracketStack.top() != '[') {
                        mismatch = true;
                        totalSyntaxErrorScore += 57;
                    }
                    bracketStack.pop();
                    break;
                case '}':
                    if (bracketStack.top() != '{') {
                        mismatch = true;
                        totalSyntaxErrorScore += 1197;
                    }
                    bracketStack.pop();
                    break;
                case '>':
                    if (bracketStack.top() != '<') {
                        mismatch = true;
                        totalSyntaxErrorScore += 25137;
                    }
                    bracketStack.pop();
                    break;
            }
            if (mismatch) {
                break;
            }
        }
        if (!mismatch && !bracketStack.empty()) {
            uint64_t score = 0;
            while (!bracketStack.empty()) {
                char c = bracketStack.top();
                switch (c) {
                    case '(':
                        score = (score * 5) + 1;
                        break;
                    case '[':
                        score = (score * 5) + 2;
                        break;
                    case '{':
                        score = (score * 5) + 3;
                        break;
                    case '<':
                        score = (score * 5) + 4;
                        break;
                }
                bracketStack.pop();
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
