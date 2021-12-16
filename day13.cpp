#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

class Point {
    public:
        uint32_t x;
        uint32_t y;
    public:
        Point(const std::string& s) {
            std::string::size_type pos = s.find(',');
            x = std::stoul(s.substr(0, pos));
            y = std::stoul(s.substr(pos + 1));
        }
        Point(uint32_t _x, uint32_t _y) : x(_x), y(_y) {
        }
        uint32_t hashcode() const {
            // assuming x and y are less than 16-bit
            return (x << 16) + y;
        }
        static bool cmp(const Point& lhs, const Point& rhs) {
            return lhs.hashcode() < rhs.hashcode();
        }
};
typedef std::set<Point, decltype(&Point::cmp)> SetOfPoints;

class Fold {
    public:
        char axis;
        uint32_t value;
    public:
        Fold(const std::string& s) {
            std::string::size_type pos = s.find('=');
            axis = s[pos - 1];
            value = std::stoul(s.substr(pos + 1));
        }
};

void processFold(const SetOfPoints& srcPoints, const Fold& fold, SetOfPoints& dstPoints) {
    for (SetOfPoints::iterator itr = srcPoints.begin(); itr != srcPoints.end(); ++itr) {
        const Point& pt = *itr;
        if (fold.axis == 'x') {
            if (pt.x > fold.value) {
                int x = (2 * fold.value) - pt.x;
                dstPoints.emplace(x, pt.y);
            } else {
                dstPoints.insert(pt);
            }
        } else {
            if (pt.y > fold.value) {
                int y = (2 * fold.value) - pt.y;
                dstPoints.emplace(pt.x, y);
            } else {
                dstPoints.insert(pt);
            }
        }
    }
}

void day13() {
    std::ifstream inputStream("./data-day13.txt");

    SetOfPoints points(Point::cmp);
    std::vector<Fold> folds;

    std::string line;
    while (getline(inputStream, line)) {
        if (line.empty()) {
            break;
        }
        points.emplace(line);
    }
    while (getline(inputStream, line)) {
        folds.emplace_back(line);
    }

    {
        SetOfPoints foldedPoints(Point::cmp);
        processFold(points, folds[0], foldedPoints);

        std::cout << "--- Day 12 ---" << std::endl;
        std::cout << "Number of points=" << foldedPoints.size() << std::endl;
    }

    {
        SetOfPoints srcPoints(Point::cmp);
        srcPoints = points;
        for (size_t foldIdx=0; foldIdx < folds.size(); ++foldIdx) {
            SetOfPoints dstPoints(Point::cmp);
            processFold(srcPoints, folds[foldIdx], dstPoints);
            srcPoints = dstPoints;
        }

        // cheated a bit here by hardcoding the display size
        std::vector<std::string> display(6, std::string(40, ' '));
        for (SetOfPoints::iterator itr = srcPoints.begin(); itr != srcPoints.end(); ++itr) {
            const Point& pt = *itr;
            display[pt.y][pt.x] = 'x';
        }

        std::cout << "--- Day 12 Part 2 ---" << std::endl;
        for (size_t n=0; n<display.size(); ++n) {
            std::cout << display[n] << std::endl;
        }
    }
}
