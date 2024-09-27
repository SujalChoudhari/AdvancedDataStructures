#include <iostream>
#include <vector>
#include <cstdlib>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const int M = 2;

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";

struct Node {
    std::vector<int> keys;
    std::vector<Node*> children;
    bool leaf;

    Node(bool isLeaf = true) : leaf(isLeaf) {}
};

class BTree {
private:
    Node* root;

    Node* search(Node* x, int k) {
        int i = 0;
        while (i < x->keys.size() && k > x->keys[i]) {
            i++;
        }
        if (i < x->keys.size() && k == x->keys[i]) {
            return x;
        }
        if (x->leaf) {
            return nullptr;
        }
        return search(x->children[i], k);
    }

    void splitChild(Node* x, int i) {
        Node* y = x->children[i];
        Node* z = new Node(y->leaf);
        x->children.insert(x->children.begin() + i + 1, z);
        x->keys.insert(x->keys.begin() + i, y->keys[M/2]);
        z->keys.assign(y->keys.begin() + M/2 + 1, y->keys.end());
        y->keys.erase(y->keys.begin() + M/2, y->keys.end());
        if (!y->leaf) {
            z->children.assign(y->children.begin() + M/2 + 1, y->children.end());
            y->children.erase(y->children.begin() + M/2 + 1, y->children.end());
        }
    }

    void insertNonFull(Node* x, int k) {
        int i = x->keys.size() - 1;
        if (x->leaf) {
            x->keys.push_back(0);
            while (i >= 0 && k < x->keys[i]) {
                x->keys[i+1] = x->keys[i];
                i--;
            }
            x->keys[i+1] = k;
        } else {
            while (i >= 0 && k < x->keys[i]) {
                i--;
            }
            i++;
            if (x->children[i]->keys.size() == 2*M - 1) {
                splitChild(x, i);
                if (k > x->keys[i]) {
                    i++;
                }
            }
            insertNonFull(x->children[i], k);
        }
    }

    void printTree(Node* x, int level = 0) {
        if (x) {
            for (int i = x->keys.size() - 1; i >= 0; i--) {
                if (!x->leaf) {
                    printTree(x->children[i + 1], level + 1);
                }
                for (int j = 0; j < level; j++) std::cout << "    ";
                std::cout << YELLOW << x->keys[i] << RESET << std::endl;
            }
            if (!x->leaf) {
                printTree(x->children[0], level + 1);
            }
        }
    }

public:
    BTree() {
        root = new Node();
    }

    Node* search(int k) {
        return search(root, k);
    }

    void insert(int k) {
        Node* r = root;
        if (r->keys.size() == 2*M - 1) {
            Node* s = new Node(false);
            s->children.push_back(r);
            root = s;
            splitChild(s, 0);
            insertNonFull(s, k);
        } else {
            insertNonFull(r, k);
        }
    }

    void printTree() {
        printTree(root);
    }
};

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    BTree tree;
    int choice, key;

    while (true) {
        clearScreen();
        std::cout << BLUE << "B-Tree Operations Menu" << RESET << std::endl;
        std::cout << "1. Insert a key" << std::endl;
        std::cout << "2. Search for a key" << std::endl;
        std::cout << "3. Print the tree" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter the key to insert: ";
                std::cin >> key;
                tree.insert(key);
                std::cout << GREEN << "Key " << key << " inserted successfully." << RESET << std::endl;
                break;
            case 2:
                std::cout << "Enter the key to search: ";
                std::cin >> key;
                if (tree.search(key)) {
                    std::cout << GREEN << "Key " << key << " found in the tree." << RESET << std::endl;
                } else {
                    std::cout << RED << "Key " << key << " not found in the tree." << RESET << std::endl;
                }
                break;
            case 3:
                std::cout << YELLOW << "B-Tree Structure:" << RESET << std::endl;
                tree.printTree();
                break;
            case 4:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << RED << "Invalid choice. Please try again." << RESET << std::endl;
        }
        pauseScreen();
    }

    return 0;
}