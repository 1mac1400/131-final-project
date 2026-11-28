
// DecisionTree.cpp
// CMPR131 Group Project #2 - Decision Tree Manager
// Class design plan, function headers, & Big Five by Ivan Segoviano
// Function logic by Kevin Ionescu
// Big Five, destroy, and copyHelper by Muhammad Jafri
// Used Gemini for help with the session/navigation loop and menus

#include "DecisionTree.h"
#include <algorithm> // Used for std::max in the heightHelper function

// ===================== Big Five =====================

DecisionTree::DecisionTree() : root(nullptr) {}

DecisionTree::~DecisionTree()
{
    destroy(root);
}

DecisionTree::DecisionTree(const DecisionTree& other)
{
    root = copyHelper(other.root);
}

DecisionTree& DecisionTree::operator=(const DecisionTree& other)
{
    if (this == &other)
    {
        return *this;
    }
    
    Node* newRoot = copyHelper(other.root);
    destroy(root);
    root = newRoot;
    return *this;
}

DecisionTree::DecisionTree(DecisionTree&& other)
{
    root = other.root;
    other.root = nullptr;
}

DecisionTree& DecisionTree::operator=(DecisionTree&& other)
{
    if (this == &other)
    {
        return *this;
    }
    
    destroy(root);
    root = other.root;
    other.root = nullptr;
    
    return *this;
}

// ===================== Building and editing =====================

// DONE: Find function
Node* DecisionTree::find(Node* current, const std::string& text) {
    
    if (current == nullptr) {

        return nullptr;

    }

    
    if (current->text == text) {

        return current;

    }

    // Searches the "yes" subtree before the "no" subtree
    Node* foundInYes = find(current->yes, text);

    if (foundInYes != nullptr) {

        return foundInYes;

    }

    return find(current->no, text);

}

// DONE: clear the old tree, then make a new root node with this text.
void DecisionTree::setRoot(const std::string& text) {

    clear(root);
    root = new Node(text);

}

// DONE: find the parent (find()), check that branch is empty, add a new node.
bool DecisionTree::addQuestion(const std::string& parentText, bool isYesBranch,
                               const std::string& question)
{
    
    Node* parent = find(root, parentText);

    if (parent == nullptr) {

        std::cout << "\nThere is no parent node. 0 nodes added.\n";
        return false;

    }

    if (isYesBranch == true) {

        if (parent->yes != nullptr) {
            
            std::cout << "\nThis branch has a \"yes\" node. 0 nodes added.\n";
            return false;

        }

        else {

            parent->yes = new Node(question);

        }

    }

    else {


        if (parent->no != nullptr) {

            std::cout <<"\nThis branch has a \"no\" node. 0 nodes added.\n";
            return false;

        }

        else {

            parent->no = new Node(question);

        }

    }

    return true;

}

// DONE: same steps as addQuestion.
bool DecisionTree::addLeaf(const std::string& parentText, bool isYesBranch,
                           const std::string& outcome)
{
    
    Node* parent = find(root, parentText);

    if (parent == nullptr) {

        std::cout << "\nThere is no parent node. 0 nodes added.\n";
        return false;

    }

    if (isYesBranch == true) {

        if (parent->yes != nullptr) {

            std::cout << "\nThis branch has a \"yes\" node. 0 nodes added.\n";
            return false;

        }

        else {

            parent->yes = new Node(outcome);

        }

    }

    else {


        if (parent->no != nullptr) {

            std::cout << "\nThis branch has a \"no\" node. 0 nodes added.\n";
            return false;

        }

        else {

            parent->no = new Node(outcome);

        }

    }

    return true;

}

// DONE: Find a leaf with this text, delete it, set its parent's pointer to nullptr.
//       If the leaf is the root, delete it and set root to nullptr (empty tree).
bool DecisionTree::removeLeaf(const std::string& leafText)
{
    
    return removeLeafHelper(root, leafText);

}

// DONE: Uses a helper to pass current by reference
bool DecisionTree::removeLeafHelper(Node*& current, const std::string& leafText) {

    // Check if the parent node exists
    if (current == nullptr) {

        return false;

    }

    // Check if the node has no children and if it matches the text specified in the function call
    if (current->yes == nullptr && current->no == nullptr && current->text == leafText) {

        delete current;
        current = nullptr;

        return true;

    }

    if (removeLeafHelper(current->yes, leafText) == true) {

        return true;

    }

    else if (removeLeafHelper(current->no, leafText) == true) {

        return true;

    }

    return false;

}

