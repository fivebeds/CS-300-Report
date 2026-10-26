//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Kyle
// Version     : 1.0
// Copyright   : Copyright (c) 2023 SNHU COCE
// Description : Lab 5-2 Binary Search Tree
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);
struct Bid;
void displayBid(Bid bid);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// Internal structure for tree node
struct Node {
    Bid bid;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Bid aBid) :
            Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;      // top of the tree (nullptr when the tree is empty)
    int size;        // housekeeping: number of bids currently stored

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string bidId);
    void destroyTree(Node* node);   // helper used only by the destructor

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // An empty tree has no root and holds zero bids
    root = nullptr;
    size = 0;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // Recurse from the root, deleting every node so no memory is leaked
    destroyTree(root);
    root = nullptr;
}

/**
 * Delete a node and its subtrees (post-order so children go before parents)
 *
 * @param node Current node in tree
 */
void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

/**
 * Number of bids currently stored in the tree
 */
int BinarySearchTree::Size() {
    return size;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // Start the recursive in-order traversal at the root
    inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // Start the recursive post-order traversal at the root
    postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // Start the recursive pre-order traversal at the root
    preOrder(root);
}



/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    if (root == nullptr) {
        // Empty tree: the new bid becomes the root
        root = new Node(bid);
    } else {
        // Otherwise walk down from the root to find the bid's slot
        addNode(root, bid);
    }
    size++;
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // removeNode returns the (possibly new) root of the tree
    root = removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // Start at the root and walk downward (iteratively, no recursion needed)
    Node* current = root;

    // Keep going until we fall off the bottom or find a matching bidId
    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            return current->bid;
        }

        // Smaller keys live in the left subtree, larger in the right
        if (bidId < current->bid.bidId) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // Not found: return an empty Bid (empty bidId signals "not found")
    Bid bid;
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    if (node->bid.bidId > bid.bidId) {
        // Current node is larger, so the new bid belongs on the left
        if (node->left == nullptr) {
            node->left = new Node(bid);
        } else {
            addNode(node->left, bid);
        }
    } else {
        // Equal or smaller than the new bid, so it belongs on the right
        if (node->right == nullptr) {
            node->right = new Node(bid);
        } else {
            addNode(node->right, bid);
        }
    }
}


void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);       // everything smaller first
        displayBid(node->bid);     // then this node
        inOrder(node->right);      // then everything larger
    }
}
void BinarySearchTree::postOrder(Node* node) {
    if (node != nullptr) {
        postOrder(node->left);     // left subtree
        postOrder(node->right);    // right subtree
        displayBid(node->bid);     // this node last
    }
}

void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) {
        displayBid(node->bid);     // this node first
        preOrder(node->left);      // left subtree
        preOrder(node->right);     // right subtree
    }
}

/**
 * Remove a bid from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    // Base case: fell off the tree, so the bid does not exist
    if (node == nullptr) {
        return node;
    }

    if (bidId < node->bid.bidId) {
        // Target is smaller: recurse into the left subtree and re-link
        node->left = removeNode(node->left, bidId);
    } else if (bidId > node->bid.bidId) {
        // Target is larger: recurse into the right subtree and re-link
        node->right = removeNode(node->right, bidId);
    } else {
        // Match found
        if (node->left == nullptr && node->right == nullptr) {
            // Case 1: leaf node, just delete it
            delete node;
            size--;
            return nullptr;
        } else if (node->left != nullptr && node->right == nullptr) {
            // Case 2a: only a left child, promote it
            Node* temp = node->left;
            delete node;
            size--;
            return temp;
        } else if (node->left == nullptr && node->right != nullptr) {
            // Case 2b: only a right child, promote it
            Node* temp = node->right;
            delete node;
            size--;
            return temp;
        } else {
            // Case 3: two children. Find the in-order successor
            // (smallest node in the right subtree)
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            // Copy the successor's bid up, then delete the successor
            // from the right subtree (it has at most one child)
            node->bid = temp->bid;
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }
    return node;
}



//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
