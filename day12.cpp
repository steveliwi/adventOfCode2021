#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

class Node {
    public:
        Node(std::string _name) :
            name(_name) {
        }
    public:
        std::string name;
        std::vector<Node*> connectedNodes;
};
typedef std::map<std::string, std::unique_ptr<Node>> NameToNodeMap;

// Note that visitedNodes must be pass-by-value (not reference).
void traverseNode(Node* node, std::string pathInProgress, std::set<std::string> visitedNodes, bool allowDoubleVisit, std::vector<std::string>& completePaths) {
    if (!pathInProgress.empty()) {
        pathInProgress += ",";
    }
    pathInProgress += node->name;

    if (node->name == "end") {
        completePaths.push_back(pathInProgress);
        return;
    }

    if (std::islower(node->name[0])) {
        if (visitedNodes.find(node->name) == visitedNodes.end()) {
            // first time node is visited
            visitedNodes.insert(node->name);
        } else {
            // node has been visited before
            if (!allowDoubleVisit) {
                return;
            }
            allowDoubleVisit = false;
        }
    }

    for (size_t n=0; n<node->connectedNodes.size(); ++n) {
        Node* connectedNode = node->connectedNodes[n];
        if (connectedNode->name != "start") {
            traverseNode(connectedNode, pathInProgress, visitedNodes, allowDoubleVisit, completePaths);
        }
    }
}

void day12() {
    std::ifstream inputStream("./data-day12.txt");

    NameToNodeMap nameToNodeMap;

    std::string line;
    while (getline(inputStream, line)) {
        int pos = line.find('-');
        std::string name1 = line.substr(0, pos);
        std::string name2 = line.substr(pos+1);

        // Create/Get nodes
        NameToNodeMap::iterator itrNode1 = nameToNodeMap.insert(std::make_pair(name1, std::make_unique<Node>(name1))).first;
        NameToNodeMap::iterator itrNode2 = nameToNodeMap.insert(std::make_pair(name2, std::make_unique<Node>(name2))).first;

        // Connect the 2 nodes
        itrNode1->second->connectedNodes.push_back((itrNode2->second).get());
        itrNode2->second->connectedNodes.push_back((itrNode1->second).get());
    }

    Node* startNode = nameToNodeMap["start"].get();

    {
        std::string pathInProgress;
        std::set<std::string> visitedNodes;
        std::vector<std::string> completePaths;
        traverseNode(startNode, pathInProgress, visitedNodes, /*allowDoubleVisit*/false, completePaths);

        std::cout << "--- Day 12 ---" << std::endl;
        std::cout << "Number of path=" << completePaths.size() << std::endl;
    }

    {
        std::string pathInProgress;
        std::set<std::string> visitedNodes;
        std::vector<std::string> completePaths;
        traverseNode(startNode, pathInProgress, visitedNodes, /*allowDoubleVisit*/true, completePaths);

        std::cout << "--- Day 12 Part 2 ---" << std::endl;
        std::cout << "Number of path=" << completePaths.size() << std::endl;
    }
}