// DONE: Find the node with oldText and change its text.
bool DecisionTree::rename(const std::string& oldText, const std::string& newText)
{
    // Set a target
    Node* target = find(root, oldText);

    // Check if it exists
    if (target == nullptr) {

        std::cout << "There is no node here.";
        return false;

    }

    // Set the old text to the new desired text 
    else {

        target->text = newText;
        return true;

    }

}

// DONE: find the node and set its attribute.
bool DecisionTree::setAttribute(const std::string& nodeText,
                                const std::string& attribute)
{
    // Set a target
    Node* target = find(root, nodeText);

    // Check if it exists
    if (target == nullptr) {

        std::count << "There is no node here.";
        return false;

    }

    // Set the old attribute to the new desired attribute
    else {

        target->attribute = attribute;
        return true;

    }

}

// ===================== Viewing and running =====================

// DONE: preorder print, indented by depth (use displayHelper()).
void DecisionTree::display() const {

    if (root == nullptr) {

        std::cout << "\nThe tree is empty.\n";
        return;

    }

    displayHelper(root, 0, "");

}

void DecisionTree::displayHelper(const Node* node, int depth, const std::string& label) const {

    if (node == nullptr) {

        return;

    }

    // Indent based on current depth

    for (int i = 0; i < depth; ++i) {

        std::cout << "  ";

    }

    std::cout << label << node->text << "\n";

    displayHelper(node->yes, depth + 1, "[Yes] ");
    displayHelper(node->no, depth + 1, "[No] ");

}

// DONE: start at root, ask y/n (validate input), follow yes/no until a leaf.
//       If a question is missing its yes or no branch, print a message
//       and ask again instead of crashing.
void DecisionTree::runSession() const {

    if (root == nullptr) {

        std::cout << "The decision tree is empty.\n";
        return;

    }

    const Node* current = root;
    std::string input;

    // Loop while current node is not a leaf
    while (current->yes != nullptr || current->no != nullptr) {

        std::cout << current->text << " (y/n): ";
        std::cin >> input;

        // Validate input (accepts y/Y/yes/Yes and n/N/no/No)
        if (input == "y" || input == "Y" || input == "yes" || input == "Yes") {

            if (current->yes == nullptr) {

                std::cout << "No 'yes' branch available for this question. Please try again.\n";

            }

            else {

                current = current->yes;

            }

        }

        else if (input == "n" || input == "N" || input == "no" || input == "No") {

            if (current->no == nullptr) {

                std::cout << "No 'no' branch available for this question. Please try again.\n";

            }

            else {

                current = current->no;

            }

        }

        else {

            std::cout << "Invalid input. Please enter 'y' or 'n'.\n";

        }

    }

    std::cout << "Outcome: " << current->text << "\n";

}

// DONE: return heightHelper(root).
int DecisionTree::height() const
{
    return heightHelper(root);
}

int DecisionTree::heightHelper(const Node* node) const {

    if (node == nullptr) {

        return -1; // Standard convention: empty tree = -1, single root node = 0 ??

    }

    int yesHeight = heightHelper(node->yes);
    int noHeight = heightHelper(node->no);

    return 1 + std::max(yesHeight, noHeight);

}

// DONE: return countLeavesHelper(root).
int DecisionTree::countLeaves() const
{
    return countLeavesHelper(root);
}

int DecisionTree::countLeavesHelper(const Node* node) const {

    int totalLeaves = 0;

    if (node == nullptr) {

        return 0;

    }

    else if (node->yes == nullptr && node->no == nullptr) {

        return 1;

    }

    else {

        totalLeaves += countLeavesHelper(node->yes);
        totalLeaves += countLeavesHelper(node->no);

        return totalLeaves;

    }

}

bool DecisionTree::isEmpty() const
{
    return root == nullptr;
}

void DecisionTree::destroy(Node *&root)
{
    if (root == nullptr)
    {
        return;
    }
    destroy(root->yes);
    destroy(root->no);
    delete root;
    root = nullptr;
}

Node* DecisionTree::copyHelper(const Node *otherRoot)
{
    if (otherRoot == nullptr) //base case
    {
        return nullptr;
    }
    
    Node* newNode = new Node(otherRoot->text); 
    newNode->attribute = otherRoot->attribute;
    newNode->yes = copyHelper(otherRoot->yes);
    newNode->no = copyHelper(otherRoot->no);
    return newNode;
    
}

