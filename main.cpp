// CMPR131 Group Project #2 - Decision Tree Manager
// Testing driver - Shahsar Shamim

#include "DecisionTree.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace
{
int passed = 0;
int failed = 0;

void check(bool condition, const std::string& description)
{
    std::cout << (condition ? "[PASS] " : "[FAIL] ") << description << '\n';
    if (condition)
        ++passed;
    else
        ++failed;
}

std::string captureDisplay(const DecisionTree& tree)
{
    std::ostringstream output;
    std::streambuf* previousOutput = std::cout.rdbuf(output.rdbuf());
    tree.display();
    std::cout.rdbuf(previousOutput);
    return output.str();
}

std::string captureSession(const DecisionTree& tree, const std::string& answers)
{
    std::istringstream input(answers);
    std::ostringstream output;
    std::streambuf* previousInput = std::cin.rdbuf(input.rdbuf());
    std::streambuf* previousOutput = std::cout.rdbuf(output.rdbuf());
    tree.runSession();
    std::cout.rdbuf(previousOutput);
    std::cin.rdbuf(previousInput);
    return output.str();
}

bool contains(const std::string& output, const std::string& text)
{
    return output.find(text) != std::string::npos;
}

void testEditingAndTraversal()
{
    DecisionTree tree;
    check(tree.isEmpty(), "A new tree is empty");
    check(tree.height() == -1, "An empty tree has height -1");
    check(tree.countLeaves() == 0, "An empty tree has no leaves");

    tree.setRoot("Is it raining?");
    check(!tree.isEmpty(), "Setting the root makes the tree nonempty");
    check(tree.height() == 0, "A root-only tree has height 0");
    check(tree.countLeaves() == 1, "A root with no children is a leaf");

    check(tree.addLeaf("Is it raining?", true, "Take an umbrella"), "Add a yes outcome");
    check(tree.addQuestion("Is it raining?", false, "Is it sunny?"), "Add a no question");
    check(tree.addLeaf("Is it sunny?", true, "Wear sunglasses"), "Add a nested yes outcome");
    check(tree.addLeaf("Is it sunny?", false, "Take a light jacket"), "Add a nested no outcome");
    check(tree.height() == 2, "The completed tree has height 2");
    check(tree.countLeaves() == 3, "The completed tree has three leaves");

    check(!tree.addLeaf("Is it raining?", true, "Duplicate branch"), "Reject an occupied branch");
    check(!tree.addQuestion("Missing parent", false, "New question"), "Reject a missing parent");
    check(!tree.removeLeaf("Is it sunny?"), "Reject removing an internal node as a leaf");
    check(!tree.removeLeaf("Missing outcome"), "Reject removing a missing leaf");
    check(!tree.rename("Missing node", "New name"), "Reject renaming a missing node");
    check(!tree.setAttribute("Missing node", "Tag"), "Reject tagging a missing node");

    check(tree.rename("Take an umbrella", "Bring an umbrella"), "Rename an outcome");
    check(tree.setAttribute("Bring an umbrella", "Rain gear"), "Set a custom attribute");
    const std::string display = captureDisplay(tree);
    check(contains(display, "Is it raining?") && contains(display, "Is it sunny?")
              && contains(display, "Bring an umbrella") && contains(display, "Wear sunglasses")
              && contains(display, "Take a light jacket") && contains(display, "Rain gear")
              && !contains(display, "Take an umbrella"),
          "Display shows questions, outcomes, the new name, and the attribute");

    check(contains(captureSession(tree, "maybe\ny\n"), "Bring an umbrella"),
          "Session recovers from invalid input and follows yes");
    check(contains(captureSession(tree, "n\ny\n"), "Wear sunglasses"), "Session follows no then yes");
    check(contains(captureSession(tree, "n\nn\n"), "Take a light jacket"), "Session follows no then no");

    check(tree.removeLeaf("Take a light jacket"), "Remove a nested leaf");
    check(tree.countLeaves() == 2, "Removing a leaf updates the count");
    check(contains(captureSession(tree, "n\nn\ny\n"), "Wear sunglasses"),
          "Session retries a question after the user selects a missing branch");
    tree.setRoot("Stay inside");
    check(tree.height() == 0 && tree.countLeaves() == 1
              && !contains(captureDisplay(tree), "Is it raining?"),
          "Replacing the root clears the previous tree");
    check(tree.removeLeaf("Stay inside") && tree.isEmpty(), "Removing the only root empties the tree");
}

void testCopyAndMove()
{
    DecisionTree original;
    original.setRoot("Original root");
    original.addLeaf("Original root", true, "Original outcome");
    original.setAttribute("Original outcome", "Original tag");

    DecisionTree copied(original);
    check(copied.rename("Original outcome", "Copied outcome")
              && contains(captureDisplay(original), "Original outcome")
              && contains(captureDisplay(copied), "Copied outcome")
              && contains(captureDisplay(copied), "Original tag"),
          "Copy construction copies the subtree and keeps edits independent");

    DecisionTree assigned;
    assigned.setRoot("Old destination");
    assigned = original;
    check(assigned.removeLeaf("Original outcome")
              && original.height() == 1 && assigned.height() == 0
              && !contains(captureDisplay(assigned), "Old destination"),
          "Copy assignment replaces the destination with an independent copy");

    DecisionTree moved(std::move(copied));
    check(copied.isEmpty() && contains(captureDisplay(moved), "Copied outcome"),
          "Move construction transfers the tree and empties the source");

    assigned = std::move(original);
    check(original.isEmpty() && assigned.height() == 1
              && contains(captureDisplay(assigned), "Original outcome"),
          "Move assignment transfers the tree and empties the source");
}
}

int main()
{
    std::cout << "Decision Tree Manager tests\n\n";
    testEditingAndTraversal();
    testCopyAndMove();
    std::cout << '\n' << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
