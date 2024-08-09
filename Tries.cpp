#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <vector>

class StandardTriesNode
{
private:
    char m_Character;
    int m_WordEnd;
    std::shared_ptr<StandardTriesNode> m_Children[26] = {nullptr};

public:
    StandardTriesNode(char character) : m_Character(character), m_WordEnd(0) {}

    void AddChild(char character)
    {
        int index = character - 'A';
        if (index < 0 || index >= 26)
            return;

        if (m_Children[index] == nullptr)
        {
            m_Children[index] = std::make_shared<StandardTriesNode>(character);
        }
    }

    std::shared_ptr<StandardTriesNode> GetChild(char character)
    {
        int index = character - 'A';
        if (index < 0 || index >= 26)
            return nullptr;
        return m_Children[index];
    }

    void IncrementWordEnd()
    {
        m_WordEnd++;
    }

    void DecrementWordEnd()
    {
        m_WordEnd--;
    }

    int GetWordEndCount() const
    {
        return m_WordEnd;
    }

    bool HasChildren() const
    {
        for (const auto &child : m_Children)
        {
            if (child != nullptr)
                return true;
        }
        return false;
    }

    static bool Search(std::shared_ptr<StandardTriesNode> rootNode, const std::string &word)
    {
        std::shared_ptr<StandardTriesNode> currentNode = rootNode;

        for (char character : word)
        {
            currentNode = currentNode->GetChild(character);
            if (currentNode == nullptr)
                return false;
        }

        return currentNode->GetWordEndCount() > 0;
    }

    static void Insert(std::shared_ptr<StandardTriesNode> rootNode, const std::string &word)
    {
        std::shared_ptr<StandardTriesNode> currentNode = rootNode;

        for (char character : word)
        {
            currentNode->AddChild(character);
            currentNode = currentNode->GetChild(character);
        }

        currentNode->IncrementWordEnd();
    }

    static bool Delete(std::shared_ptr<StandardTriesNode> rootNode, const std::string &word)
    {
        std::shared_ptr<StandardTriesNode> currentNode = rootNode;

        for (char character : word)
        {
            currentNode = currentNode->GetChild(character);
            if (currentNode == nullptr)
                return false;
        }

        currentNode->DecrementWordEnd();
        return true;
    }
};

void DisplayMenu()
{
    std::cout << "Trie Menu:\n";
    std::cout << "1. Insert a word\n";
    std::cout << "2. Search for a word\n";
    std::cout << "3. Delete a word\n";
    std::cout << "4. Display all words\n";
    std::cout << "5. Exit\n";
    std::cout << "Choose an option: ";
}

void DisplayAllWords(std::shared_ptr<StandardTriesNode> rootNode, std::string currentWord, std::vector<std::string> &allWords)
{
    if (rootNode->GetWordEndCount() > 0)
    {
        allWords.push_back(currentWord);
    }

    for (char character = 'A'; character <= 'Z'; ++character)
    {
        std::shared_ptr<StandardTriesNode> child = rootNode->GetChild(character);
        if (child != nullptr)
        {
            DisplayAllWords(child, currentWord + character, allWords);
        }
    }
}

int main()
{
    auto rootNode = std::make_shared<StandardTriesNode>('\0');
    std::vector<std::string> basicWords = {"BAT", "BATMAN", "BATCAVE", "BANANA", "BALI", "BALL", "BASE"};

    for (const auto &word : basicWords)
    {
        StandardTriesNode::Insert(rootNode, word);
    }

    std::string word;

    while (true)
    {
        std::cin.clear();
        int choice;
        DisplayMenu();
        std::cin >> choice;
        std::vector<std::string> allWords;

        switch (choice)
        {
        case 1:
            std::cout << "Enter a word to insert: ";
            std::cin >> word;
            // Ensure the word is in uppercase
            std::transform(word.begin(), word.end(), word.begin(), ::toupper);
            StandardTriesNode::Insert(rootNode, word);
            std::cout << "Word inserted successfully!\n";
            break;
        case 2:
            std::cout << "Enter a word to search: ";
            std::cin >> word;
            std::transform(word.begin(), word.end(), word.begin(), ::toupper);
            if (StandardTriesNode::Search(rootNode, word))
            {
                std::cout << "Word found in the trie.\n";
            }
            else
            {
                std::cout << "Word not found in the trie.\n";
            }
            break;
        case 3:
            std::cout << "Enter a word to delete: ";
            std::cin >> word;
            std::transform(word.begin(), word.end(), word.begin(), ::toupper);
            if (StandardTriesNode::Delete(rootNode, word))
            {
                std::cout << "Word deleted successfully!\n";
            }
            else
            {
                std::cout << "Word not found in the trie.\n";
            }
            break;
        case 4:
            DisplayAllWords(rootNode, "", allWords);
            std::cout << "All words in the trie:\n";
            for (const auto &word : allWords)
            {
                std::cout << word << "\t";
            }
            std::cout << "\n";
            break;
        case 5:
            std::cout << "Exiting...\n";
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            return 0;
        }
    }

    return 0;
}