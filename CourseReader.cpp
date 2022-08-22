//============================================================================
// Name        : CourseReader.cpp
// Author      : Thomas Dang
// Version     : 1.0
// Description : Project 2 - Course Storage, Sorting, and Display
//============================================================================

#include <fstream>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

struct Course {
	string courseNumber;
	string courseName;
	vector<string> prereqs;
};

/*
* Implementation of a quick sort algorithm
* @param courses address to be sorted
* @param int begin for sorting
* @param int end index for sorting
* 
*/
int partition(vector<Course>& courses, int begin, int end) {
	bool run = true;
	int low = begin;
	int high = end;
	int pivot = low + (high - low) / 2;

	while (run) {
		while (courses.at(low).courseNumber < courses.at(pivot).courseNumber) {
			low++;
		}
		while (courses.at(high).courseNumber > courses.at(pivot).courseNumber) {
			high--;
		}
		if (low >= high) {
			run = false;
		} else {
			swap(courses.at(low), courses.at(high));
			low++;
			high--;
		}
	}
	return high;
}

/*
* Implementation of a quick sort algorithm
* @param courses address to be sorted
* @param int begin for sorting
* @param int end index for sorting
*
*/
void sort(vector<Course>& courses, int begin, int end) {
	int mid = 0;

	if (begin >= end) {
		return;
	}
	mid = partition(courses, begin, end);
	//Recursively sort partitions until each has only one element, at which point the vector has been sorted
	sort(courses, begin, mid);
	sort(courses, mid + 1, end);
}

/*
* Find and return a course in a vector of courses. Returns an empty course if the course is not found.
* @param courseNumber of course to find
* @param courses vector to search
*/
Course searchCourse(string courseNumber, vector<Course> courses) {
	Course returnCourse;
	
	for (Course course : courses) {
		if (course.courseNumber == courseNumber) {
			returnCourse = course;
			break;
		}
	}
	return returnCourse;
}

/*
* Check that the prerequisites of each course exists in the list of courses
* Remove the prereq from the course if it is not found
* 
* @param courses address of courses to validate
* 
*/
void validatePrereqs(vector<Course>& courses) {
	for (Course& course : courses) {
		vector<string>& prereqs = course.prereqs;
		if (prereqs.size() > 0) {
			for (int i = 0; i < prereqs.size(); i++) {
				Course checkCourse = searchCourse(prereqs.at(i), courses);
				if (checkCourse.courseNumber.compare(prereqs.at(i)) != 0 || prereqs.at(i) == "") {
					prereqs.erase(prereqs.begin()+i);
				}
			}
		}
	}
}

/*
* Extract data from a csv file
* @param filename to read data from
*/
vector<Course> loadData(string filename) {
	ifstream inF;
	string currLine;
	vector<Course> courses;

	inF.open(filename);
	//Verify file has been opened. If not, file does not exist or can not be read.
	if (!inF.is_open()) {
		inF.close();
		cout << "Something went wrong, please verify the correct file was input." << endl;
		return courses;
	}

	while (!inF.eof()) {
		//Gets line until linebreak
		while (getline(inF, currLine, '\n')) {

			Course course;
			string value;
			vector<string> row;

			while (currLine != "") {
				if (currLine.find(',') < currLine.length()) {
					value = currLine.substr(0, currLine.find(','));
					currLine.erase(0, currLine.find(',') + 1);
					row.push_back(value);
				}
				else {
					row.push_back(currLine);
					currLine = "";
				}
			}
			//If row does not contain at least 2 parameters, a line in the file is improperly formatted and should be skipped.
			if (row.size() < 2) {
				continue;
			}
			course.courseNumber = row[0];
			course.courseName = row[1];

			for (int i = 2; i < row.size(); i++) {
				course.prereqs.push_back(row[i]);
			}
			courses.push_back(course);
		}
	}
	inF.close();

	if (empty(courses)) {
		cout << "The file was found, but no courses were extracted. Please verify the file is formatted as a csv and try again." << endl;
	}
	else {
		sort(courses, 0, courses.size() - 1);
		validatePrereqs(courses);
	}
	return courses;
}

/*
*@param course to print info from
*/
void printCourse(Course course) {
	if (course.courseNumber.size() > 0) {
		cout << course.courseNumber << " | " << course.courseName << endl;
		if (!empty(course.prereqs)) {
			cout << "Prerequisites: ";
			for (int i = 0; i < course.prereqs.size(); i++) {
				cout << course.prereqs.at(i);
				if (i < course.prereqs.size() - 1) {
					cout << ", ";
				}
			} cout << endl;
		}
		cout << endl;
	}
	else {
		cout << "Course not found." << endl;
	}
}

/*
* @param courses vector to print course info from 
*/
void printCourses(vector<Course> courses) {
	for (Course course : courses) {
		cout << course.courseNumber << ", " << course.courseName << endl;
	}
}

void printMenu() {
	cout << "Welcome to the course planner.\n" << endl;
	cout << "	1. Load Data Structure." << endl;
	cout << "	2. Print Course List." << endl;
	cout << "	3. Print Course." << endl;
	cout << "	9. Exit.\n" << endl;
	cout << "What would you like to do?\n" << endl;
}

/*
* Main method
*/
int main() {
	string filename;
	string courseNumber;
	string choice;
	vector<Course> courses;

	do {
		printMenu();
		cin >> choice;
		//Verify user input is a single number. Non-intergral input or multiple characters are 
		if (!isdigit(choice[0]) || choice.size() > 1) {
			choice = "0";
		}

		switch (stoi(choice)) {
		case 1:
			cout << "Please input the filename" << endl;
			cin >> filename;
			courses = loadData(filename);
			break;
		case 2:
			if (empty(courses)) {
				cout << "Courses not loaded." << endl;
				break;
			}
			printCourses(courses);
			break;
		case 3:
			if (empty(courses)) {
				cout << "Courses not loaded." << endl;
				break;
			}
			cout << "What course do you want to know about? (Case sensitive)" << endl;
			cin >> courseNumber;
			cout << endl;
			printCourse(searchCourse(courseNumber, courses));
			break;
		case 9:
			cout << "Exit" << endl;
			break;
		default:
			cout << "Please enter a valid choice." << endl;
		}
	} while (choice != "9");

	return 0;
}