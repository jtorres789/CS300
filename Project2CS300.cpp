//=====================================================
// Name       : Project2CS300
// Author     : Jorge Torres
// Version    : 1.0
// Description: CS300 Project 2 
// ====================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>


// Using declarations

using std::cout; using std::cerr; using std::cin;
using std::string; using std::vector;
using std::ifstream; using std::stringstream;

// Course class 
class Course {

	// attributes
	string id{};
	string title{};
	vector<string>* prereqs;
	vector<Course>* prereqs2;

public:

	// course constructor
	// set memory to prereqs 
	Course() {
		prereqs = new vector<string>();
		prereqs2 = new vector<Course>();
	}

	// COnstructor of parameters 
	// Create course Id and title 
	Course(string i, string t) : id(i), title(t) {
		prereqs = new vector<string>();
		prereqs2 = new vector<Course>();
	}

	//get functions 
	string Id() { return this->id; }
	string Title() { return this->title; }
	vector<string>* Prereqs() { return this->prereqs; }


	void AddPrereqs2(Course course) {
		prereqs2->push_back(course);
	}

	// String insertion to the prereq vector 
	void AddPrereq(string title) { prereqs->push_back(title); }

	//Print only ID and Title of course 
	// will be used mostly when outputing an entire list of courses with no prerequisites. 
	void PrintCourseOnly() {
		cout << this->id << ": " << this->title << "\n";
	}

	//Print course with all info
	// Mostly for single couurse print output
	void PrintCourseInfo() {

		//Print course Id and Title
		cout << this->id << ": " << this->title << "\n";

		// in case the prereq vector is not empty
		// cycle trough the vector and print each item
		if (!prereqs->empty()) {
			cout << "\t---> Prereqs: ";

			for (int i = 0; i < prereqs->size(); i++) {
				if (i == prereqs->size() - 1)
					cout << prereqs->at(i) << "\n";
				else
					cout << prereqs->at(i) << ", ";
			}
		}
		else {
			cout << "\t---> Prereqs: NONE\n";
		}
		cout << "\n";
	}
};

//Node Structure to hold the course object and its children( left and right) in tree

struct Node {

	//Node Attributes 
	Course course;
	Node* left;
	Node* right;


	//default Constructor
	// setting pointers both sides to null 
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//Parameter constructor 
	// permits the creation of a node with the course object already defined
	// assingning both children into null 
	Node(Course c) {
		course = c;
		left = nullptr;
		right = nullptr;
	}

	//print only the course object 
	//calls the priunt function  in Course class
	// mostly used when outputing the entire list of courses 
	void PrintCourseOnly() {
		this->course.PrintCourseOnly();
	}

	//Print info for a course
	// will call trhe print info function in the course class 
	// mostly used when printing course with additional info 
	void PrintCourseInfo() {
		this->course.PrintCourseInfo();
	}
};
			

//Binary search class
// figured out it might be better based on professor feedback 
// will hold all courses in a tree data structure 
class BinarySearchTree {

	// class attributes
	Node* root;

	// InOrder Traversal function 
	void InOrderRecur(Node* node) {

		//if the root is empty, simple return
		if (node == nullptr) {
			return;
		}

		// call recursively the left side, call print, then recursively call the right side
		InOrderRecur(node->left);
		node->PrintCourseInfo();
		InOrderRecur(node->right);
	}

