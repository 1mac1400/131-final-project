
// DecisionTree.h
// CMPR131 Group Project #2 - Decision Tree Manager
// CLASS DESIGN PLAN by Ivan Segoviano

#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <string>

// One node of the tree.
struct Node
{
    std::string text;       // the question or the final outcome
    std::string attribute;  // optional custom tag (empty if none)
    Node* yes;              // left child (the "yes" answer)
    Node* no;               // right child (the "no" answer)

    // New nodes start as leaves with no attribute.
    explicit Node(const std::string& nodeText)
        : text(nodeText), attribute(""), yes(nullptr), no(nullptr) {}

    // A leaf has no children.
    bool isLeaf() const { return yes == nullptr && no == nullptr; }
};

class DecisionTree
{
public:
    // ---------- Big Five ----------
    DecisionTree();                                          // empty tree
    ~DecisionTree();                                         // frees all nodes
    DecisionTree(const DecisionTree& other);                 // copy constructor
    DecisionTree& operator=(const DecisionTree& other);      // copy assignment
    DecisionTree(DecisionTree&& other);                      // move constructor
    DecisionTree& operator=(DecisionTree&& other);           // move assignment

    // ---------- Building and editing ----------
    // Starts a new tree with this root (clears any existing tree).
    void setRoot(const std::string& text);

    // Adds a question under the node with text parentText.
    // isYesBranch: true = left (yes), false = right (no).
    // Returns false if the parent is not found or that branch is taken.
    bool addQuestion(const std::string& parentText, bool isYesBranch,
                     const std::string& question);

    // Same as addQuestion, but adds an outcome (leaf).
    bool addLeaf(const std::string& parentText, bool isYesBranch,
                 const std::string& outcome);

    // Removes the leaf with this text. Returns false if no such leaf exists.
    // If the leaf is the root (the only node), the tree becomes empty.
    bool removeLeaf(const std::string& leafText);

    // Changes a node's text. Returns false if oldText is not found.
    bool rename(const std::string& oldText, const std::string& newText);

    // Gives a node a custom attribute. Returns false if the node is not found.
    bool setAttribute(const std::string& nodeText, const std::string& attribute);

    // ---------- Viewing and running ----------
    void display() const;     // prints the whole tree, indented by depth
    void runSession() const;  // asks yes/no questions until an outcome is reached
    int height() const;       // -1 for an empty tree, 0 for a single node
    int countLeaves() const;  // number of outcomes in the tree
    bool isEmpty() const;     // true if there is no root

private:
    Node* root;

    // Recursive helpers (the public functions call these, starting at root)
    Node* find(Node* node, const std::string& text);  // not const: callers may edit the node
    void displayHelper(const Node* node, int depth, const std::string& label) const;
    int heightHelper(const Node* node) const;
    int countLeavesHelper(const Node* node) const;
    Node* copyHelper (const Node* otherRoot); //copy constructor and overloaded assignment operator helper
    void destroy(Node*& root); //helper for destructor, uses postorder
};

#endif
