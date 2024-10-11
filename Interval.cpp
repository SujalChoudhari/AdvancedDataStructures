#include <iostream>
#include <memory>
#include <algorithm>

struct IntervalNode {
public:
    int lowerBound;
    int higherBound;
    int maxValue;

    std::shared_ptr<IntervalNode> leftChild;
    std::shared_ptr<IntervalNode> rightChild;

public:
    IntervalNode(int lowerBound, int higherBound)
        : lowerBound(lowerBound), higherBound(higherBound), maxValue(higherBound) {
        leftChild = nullptr;
        rightChild = nullptr;
    }
};

struct IntervalTree {
public:
    std::shared_ptr<IntervalNode> rootNode;

public:
    IntervalTree() : rootNode(nullptr) {}

    void InsertInterval(int lowerBound, int upperBound) {
        std::shared_ptr<IntervalNode> newNode = std::make_shared<IntervalNode>(lowerBound, upperBound);
        rootNode = InsertNode(rootNode, newNode);
    }

    std::shared_ptr<IntervalNode> InsertNode(std::shared_ptr<IntervalNode> currRootNode, std::shared_ptr<IntervalNode> newNode) {
        if (currRootNode == nullptr) {
            return newNode;
        }

        if (newNode->lowerBound < currRootNode->lowerBound) {
            currRootNode->leftChild = InsertNode(currRootNode->leftChild, newNode);
        } else {
            currRootNode->rightChild = InsertNode(currRootNode->rightChild, newNode);
        }

        // Update the maxValue of the current node
        currRootNode->maxValue = std::max(currRootNode->maxValue, newNode->higherBound);
        return currRootNode;
    }

    // Function to search for overlapping intervals
    void SearchOverlapping(int lowerBound, int upperBound) {
        SearchOverlapping(rootNode, lowerBound, upperBound);
    }

    void SearchOverlapping(std::shared_ptr<IntervalNode> currNode, int lowerBound, int upperBound) {
        if (currNode == nullptr) return;

        // Check for overlap
        if (currNode->lowerBound <= upperBound && lowerBound <= currNode->higherBound) {
            std::cout << "Overlapping Interval: [" << currNode->lowerBound << ", " << currNode->higherBound << "]\n";
        }

        // Traverse left and right children if necessary
        if (currNode->leftChild != nullptr && currNode->leftChild->maxValue >= lowerBound) {
            SearchOverlapping(currNode->leftChild, lowerBound, upperBound);
        }
        SearchOverlapping(currNode->rightChild, lowerBound, upperBound);
    }
};

int main() {
    IntervalTree tree;
    
    tree.InsertInterval(15, 25);
    tree.InsertInterval(10, 30);
    tree.InsertInterval(5, 15);
    tree.InsertInterval(20, 35);
    
    std::cout << "Searching for overlaps with interval [14, 22]:\n";
    tree.SearchOverlapping(14, 22);

    std::cout << "Searching for overlaps with interval [30, 40]:\n";
    tree.SearchOverlapping(30, 40);

    return 0;
}
