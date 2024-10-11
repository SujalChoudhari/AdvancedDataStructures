#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#define ORDER_INTERNAL 3 // Maximum keys in internal node
#define ORDER_LEAF 4     // Maximum keys in leaf node

class BPlusNode
{
public:
    std::vector<int> keys;                            // For internal nodes or leaf nodes
    std::vector<std::shared_ptr<BPlusNode>> children; // For internal nodes
    std::vector<int> dataPointers;                    // Only for leaf nodes
    std::shared_ptr<BPlusNode> next;                  // Pointer to the next leaf node

    bool isLeaf() const
    {
        return children.empty(); // A node is a leaf if it has no children
    }
};

class BPlusTree
{
public:
    std::shared_ptr<BPlusNode> root;

    BPlusTree() : root(nullptr) {}

    void insert(int key)
    {
        if (!root)
        {
            // Create the root if it doesn't exist
            root = std::make_shared<BPlusNode>();
            root->keys.push_back(key);
            return;
        }
        // Insert into the tree
        auto newKey = insertRec(root, key);
        if (newKey)
        {
            // If the root was split, create a new root
            auto newRoot = std::make_shared<BPlusNode>();
            newRoot->keys.push_back(*newKey);
            newRoot->children.push_back(root);
            newRoot->children.push_back(newKey->second);
            root = newRoot;
        }
    }

private:
    std::pair<std::optional<int>, std::shared_ptr<BPlusNode>> insertRec(std::shared_ptr<BPlusNode> node, int key)
    {
        if (node->isLeaf())
        {
            // Insert key in the leaf node
            return insertInLeaf(node, key);
        }
        else
        {
            // Find the child to recurse into
            int i = 0;
            while (i < node->keys.size() && key > node->keys[i])
            {
                i++;
            }
            auto newKey = insertRec(node->children[i], key);
            if (newKey.first)
            {
                // If the child was split, add the new key to the internal node
                node->keys.insert(node->keys.begin() + i, *newKey.first);
                node->children.insert(node->children.begin() + i + 1, newKey.second);
            }
            // Check for overflow
            if (node->keys.size() > ORDER_INTERNAL - 1)
            {
                return splitInternal(node);
            }
        }
        return {std::nullopt, nullptr};
    }

    std::pair<std::optional<int>, std::shared_ptr<BPlusNode>> insertInLeaf(std::shared_ptr<BPlusNode> leaf, int key)
    {
        // Insert the key in sorted order
        auto it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        if (it != leaf->keys.end() && *it == key)
        {
            // Key already exists; no duplicates allowed
            return {std::nullopt, nullptr};
        }
        leaf->keys.insert(it, key);
        if (leaf->keys.size() > ORDER_LEAF - 1)
        {
            // Overflow: need to split the leaf
            return splitLeaf(leaf);
        }
        return {std::nullopt, nullptr};
    }

    std::pair<std::optional<int>, std::shared_ptr<BPlusNode>> splitLeaf(std::shared_ptr<BPlusNode> leaf)
    {
        auto newLeaf = std::make_shared<BPlusNode>();
        int mid = leaf->keys.size() / 2;

        newLeaf->keys.assign(leaf->keys.begin() + mid, leaf->keys.end());
        leaf->keys.erase(leaf->keys.begin() + mid, leaf->keys.end());

        // Update next pointer for linked list traversal of leaf nodes
        newLeaf->next = leaf->next;
        leaf->next = newLeaf;

        return {newLeaf->keys.front(), newLeaf}; // Return the first key of the new leaf
    }

    std::pair<std::optional<int>, std::shared_ptr<BPlusNode>> splitInternal(std::shared_ptr<BPlusNode> internal)
    {
        auto newInternal = std::make_shared<BPlusNode>();
        int mid = internal->keys.size() / 2;

        newInternal->keys.assign(internal->keys.begin() + mid + 1, internal->keys.end());
        newInternal->children.assign(internal->children.begin() + mid + 1, internal->children.end());

        internal->keys.erase(internal->keys.begin() + mid, internal->keys.end());
        internal->children.erase(internal->children.begin() + mid + 1, internal->children.end());

        return {internal->keys[mid], newInternal}; // Return the middle key and the new node
    }
};

int main()
{
    BPlusTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(12);
    tree.insert(30);

    // Additional testing code can go here

    return 0;
}
