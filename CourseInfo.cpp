//Kaitlyn Lush

#include <iostream>
#include <fstream>
#include <vector>

#include <sstream>
#include "CSVParser.hpp"


using namespace std;

//defines structure for each course containing an id, name, and vector to hold prereqs
struct Course {
	string courseNum;
	string courseName;
	vector<string> prereqs;

	Course() {}
};

//defines structure of node containing course info and left and right children
struct Node {
	Course course;
	Node* left;
	Node* right;
	//sets node branches to nullptr
	Node() {
		left = nullptr;
		right = nullptr;
	}
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};
//defines Course binary search tree class
class CourseBST {
private:
	Node* root;

	void addNode(Node* node, Course course);
	void printSampleSchedule(Node* node);
	void printCourseInformation(Node* node, string courseNum);

public:
	CourseBST();
	virtual ~CourseBST();
	void DeleteRecursive(Node* node);
	void Insert(Course course);
	int NumPrerequisiteCourses(Course course);
	void PrintSampleSchedule();
	void PrintCourseInformation(string courseNum);

};
//constructs binary search tree and initialize root
CourseBST::CourseBST() {
	root = nullptr;
}
//deconstruct Binary search tree by passing root to function to recursively delete each node
CourseBST::~CourseBST() {
	DeleteRecursive(root);
}
//delete bst nodes recursively 
void CourseBST::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}
//insert node into bst
void CourseBST::Insert(Course course) {
	//if root not initialized
	if (root == nullptr)
		//set with current course info
		root = new Node(course);
	//else send node to addnode to be added to bst
	else
		this->addNode(root, course);
}
//returns number of prereqs course has
int CourseBST::NumPrerequisiteCourses(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.prereqs.size(); i++) {
		if (course.prereqs.at(i).length() > 0)
			count++;
	}
	return count;
}
//passes bst root to private method
void CourseBST::PrintSampleSchedule() {
	this->printSampleSchedule(root);
}
//searches courseID and displays if found
void CourseBST::PrintCourseInformation(string courseNum) {
	this->printCourseInformation(root, courseNum);
}
//public insert method
//sets alphanumeric location and if current child is nullptr adds node to bst
//else traverses bst until nullptr is found
void CourseBST::addNode(Node* node, Course course) {
	//if current courseNum less than current node
	if (node->course.courseNum.compare(course.courseNum) > 0) {
		if (node->left == nullptr)
			node->left = new Node(course);
		else
			this->addNode(node->left, course);
	}
	//else current courseNum = or > than current node
	else {
		if (node->right == nullptr)
			node->right = new Node(course);
		else
			this->addNode(node->right, course);
	}
}
//recursively prints loaded courses in order
void CourseBST::printSampleSchedule(Node* node) {
	if (node != nullptr) {
		printSampleSchedule(node->left);
		cout << node->course.courseNum << "," << node->course.courseName << endl;
		printSampleSchedule(node->right);
	}
	return;
}
//displays single course info including prereqs
void CourseBST::printCourseInformation(Node* curr, string courseNum) {
	//traverse bst until matching courseNum found or until bottom is reached
	while (curr != nullptr) {
		if (curr->course.courseNum.compare(courseNum) == 0) {
			//display course & get prereqs
			cout << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;
			unsigned int size = NumPrerequisiteCourses(curr->course);
			cout << "Prerequisite(s): ";

			//if prereq exists display each prereq w correct format
			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.prereqs.at(i);
				if (i != size - 1)
					cout << ", ";
			}
			//if no prereqs display to user and return to stop search
			if (i == 0)
				cout << "No prerequisites required.";
			cout << endl;
			return;
		}

		//passed courseNum < than current traverse left
		else if (courseNum.compare(curr->course.courseNum) < 0)
			curr = curr->left;

		//passed courseNum > than cuurent traverse right
		else
			curr = curr->right;
	}
	//reached nullptr and course not found then inform user
	cout << "Course " << courseNum << " not found." << endl;
}
//load courses for csv file
bool loadCourses(string csvPath, CourseBST* coursesBST) {
	//open course file get each line and insert into bst
	try {
		fstream courseFile(csvPath);

		if (courseFile.is_open()) {
			while (!courseFile.eof()) {

				//vector to hold each csv 
				vector<string> courseInfo;
				string courseData;

				getline(courseFile, courseData);
				while (courseData.length() > 0) {
					//get substring of each course data add to vector & delete
					unsigned int comma = courseData.find(',');
					if (comma < 100) {
						courseInfo.push_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}
					//add last course after final comma
					else {
						courseInfo.push_back(courseData.substr(0, courseData.length()));
						courseData = "";
					}
				}
				//load separated values into a course and insert into bst & close file
				Course course;
				course.courseNum = courseInfo[0];
				course.courseName = courseInfo[1];

				for (unsigned int i = 2; i < courseInfo.size(); i++) {
					course.prereqs.push_back(courseInfo[i]);
				}
				coursesBST->Insert(course);
			}
			courseFile.close();
			return true;
		}
	}
	catch (csv:Error& e) {
		cerr << e.what() << endl;
}
	return false;
}

//Main
int main(int argc, char* argv[]) {
	//command line argument
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "";
		break;
	}
	//define bst 
	CourseBST* coursesBST = nullptr;
	cout << "\nWelcome to the course planner.\n" << endl;

	//make user choice a string & convert first char to int prevents invalid data
	string choice = "0";
	int userChoice = choice[0] - '0';

	//if not 9 or exit
	while (userChoice != 9) {
		cout << "  1. Load Data Structure" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << endl;
		cout << "\nWhat would you like to do? ";
		cin >> choice;

		//check is user choice is double digit
		if (choice.length() == 1)
			userChoice = choice[0] - '0';
		else
			userChoice = 0;
		bool success = 1;

		//handles user choice from menu
		switch (userChoice) {
			//get file path name and load courses into bst
		case 1:
			if (coursesBST == nullptr)
				coursesBST = new CourseBST();
			if (csvPath.length() == 0) {
				cout << "Enter the file name that contains the course data: ";
				cin >> csvPath;
			}
			//determine if file opened
			success = loadCourses(csvPath, coursesBST);
			if (success)
				cout << "Courses have been loaded.\n" << endl;
			else
				cout << "File not found.\n" << endl;
			csvPath = "";
			break;
			//validate bst exists & print sample schedule
		case 2:
			if (coursesBST != nullptr && success) {
				cout << "Here is a sample schedule:\n" << endl;
				coursesBST->PrintSampleSchedule();
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			break;

			//search display course info
		case 3:
			if (coursesBST != nullptr && success) {
				if (courseId.length() == 0) {
					cout << "What course do you want to know about? ";
					cin >> courseId;
					for (auto& userChoice : courseId) userChoice = toupper(userChoice);
				}
				coursesBST->PrintCourseInformation(courseId);
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			courseId = "";
			break;
			// user entered invalid data or exited program
		default:
			if (userChoice != 9)
				cout << choice << " is not a valid option\n" << endl;
			break;
		}
	}
	cout << "\nThank you for using the course planner!" << endl;
	return 0;
}