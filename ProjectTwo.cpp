//============================================================================
// Name        : ProjectTwo.cpp
// Course      : CS 300 - Project Two
// Description : ABCU advising assistance program. Loads course data from a
//               CSV file into a binary search tree, then lets an advisor
//               print an alphanumeric course list or look up one course.
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Holds the data for a single course.
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Removes leading/trailing whitespace (including stray '\r' from Windows files).
string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

// Returns an upper-case copy of the text so lookups are case-insensitive.
string toUpper(string text) {
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char c) { return static_cast<char>(toupper(c)); });
    return text;
}

// Binary search tree keyed on course number; in-order traversal gives
// the courses in alphanumeric order.
class CourseTree {
public:
    CourseTree() : root(nullptr) {}
    ~CourseTree() { destroy(root); }

    // Prevent accidental copies (the tree owns raw pointers).
    CourseTree(const CourseTree&) = delete;
    CourseTree& operator=(const CourseTree&) = delete;

    // Inserts a course; duplicate course numbers are ignored.
    void insert(const Course& course) { insert(root, course); }

    // Prints every course in alphanumeric order.
    void printInOrder() const { printInOrder(root); }

    // Returns a pointer to the matching course, or nullptr if not found.
    const Course* search(const string& courseNumber) const {
        const Node* current = root;
        while (current != nullptr) {
            if (courseNumber == current->course.courseNumber) {
                return &current->course;
            }
            current = (courseNumber < current->course.courseNumber)
                          ? current->left : current->right;
        }
        return nullptr;
    }

    bool isEmpty() const { return root == nullptr; }

    // Empties the tree so the data can be reloaded cleanly.
    void clear() {
        destroy(root);
        root = nullptr;
    }

private:
    struct Node {
        Course course;
        Node* left;
        Node* right;
        explicit Node(const Course& c) : course(c), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void insert(Node*& node, const Course& course) {
        if (node == nullptr) {
            node = new Node(course);
        } else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        } else if (course.courseNumber > node->course.courseNumber) {
            insert(node->right, course);
        }
    }

    void printInOrder(const Node* node) const {
        if (node == nullptr) {
            return;
        }
        printInOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseName << endl;
        printInOrder(node->right);
    }

    void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
};

// Reads the course file into the tree. Returns true on success.
bool loadCourses(const string& fileName, CourseTree& tree) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: could not open file \"" << fileName << "\"." << endl;
        return false;
    }

    vector<Course> courses;
    set<string> knownNumbers;
    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        ++lineNumber;
        if (trim(line).empty()) {
            continue;  // skip blank lines
        }

        // Split the line on commas, dropping empty trailing fields.
        vector<string> tokens;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            token = trim(token);
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }

        // Every line needs at least a course number and a course name.
        if (tokens.size() < 2) {
            cout << "Error: line " << lineNumber
                 << " is missing a course number or name." << endl;
            return false;
        }

        Course course;
        course.courseNumber = toUpper(tokens[0]);
        course.courseName = tokens[1];
        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(toUpper(tokens[i]));
        }

        knownNumbers.insert(course.courseNumber);
        courses.push_back(course);
    }

    // Second pass: every prerequisite must be a course that exists in the file.
    for (const Course& course : courses) {
        for (const string& prereq : course.prerequisites) {
            if (knownNumbers.find(prereq) == knownNumbers.end()) {
                cout << "Error: " << course.courseNumber << " lists prerequisite "
                     << prereq << ", which is not in the file." << endl;
                return false;
            }
        }
    }

    // File is valid, so replace any previously loaded data.
    tree.clear();
    for (const Course& course : courses) {
        tree.insert(course);
    }
    return true;
}

// Prints the title and prerequisites for a single course.
void printCourse(const CourseTree& tree, const string& input) {
    const Course* course = tree.search(toUpper(trim(input)));
    if (course == nullptr) {
        cout << trim(input) << " was not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseName << endl;
    cout << "Prerequisites: ";
    if (course->prerequisites.empty()) {
        cout << "None";
    }
    for (size_t i = 0; i < course->prerequisites.size(); ++i) {
        if (i > 0) {
            cout << ", ";
        }
        cout << course->prerequisites[i];
    }
    cout << endl;
}

void displayMenu() {
    cout << endl;
    cout << "   1. Load Data Structure." << endl;
    cout << "   2. Print Course List." << endl;
    cout << "   3. Print Course." << endl;
    cout << "   9. Exit" << endl;
    cout << endl;
}

int main() {
    CourseTree courseTree;
    bool dataLoaded = false;
    string choice;

    cout << "Welcome to the course planner." << endl;

    while (true) {
        displayMenu();
        cout << "What would you like to do? ";
        if (!getline(cin, choice)) {
            break;  // input stream closed
        }
        choice = trim(choice);

        if (choice == "1") {
            cout << "Enter the name of the course data file: ";
            string fileName;
            getline(cin, fileName);
            fileName = trim(fileName);
            if (loadCourses(fileName, courseTree)) {
                dataLoaded = true;
                cout << "Course data loaded successfully." << endl;
            }
        } else if (choice == "2") {
            if (!dataLoaded) {
                cout << "Please load the data (option 1) first." << endl;
            } else {
                cout << "Here is a sample schedule:" << endl << endl;
                courseTree.printInOrder();
            }
        } else if (choice == "3") {
            if (!dataLoaded) {
                cout << "Please load the data (option 1) first." << endl;
            } else {
                cout << "What course do you want to know about? ";
                string courseNumber;
                getline(cin, courseNumber);
                printCourse(courseTree, courseNumber);
            }
        } else if (choice == "9") {
            cout << "Thank you for using the course planner!" << endl;
            return 0;
        } else {
            cout << choice << " is not a valid option." << endl;
        }
    }
    return 0;
}
