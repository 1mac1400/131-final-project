
// DecisionTree.cpp
// CMPR131 Group Project #2 - Decision Tree Manager
// Class design plan by Ivan Segoviano

#include "DecisionTree.h"

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

// TODO: clear the old tree, then make a new root node with this text.
void DecisionTree::setRoot(const std::string& text) {}

// TODO: find the parent (find()), check that branch is empty, add a new node.
bool DecisionTree::addQuestion(const std::string& parentText, bool isYesBranch,
                               const std::string& question)
{
    return false;
}

// TODO: same steps as addQuestion.
bool DecisionTree::addLeaf(const std::string& parentText, bool isYesBranch,
                           const std::string& outcome)
{
    return false;
}

// TODO: find a leaf with this text, delete it, set its parent's pointer to nullptr.
//       If the leaf is the root, delete it and set root to nullptr (empty tree).
bool DecisionTree::removeLeaf(const std::string& leafText)
{
    return false;
}

// TODO: find the node with oldText and change its text.
bool DecisionTree::rename(const std::string& oldText, const std::string& newText)
{
    return false;
}

// TODO: find the node and set its attribute.
bool DecisionTree::setAttribute(const std::string& nodeText,
                                const std::string& attribute)
{
    return false;
}

// ===================== Viewing and running =====================

// TODO: preorder print, indented by depth (use displayHelper()).
void DecisionTree::display() const {}

// TODO: start at root, ask y/n (validate input), follow yes/no until a leaf.
//       If a question is missing its yes or no branch, print a message
//       and ask again instead of crashing.
void DecisionTree::runSession() const {}

// TODO: return heightHelper(root).
int DecisionTree::height() const
{
    return -1;
}

// TODO: return countLeavesHelper(root).
int DecisionTree::countLeaves() const
{
    return 0;
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