	//get parent function 
	// used to recursively get the parent of a given subtree
	Node* GetParentRecur(Node* subtree, Node* node) {

		//if subtree is null, return null
		if (subtree == nullptr) {
			return nullptr;
		}
		//if node is either side of parent, return the parent 
		if (subtree->left == node || subtree->right == node) {
			return subtree;
		}
		//if the bidId is lee than parent node
		// call recursively the left side until we get the parernt 
		if (node->course.Id() < subtree->course.Id()) {
			return GetParentRecur(subtree->left, node);
		}
		// else, recur call the right side until we get the parent
		return GetParentRecur(subtree->right, node);
	}
    /// <summary>
    /// Helper function to recursively insert a course
    /// </summary>
    /// <param name="node"></param>
    /// <param name="course"></param>
    void InsertRecursive(Node* node, Course course) {

        // if the node courseId is greater than the incoming courseId
        if (node->course.Id() > course.Id()) {

            // if the left node is empty, then assign the course to the left node
            if (node->left == nullptr) {
                node->left = new Node(course);
            }

            else
                // otherwise recursively call Insert on the left side
                InsertRecursive(node->left, course);
        }
        else {

            // if the right node is empty, then set the course to the right node
            if (node->right == nullptr) {
                node->right = new Node(course);
            }

            else
                // otherwise, recursively call Insert on the right side
                InsertRecursive(node->right, course);
        }
    }

  
    /// Helper function to recursively search the tree
    Node* SearchRecursiveHelper(Node* node, string courseId) {

        // if the node is not null
        if (node != nullptr) {

            // if the key we are searching for is this node's courseId
            // then return the node
            if (courseId == node->course.Id()) {
                return node;
            }

            // else if the key is less than this node's courseId,
            // then recursively call Search on the left side
            else if (courseId < node->course.Id()) {
                return SearchRecursiveHelper(node->left, courseId);
            }
            else
                // otherwise recursively call Search on the right side
                return SearchRecursiveHelper(node->right, courseId);
        }
        else
            // else return nullptr if the node is not found
            return nullptr;
    }

  
    /// Helper function to recursively get the size of the tree
    size_t GetSizeHelper(Node* node) {
        if (node == nullptr)
            return 0;

        return 1 + GetSizeHelper(node->left) + GetSizeHelper(node->right);
    }

public:


    Node* Root() { return root; }
    /// Constructor
    BinarySearchTree() { root = nullptr; }

    /// Call ClearTree to recursively delete all nodes from tree
    ~BinarySearchTree() { ClearTree(root); }

    /// primarily used by the destructor
    void ClearTree(Node* node) {

        // if the root is null, then the tree is already empty
        if (node == nullptr)
            return;

        // recursively call ClearTree on left and right side to delete each node
        ClearTree(node->left);
        ClearTree(node->right);
        delete node;
    }

    /// Get the size of the tree
    /// Calls the helper function to recursively calculate size of tree
    size_t GetSize() {
        return GetSizeHelper(root);
    }

    /// Public insert function
    /// if the root is null, then the tree is empty so insert the new node at root
    /// otherwise call the recursive insert helper
    void Insert(Course course) {

        // if the tree is empty, then insert the  new node here
        if (root == nullptr) {
            Node* newNode = new Node(course);
            root = newNode;
        }
        else {
            // otherwise, call the insert helper function
            InsertRecursive(root, course);
        }
    }

    /// Public function to get parent of a node
    /// It calls the recursive helper function
    Node* GetParent(Node* node) {
        return GetParentRecur(root, node);
    }
    /// Public function to search for a node based on the courseId
    /// It first converts the input key to uppercase,
    /// then it calls the recursive helper function
    Node* SearchRecursive(string courseId) {

        // convert input to upper case
        for (auto i = 0; i < courseId.size(); ++i) {
            courseId[i] = std::toupper(courseId[i]);
        }

        // Call recursive helper function with root and key
        return SearchRecursiveHelper(root, courseId);
    }


    /// Public function to print the nodes in the tree
    /// It calls the Inorder helper function
    /// </summary>
    void InOrder() {

        // if the root is null, the just return as the tree is empty
        if (root == nullptr) {
            cout << "\n\t* No courses to display *\n";
            return;
        }

        // call the recursive helper function
        InOrderRecur(root);
    }


    int GetNumberOfPrereqs(string courseId) {

        // convert input to upper case
        for (auto i = 0; i < courseId.size(); ++i) {
            courseId[i] = std::toupper(courseId[i]);
        }

        // search for the node based on the courseId
        Node* temp = SearchRecursive(courseId);


        // if the node is found, then print the size of the prereq vector
        // if the node is not found, then just return 0
        if (temp != nullptr)
            return temp->course.Prereqs()->size();
        else
            return 0;
    }

};

