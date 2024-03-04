//============================================================================
// Paulina Weaver
// CS300
// Project 2
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

// Define Course and Node structures
struct Course { // Define a structure named Course
    string courseNumber; // String variable to store the course number
    string courseTitle; // String variable to store the course title
    vector<string> coursePrerequisites; // Vector to store the course prerequisites
};

struct Node { // Define a structure named Node
    Course course; // Course object to store the course data
    Node* left; // Pointer to the left child node
    Node* right; // Pointer to the right child node

    Node(const Course& aCourse) : course(aCourse), left(nullptr), right(nullptr) {} // Constructor to initialize the Node object with a course
};

// Binary Search Tree class
class BinarySearchTree { // Define a class named BinarySearchTree
private:
    Node* root; // Pointer to the root node of the binary search tree

    // Private member functions
    void addNode(Node*& node, const Course& course); // Function to add a node to the binary search tree
    void inOrder(Node* node); // Function to perform in-order traversal of the binary search tree
    Node* search(Node* node, const string& courseNumber); // Function to search for a course in the binary search tree
    void destroyTree(Node* node); // Destructor helper function to deallocate memory for all nodes in the tree

public:
    // Public member functions
    BinarySearchTree() : root(nullptr) {} // Constructor to initialize the BinarySearchTree object with a null root
    ~BinarySearchTree(); // Destructor

    void insert(const Course& course); // Function to insert a course into the binary search tree
    Course* search(const string& courseNumber); // Function to search for a course in the binary search tree
    void inOrderTraversal(); // Function to perform in-order traversal of the binary search tree
    void printCourse(const string& courseNumber); // Function to print the details of a specific course
    bool courseExists(string courseNumber); // Function to check if a course exists in the binary search tree
    void loadCourses(const string& fileName); // Function to load courses from a file into the binary search tree
};

// Destructor for BinarySearchTree
BinarySearchTree::~BinarySearchTree() {
    destroyTree(root); // Call the destroyTree function to deallocate memory for all nodes in the tree
}

// Insert function for BinarySearchTree
void BinarySearchTree::insert(const Course& course) {
    if (root == nullptr) { // If the tree is empty
        root = new Node(course); // Create a new root node with the given course
    }
    else {
        addNode(root, course); // Call the addNode function to insert the course into the tree
    }
}

// Destructor helper function to deallocate memory for all nodes in the tree
void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) { // If the node exists
        destroyTree(node->left); // Recursively delete the left subtree
        destroyTree(node->right); // Recursively delete the right subtree
        delete node; // Delete the current node
    }
}

// AddNode function for BinarySearchTree
void BinarySearchTree::addNode(Node*& node, const Course& course) {
    if (node == nullptr) { // If the current node is null
        node = new Node(course); // Create a new node with the given course
    }
    else if (course.courseNumber < node->course.courseNumber) { // If the course number is less than the current node's course number
        addNode(node->left, course); // Recursively insert the course into the left subtree
    }
    else {
        addNode(node->right, course); // Recursively insert the course into the right subtree
    }
}

// Search function for BinarySearchTree
Course* BinarySearchTree::search(const string& courseNumber) {
    Node* currentNode = root; // Start from the root node
    while (currentNode != nullptr) { // Continue until the current node is null
        if (currentNode->course.courseNumber == courseNumber) { // If the course number matches
            return &(currentNode->course); // Return a pointer to the course
        }
        else if (courseNumber < currentNode->course.courseNumber) { // If the course number is less than the current node's course number
            currentNode = currentNode->left; // Move to the left subtree
        }
        else {
            currentNode = currentNode->right; // Move to the right subtree
        }
    }
    // Course not found, return nullptr
    return nullptr;
}

