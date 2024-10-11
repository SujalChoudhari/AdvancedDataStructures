#include <iostream>
#include <vector>
#include <queue>
#include <iomanip> // For std::setw

using namespace std;

class BTreeNode {
public:
    vector<int> keys;
    vector<BTreeNode*> children;
    int degree;
    bool isLeaf;

    BTreeNode(int _degree, bool _isLeaf) {
        degree = _degree;
        isLeaf = _isLeaf;
    }

    void traverse() {
        for (int i = 0; i < keys.size(); i++) {
            if (!isLeaf)
                children[i]->traverse();
            cout << " " << keys[i];
        }
        if (!isLeaf)
            children[keys.size()]->traverse();
    }

    void insertNonFull(int key) {
        int i = keys.size() - 1;
        if (isLeaf) {
            keys.push_back(0);
            while (i >= 0 && keys[i] > key) {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = key;
        } else {
            while (i >= 0 && keys[i] > key)
                i--;
            if (children[i + 1]->keys.size() == 2 * degree - 1) {
                splitChild(i + 1, children[i + 1]);
                if (keys[i + 1] < key)
                    i++;
            }
            children[i + 1]->insertNonFull(key);
        }
    }

    void splitChild(int i, BTreeNode *y) {
        BTreeNode *z = new BTreeNode(y->degree, y->isLeaf);
        for (int j = 0; j < degree - 1; j++)
            z->keys.push_back(y->keys[j + degree]);
        if (!y->isLeaf) {
            for (int j = 0; j < degree; j++)
                z->children.push_back(y->children[j + degree]);
        }
        keys.insert(keys.begin() + i, y->keys[degree - 1]);
        children.insert(children.begin() + i + 1, z);
        y->keys.resize(degree - 1);
        if (!y->isLeaf)
            y->children.resize(degree);
    }

    void remove(int key, BTreeNode *root) {
        int idx = 0;
        while (idx < keys.size() && keys[idx] < key)
            idx++;

        if (idx < keys.size() && keys[idx] == key) {
            if (isLeaf) {
                keys.erase(keys.begin() + idx);
            } else {
                if (children[idx]->keys.size() >= degree) {
                    int predecessor = getPredecessor(idx);
                    keys[idx] = predecessor;
                    children[idx]->remove(predecessor, root);
                } else if (children[idx + 1]->keys.size() >= degree) {
                    int successor = getSuccessor(idx);
                    keys[idx] = successor;
                    children[idx + 1]->remove(successor, root);
                } else {
                    merge(idx);
                    children[idx]->remove(key, root);
                }
            }
        } else {
            if (isLeaf) {
                cout << "The key " << key << " is not in the tree.\n";
                return;
            }
            bool shouldMerge = (idx == keys.size());
            if (children[idx]->keys.size() < degree)
                fill(idx);
            if (shouldMerge && idx > keys.size())
                children[idx - 1]->remove(key, root);
            else
                children[idx]->remove(key, root);
        }
    }

    int getPredecessor(int idx) {
        BTreeNode *current = children[idx];
        while (!current->isLeaf)
            current = current->children[current->keys.size()];
        return current->keys.back();
    }

    int getSuccessor(int idx) {
        BTreeNode *current = children[idx + 1];
        while (!current->isLeaf)
            current = current->children[0];
        return current->keys.front();
    }

    void merge(int idx) {
        BTreeNode *child = children[idx];
        BTreeNode *sibling = children[idx + 1];
        child->keys.push_back(keys[idx]);
        child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());
        if (!child->isLeaf) {
            child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
        }
        keys.erase(keys.begin() + idx);
        children.erase(children.begin() + idx + 1);
        delete sibling;
    }

    void fill(int idx) {
        if (idx != 0 && children[idx - 1]->keys.size() >= degree)
            borrowFromPrev(idx);
        else if (idx != keys.size() && children[idx + 1]->keys.size() >= degree)
            borrowFromNext(idx);
        else {
            if (idx != keys.size())
                merge(idx);
            else
                merge(idx - 1);
        }
    }

    void borrowFromPrev(int idx) {
        BTreeNode *child = children[idx];
        BTreeNode *sibling = children[idx - 1];
        child->keys.insert(child->keys.begin(), keys[idx - 1]);
        keys[idx - 1] = sibling->keys.back();
        sibling->keys.pop_back();
        if (!child->isLeaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }
    }

    void borrowFromNext(int idx) {
        BTreeNode *child = children[idx];
        BTreeNode *sibling = children[idx + 1];
        child->keys.push_back(keys[idx]);
        keys[idx] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
        if (!child->isLeaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }
    }

    // Level-wise print
    void printLevelOrder() {
        if (!this) return;

        queue<BTreeNode*> q;
        q.push(this);

        while (!q.empty()) {
            int nodeCount = q.size();
            while (nodeCount > 0) {
                BTreeNode* node = q.front();
                q.pop();

                // Print keys of the current node
                for (int key : node->keys) {
                    cout << key << " ";
                }

                // Add child nodes to the queue
                if (!node->isLeaf) {
                    for (BTreeNode* child : node->children) {
                        q.push(child);
                    }
                }
                nodeCount--;
            }
            cout << endl; // New line for the next level
        }
    }
};

class BTree {
public:
    BTreeNode *root;
    int degree;

    BTree(int _degree) {
        root = nullptr;
        degree = _degree;
    }

    void traverse() {
        if (root != nullptr) {
            root->traverse();
        }
    }

    void insert(int key) {
        if (root == nullptr) {
            root = new BTreeNode(degree, true);
            root->keys.push_back(key);
        } else {
            if (root->keys.size() == 2 * degree - 1) {
                BTreeNode *s = new BTreeNode(degree, false);
                s->children.push_back(root);
                s->splitChild(0, root);
                int i = 0;
                if (s->keys[0] < key)
                    i++;
                s->children[i]->insertNonFull(key);
                root = s;
            } else {
                root->insertNonFull(key);
            }
        }
    }

    void remove(int key) {
        if (!root) {
            cout << "The tree is empty.\n";
            return;
        }
        root->remove(key, root);
        if (root->keys.empty()) {
            BTreeNode *tmp = root;
            root = (root->isLeaf) ? nullptr : root->children[0];
            delete tmp;
        }
    }

    void printLevelOrder() {
        if (root != nullptr) {
            root->printLevelOrder();
        } else {
            cout << "The tree is empty.\n";
        }
    }
};

void displayMenu() {
    cout << "B-Tree Operations Menu:\n";
    cout << "1. Insert\n";
    cout << "2. Traverse\n";
    cout << "3. Delete\n";
    cout << "4. Print Level Order\n";
    cout << "5. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    int degree;
    cout << "Enter the degree of B-Tree: ";
    cin >> degree;

    BTree bTree(degree);
    int choice, key;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter key to insert: ";
                cin >> key;
                bTree.insert(key);
                break;
            case 2:
                cout << "B-Tree traversal:";
                bTree.traverse();
                cout << endl;
                break;
            case 3:
                cout << "Enter key to delete: ";
                cin >> key;
                bTree.remove(key);
                break;
            case 4:
                cout << "B-Tree level order:\n";
                bTree.printLevelOrder();
                break;
            case 5:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    }

    return 0;
}