/// <summary>
/// Print the introduction to the program
/// </summary>
void PrintIntroduction() {
    cout << "Welcome to the ABC Course Planner\n\n";
}

/// <summary>
/// Print the main menu
/// </summary>
void PrintMenu() {
    cout << "\n\n\tMain Menu\n";
    cout << "1. Load Courses from a file\n";
    cout << "2. Display all courses\n";
    cout << "3. Display course information\n";
    cout << "9. Exit application\n";
}

/// Get the menu choice from user input
int GetChoice() {
    int choice = 0;
    cout << "\nPlease enter your selection: ";
    cin >> choice;
    cin.clear();
    cin.ignore();
    return choice;
}

/// Get the filename from user input
string GetFileName() {
    string filename{};
    cout << "\nPlease enter the filename: ";
    getline(cin, filename);
    return filename;
}

/// Get user input for the courseId they want to search for
string GetInputCourse() {
    string userInput{};
    cout << "\nPlease enter the course id: ";
    getline(cin, userInput);
    return userInput;
}

string trimWord(string str) {
    const char* removeChar = " \t\n\r\f\v";
    str.erase(str.find_last_not_of(removeChar) + 1);
    str.erase(0, str.find_first_not_of(removeChar));
    return str;
}

/// This function reads the file and creates a vector of only the courseId's
/// It is used to store the master list of courses that we use for verifying against prereqs
void CreateMasterCourseList(string filepath, vector<string>& courses) {

    // create stream object for reading the file
    ifstream infile;

    // attempt to open the file
    infile.open(filepath);

    // if the file does not exist then return
    if (!infile) {
        //   cerr << filepath << " does not exist\n";
        return;
    }

    //cout << "\n\t* Successfully loaded file " << filepath << " *\n\n";

    // local variables used in this function
    string line{};
    char delim = ',';
    string word{};
    vector<string> courseLine = vector<string>();

    // read each line of the file
    while (getline(infile, line)) {

        // create stringstream object to process each line
        stringstream fullLine(line);

        // store each word of the line into the courses vector
        while (getline(fullLine, word, delim)) {

            // trim any whitespace chars from the word
            word = trimWord(word);

            // add the word to the courseLine vector
            courseLine.push_back(word);
        }

        // we only need the first word of the line
        courses.push_back(courseLine[0]);

        // clear the course line before next iteration
        courseLine.clear();
    }

    // close file
    infile.close();
}


