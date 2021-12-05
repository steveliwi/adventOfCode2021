#include "utils.h"

bool getLineAsInt(std::ifstream& stream, int& value, int base) {
    std::string line;
    if (!getline(stream, line)) {
        return false;
    }
    value = std::stoi(line, 0, base);
    return true;
}