// InOrder traversal function for BinarySearchTree
void BinarySearchTree::inOrderTraversal() {
    inOrder(root); // Start in-order traversal from the root node
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) { // If the node exists
        inOrder(node->left); // Recursively traverse the left subtree
        // Print course details
        cout << "\n" << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        if (!node->course.coursePrerequisites.empty()) { // If course prerequisites exist
            cout << "Prerequisites: ";
            // Loop through prerequisites
            for (size_t i = 0; i < node->course.coursePrerequisites.size(); ++i) {
                cout << node->course.coursePrerequisites[i];
                // Print comma if not the last prerequisite
                if (i != node->course.coursePrerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        else {
            cout << "No prerequisites.\n" << endl; // If no prerequisites
        }
        inOrder(node->right); // Recursively traverse the right subtree
    }
}

// PrintCourse function for BinarySearchTree
void BinarySearchTree::printCourse(const string& courseNumber) {
    Course* foundCourse = search(courseNumber); // Search for the course
    if (foundCourse) { // If the course is found
        // Print course details
        cout << "\n" << foundCourse->courseNumber << ", " << foundCourse->courseTitle << endl;
        if (!foundCourse->coursePrerequisites.empty()) { // If course prerequisites exist
            cout << "Prerequisites: ";
            // Loop through prerequisites
            for (size_t i = 0; i < foundCourse->coursePrerequisites.size(); ++i) {
                cout << foundCourse->coursePrerequisites[i];
                // Print comma if not the last prerequisite
                if (i != foundCourse->coursePrerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        else {
            cout << "No prerequisites.\n" << endl; // If no prerequisites
        }
    }
    else {
        cout << "\nCourse not found.\n" << endl; // If the course is not found
    }
}

// CourseExists function for BinarySearchTree
bool BinarySearchTree::courseExists(string courseNumber) {
    return search(courseNumber) != nullptr; // Check if the course exists in the tree
}

// LoadCourses function for BinarySearchTree
void BinarySearchTree::loadCourses(const string & fileName) {
    string line;
    ifstream file(fileName); // Open the file

    unordered_set<string> courses; // Store course numbers in a set

    try {
        if (!file.is_open()) { // If the file cannot be opened
            throw runtime_error("\nError: File not found at path: " + fileName); // Throw an exception
        }
        // Read all lines from the file to build the list of courses
        while (getline(file, line)) { // Read each line from the file
            vector<string> data;
            size_t pos = 0;
            string token;
            while ((pos = line.find(',')) != string::npos) { // Split the line by comma
                token = line.substr(0, pos);
                data.push_back(token); // Add token to data vector
                line.erase(0, pos + 1); // Erase token from the line
            }
            data.push_back(line); // Add the remaining line to data vector

            if (data.size() < 2) { // If the data is insufficient
                throw runtime_error("\nError: Insufficient parameters in line: " + line); // Throw an exception
                continue;
            }
            else {
                string courseNumber = data[0]; // Extract course number
                courses.insert(courseNumber); // Add course to the set
            }
        }
    }
    catch (const exception& e) {
        cerr << e.what() << endl; // Print error message if an exception occurs
        file.close(); // Close the file
        return; // Exit the function
    }

    file.close(); // Close the file

    // Re-open the file to process each line now that we have the set of courses
    file.open(fileName); // Re-open the file
    if (!file.is_open()) { // If the file cannot be opened
        throw runtime_error("\nError: File not found at path: " + fileName); // Throw an exception
    }

    try {
        // Process each line again to insert courses into the binary search tree
        while (getline(file, line)) { // Read each line from the file
            vector<string> data;
            size_t pos = 0;
            string token;
            while ((pos = line.find(',')) != string::npos) { // Split the line by comma
                token = line.substr(0, pos);
                data.push_back(token); // Add token to data vector
                line.erase(0, pos + 1); // Erase token from the line
            }
            data.push_back(line); // Add the remaining line to data vector

            if (data.size() < 2) { // If the data is insufficient
                throw runtime_error("\nError: Insufficient parameters in line: " + line); // Throw an exception
                continue;
            }
            else {
                string courseNumber = data[0]; // Extract course number
                string courseTitle = data[1]; // Extract course title
                vector<string> coursePrerequisites;

                if (data.size() > 2) { // If prerequisites exist
                    for (size_t i = 2; i < data.size(); ++i) { // Iterate over prerequisites
                        string prerequisite = data[i]; // Extract prerequisite
                        if (courses.find(prerequisite) == courses.end()) { // Check if prerequisite exists in the set
                            throw runtime_error("\nError: Prerequisite does not exist as a course: " + prerequisite); // Throw an exception
                            continue;
                        }
                        coursePrerequisites.push_back(prerequisite); // Add prerequisite to the vector
                    }
                }

                Course course{ courseNumber, courseTitle, coursePrerequisites }; // Create a course object
                insert(course); // Insert the course into the binary search tree
            }
        }
        cout << "\nFile loaded successfully." << endl; // Output message indicating successful loading
    }
    catch (const exception& e) {
        cerr << e.what() << endl; // Print error message if an exception occurs
    }

    file.close(); // Close the file
}

// Menu function
void menu() {
    BinarySearchTree courseBst; // Create an instance of BinarySearchTree
    int choice = 0; // Variable to store user's choice
    string courseNumber, fileName; // Variable to store course number

    cout << "Welcome to the course planner.\n" << endl;  

    while (true) { // Infinite loop for menu
        cout << "Menu:" << endl;
        cout << "1. Load Binary Search Tree." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "4. Exit." << endl;
        cout << "Enter your choice: ";

        // Check if the input is a valid integer
        if (!(cin >> choice)) { // If input is not a valid integer
            // If not, clear the error flag and discard the input buffer
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a number.\n" << endl; // Display error message
            continue; // Skip to next iteration of the loop
        }

        switch (choice) { // Switch statement based on user's choice

        case 1:
            // Prompt the user to enter the filename
            cout << "\nEnter the filename containing the course data: ";
            cin >> fileName;

            courseBst.loadCourses(fileName); // Call loadCourses function
            cout << endl;
            break;

        case 2:
            cout << "\nHere is a sample schedule:\n" << endl;
            courseBst.inOrderTraversal(); // Call inOrderTraversal function
            break;

        case 3:
            cout << "\n" << "Enter course number: ";
            cin >> courseNumber; // Input course number
            courseBst.printCourse(courseNumber); // Call printCourse function
            break;

        case 4:
            cout << "\nThank you for using the course planner!\nExiting program." << endl; // Exit message
            return; // Exit the function and the program

        default:
            cout << "\nInvalid choice. Please try again.\n" << endl; // Error message for invalid choice
            break;
        }
    }
}

int main() {
    menu(); // Call the menu function
    return 0; // Return 0 to indicate successful execution
}