/// Load courses based on file name
/// Read each line and build a course object
/// Then insert the course object into the tree
void LoadCourses(string filepath, BinarySearchTree* bst) {

    // Vector used to create the master course list
    vector<string> masterCourseList = vector<string>();
    CreateMasterCourseList(filepath, masterCourseList);

    // No read the file again and capture all the words of each line for processing
    ifstream infile;

    // attempt to open the file
    infile.open(filepath);

    // if the file does not exist then return
    if (!infile) {
        cerr << filepath << " does not exist\n";
        return;
    }

    // inform user that the file has been loaded successfully
    cout << "\n\t* Successfully loaded file " << filepath << " *\n\n";

    // local variables used in main
    string line{};
    char delim = ',';
    string word{};
    vector<string> courseLine = vector<string>();
    vector<string> tempCourseList = vector<string>();

    // read each line of the file
    while (getline(infile, line)) {

        // create stringstream object to process each line
        stringstream fullLine(line);

        // store each word of the line into the courseLine vector
        while (getline(fullLine, word, delim)) {
            word = trimWord(word);
            courseLine.push_back(word);
        }

        // if the line does not contain at least two words, 
        // then we know the file has an incomplete record,
        // return back to caller
        if (courseLine.size() < 2) {
            cerr << "Incomplete record in the file\n";
            return;
        }

        // create a temporary vector to store the courseId's only
        // this vector is used to validate prereq courses
        //vector<string> tempCourseList = vector<string>();
        tempCourseList.push_back(courseLine[0]);

        // create the course object using the parameterized constructor
        // index 0 is the courseId and index 1 is the course title
        Course c(courseLine[0], courseLine[1]);

        // search remaining vector if it has more strings
        // if there are more words in the string, then the course
        // has prereqs.
        // So we add those words to the course prereq vector if they qualify
        if (courseLine.size() > 2) {

            // iterate over the remaining words in the course line
            for (auto iter = courseLine.begin() + 2;
                iter != courseLine.end(); ++iter) {

                // check to see if the next prereq is a valid course by searching the masterCourseList
                vector<string>::iterator testIter = std::find(masterCourseList.begin(), masterCourseList.end(), *iter);

                // if the prereq is a valid course, then add it to the course object prereq vector
                // otherwise just skip the word because it is not a valid course
                if (testIter != masterCourseList.end())
                    c.AddPrereq(*iter);
            }
        }

        // now that we have completed object creation,
        // Before we add the course, we need to make sure it doesn't already exist
        // Search the tree with the new node Id
        Node* verifyNode = bst->SearchRecursive(c.Id());

        // if it doesn't yet exist, then insert into tree
        if (verifyNode == nullptr) {

            // let's insert it into the tree
            bst->Insert(c);

            // print the course to output to verify it has been created
            cout << c.Id() << " " << c.Title() << " has been read\n";

        }

        // if the course Id already exists, then don't add it again
        else {
            cout << c.Id() << " already exists\n";
        }


        // clear the courseLine vector before next iteration of the loop
        courseLine.clear();

    }

    // close the file
    infile.close();

}


/// Program start function
/// Can take in command line argument for the filename
int main(int argc, char* argv[])
{
    // variable to store the filename
    string filename{};

    // switch statement to capture command line arguments if they are given
    switch (argc) {
    case 2:
        filename = argv[1];
        break;
    case 3:
        break;
    default:
        filename = "ABCU_test.txt";
        break;

    }

    // instantiate the binary search tree
    BinarySearchTree* bst = new BinarySearchTree();

    // print the introduction
    PrintIntroduction();

    // local variables used in the loop
    int choice = 0;
    string inputCourse{};
    Node* temp{};
    vector<string> CourseListTest = vector<string>();

    // main loop - continue processing until user selects 9 to exit the app
    while (choice != 9) {

        // Print the menu and get user input for their menu selection
        PrintMenu();
        choice = GetChoice();

        // switch statement to process the menu selection
        switch (choice) {

            // load courses
        case 1:

            // get user input for the filename
            filename = GetFileName();
            LoadCourses(filename, bst);


            // this line for testing
            //LoadCourses("ABCU_test.txt", bst);

            cout << "\n\n\t* Press enter to continue *\n";
            cin.get();
            break;

            // display the course listing InOrder
        case 2:
            cout << "\n\nCourse Listing\n";
            bst->InOrder();
            cout << bst->GetSize() << " courses stored\n";
            cout << "\n\n\t* Press enter to continue *\n";
            cin.get();
            break;

            // Enable user to input courseId for a course they want more info about
        case 3:

            // get user input
            inputCourse = GetInputCourse();

            // search the tree to make sure the course exists
            temp = bst->SearchRecursive(inputCourse);

            // if the course is not found, then inform user
            if (temp == nullptr) {
                cout << "\n\n\t* " << inputCourse << " not found *\n";
            }
            // otherwise, print the course info
            else {
                cout << "\n\nCourse found: \n";
                temp->PrintCourseInfo();
            }
            cout << "\n\n\t* Press enter to continue *\n";
            cin.get();
            break;
        case 9:
            break;
        default:
            cerr << "\n\n\t* Invalid input, please try again * \n\n";
            break;
        }

    }

    cout << "\n\nThank you for using the ABC Course Planner\n";

}

