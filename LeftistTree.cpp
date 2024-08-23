#include <iostream>
#include <memory>
#include <queue>
#include <vector>


const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RESET = "\033[0m";

struct LeftNode
{
    int value;
    std::shared_ptr<LeftNode> left;
    std::shared_ptr<LeftNode> right;

    LeftNode(int v) : value(v), left(nullptr), right(nullptr) {}
};

void PrintTree(const std::shared_ptr<LeftNode> &root, int level = 0, std::string prefix = "")
{
    if (root == nullptr) {
        std::cout << std::string(level * 4, ' ') << prefix << RED << "null" << RESET << std::endl;
        return;
    }
    
    std::cout << std::string(level * 4, ' ') << prefix << GREEN << root->value << RESET << std::endl;
    PrintTree(root->left, level + 1, "L->");
    PrintTree(root->right, level + 1, "R->");
}

std::shared_ptr<LeftNode> MergeTrees(std::shared_ptr<LeftNode> t1, std::shared_ptr<LeftNode> t2)
{
    std::cout << YELLOW << "Merging trees:" << RESET << std::endl;
    std::cout << YELLOW << "Tree 1 before merge:" << RESET << std::endl;
    PrintTree(t1);

    std::cout << YELLOW << "Tree 2 before merge:" << RESET << std::endl;
    PrintTree(t2);

    if (!t1) {
        std::cout << RED << "Tree 1 is null, returning Tree 2." << RESET << std::endl;
        return t2;
    }
    if (!t2) {
        std::cout << RED << "Tree 2 is null, returning Tree 1." << RESET << std::endl;
        return t1;
    }

    if (t1->value > t2->value) {
        std::cout << RED << "Swapping trees because Tree 1 root value (" << t1->value << ") > Tree 2 root value (" << t2->value << ")" << RESET << std::endl;
        std::swap(t1, t2);
    }

    std::cout << YELLOW << "Recursively merging right subtree of Tree 1 with Tree 2." << RESET << std::endl;
    t1->right = MergeTrees(t1->right, t2);

    if (!t1->left || (t1->right && t1->left->value > t1->right->value)) {
        std::cout << YELLOW << "Swapping left and right children of Tree 1." << RESET << std::endl;
        std::swap(t1->left, t1->right);
    }

    std::cout << YELLOW << "Tree 1 after merge:" << RESET << std::endl;
    PrintTree(t1);

    return t1;
}

std::shared_ptr<LeftNode> InsertNode(std::shared_ptr<LeftNode> root, int value)
{
    std::cout << GREEN << "Inserting value: " << value << RESET << std::endl;
    std::shared_ptr<LeftNode> newNode = std::make_shared<LeftNode>(value);
    root = MergeTrees(root, newNode);
    return root;
}

void DisplayTree(const std::shared_ptr<LeftNode> &root)
{
    if (root == nullptr) {
        std::cout << RED << "Tree is empty." << RESET << std::endl;
        return;
    }

    std::queue<std::shared_ptr<LeftNode>> q;
    q.push(root);

    std::cout << YELLOW << "Tree contents (level order):" << RESET << std::endl;

    while (!q.empty())
    {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i)
        {
            auto node = q.front();
            q.pop();
            std::cout << GREEN << node->value << RESET << " ";

            if (node->left != nullptr) {
                q.push(node->left);
            }

            if (node->right != nullptr) {
                q.push(node->right);
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::shared_ptr<LeftNode> root = nullptr;
    int choice, value;

    while (true)
    {
        std::cout << YELLOW << "Menu:" << RESET << std::endl;
        std::cout << "1. Insert Node" << std::endl;
        std::cout << "2. Display Tree" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cout << "\n\n\n\n";
        switch (choice)
        {
        case 1:
            std::cout << YELLOW << "Enter value to insert: " << RESET;
            std::cin >> value;
            root = InsertNode(root, value);
            break;

        case 2:
            DisplayTree(root);
            break;

        case 3:
            return 0;

        default:
            std::cout << RED << "Invalid choice. Please try again." << RESET << std::endl;
        }
    }

    return 0;
}
