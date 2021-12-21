#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class BitReader {
    public:
        std::vector<uint8_t> data;
    private:
        uint32_t nibble_pos;
        uint32_t bit_pos;
    public:
        BitReader() : nibble_pos(0), bit_pos(0) {
        }
        uint64_t numBitsRead() const {
            return (nibble_pos * 4) + bit_pos;
        }
        uint8_t nextBit() {
            // bit 0 is MSB
            uint8_t val = ((data[nibble_pos] >> (3 - bit_pos)) & 1);
            if (bit_pos == 3) {
                ++nibble_pos;
                bit_pos = 0;
            } else {
                ++bit_pos;
            }
            return val;
        }
        uint64_t nextBits(size_t numBits) {
            uint64_t val = 0;
            for (size_t n=0; n<numBits; ++n) {
                uint8_t b = nextBit();
                val = (val << 1) | b;
            }
            return val;
        }
};

uint64_t processPacket(BitReader& br, uint64_t& sumOfVersions) {
    uint64_t version = br.nextBits(3);
    uint64_t typeId = br.nextBits(3);

    sumOfVersions += version;

    uint64_t packetValue;
    if (typeId == 4) {
        // literal
        packetValue = 0;
        for (; ;) {
            uint64_t number = br.nextBits(5);
            uint64_t prefix = ((number >> 4) & 1); // bit 5
            uint64_t value = number & 0xF;
            packetValue = (packetValue << 4) | value;
            if (prefix == 0) {
                break;
            }
        }
    }
    else {
        // operator
        std::vector<uint64_t> packetValues;
        uint64_t lengthType = br.nextBit();
        if (lengthType == 0) {
            uint64_t numBitsInSubPackets = br.nextBits(15);
            uint64_t start = br.numBitsRead();
            while ((br.numBitsRead() - start) < numBitsInSubPackets) {
                uint64_t v = processPacket(br, sumOfVersions);
                packetValues.push_back(v);
            }
        }
        else if (lengthType == 1) {
            uint64_t numSubPackets = br.nextBits(11);
            for (int n=0; n<numSubPackets; ++n) {
                uint64_t v = processPacket(br, sumOfVersions);
                packetValues.push_back(v);
            }
        }

        switch (typeId) {
            case 0: // sum
                packetValue = 0;
                for (auto& v : packetValues) {
                    packetValue += v;
                }
                break;
            case 1: // product
                packetValue = 1;
                for (auto& v : packetValues) {
                    packetValue *= v;
                }
                break;
            case 2: // min
                packetValue = packetValues[0];
                for (size_t n=1; n<packetValues.size(); ++n) {
                    uint64_t v = packetValues[n];
                    if (packetValue > v) {
                        packetValue = v;
                    }
                }
                break;
            case 3: // max
                packetValue = packetValues[0];
                for (size_t n=1; n<packetValues.size(); ++n) {
                    uint64_t v = packetValues[n];
                    if (packetValue < v) {
                        packetValue = v;
                    }
                }
                break;
            case 5: // greater than
                packetValue = (packetValues[0] > packetValues[1]) ? 1 : 0;
                break;
            case 6: // less than
                packetValue = (packetValues[0] < packetValues[1]) ? 1 : 0;
                break;
            case 7: // equal to
                packetValue = (packetValues[0] == packetValues[1]) ? 1 : 0;
                break;
        }
    }
    return packetValue;
}

void day16() {
    std::ifstream inputStream("./data-day16.txt");

    std::string line;
    getline(inputStream, line);

    BitReader br;
    std::vector<uint8_t>& data = br.data;
    for (char c : line) {
        uint8_t val = ((c >= '0') && (c <='9')) ? (c - '0') : (10 + (c - 'A'));
        data.push_back(val);
    }

    uint64_t sumOfVersions = 0;
    uint64_t packetValue = processPacket(br, sumOfVersions);

    std::cout << "--- Day 16 ---" << std::endl;
    std::cout << "sumOfVersions=" << sumOfVersions << std::endl;

    std::cout << "--- Day 16 Part 2 ---" << std::endl;
    std::cout << "packetValue=" << packetValue << std::endl;
}
