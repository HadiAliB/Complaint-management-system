#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <map>
#include <functional>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;



class Complaint;
class Status;
class Assignment;

class ComplaintManagementSystem {

};


// Base class for departments
class Department {
	string name;
public:
	virtual string getName() const = 0;
};

// Derived classes for specific departments
class IT : public Department {
	string name;
public:
	string getName() const override {
		return "IT";
	}
};

class Accounts : public Department {
	string name;
public:
	string getName() const override {
		return "Accounts";
	}
};

class Admin : public Department {
	string name;
public:
	string getName() const override {
		return "Admin";
	}
};

class Status {
public:
	virtual  string getName() const = 0;
	virtual bool isOpen() const { return false; }
};

class New : public Status {
public:
	string getName() const override {
		return "New";
	}
	bool isOpen() const override {
		return true;
	}
};

class Assigned : public Status {
public:
	string getName() const override {
		return "Assigned";
	}
	bool isOpen() const override {
		return true;
	}
};

class Resolved : public Status {
public:
	string getName() const override {
		return "Resolved";
	}
	bool isOpen() const override {
		return true;
	}
};

class Closed : public Status {
public:
	string getName() const override {
		return "Closed";
	}
	bool isOpen() const override {
		return true;
	}
};


class User {
	string username;
	string password;
public:
	User() {

	}


	User(const string& username, const string& password) : username(username), password(password) {}

	string getUsername() const {
		return username;
	}

	void setUsername(const string& newUsername) {
		username = newUsername;
	}

	string getPassword() const {
		return password;
	}

	void setPassword(const string& newPassword) {
		password = newPassword;
	}

	virtual int getUserType() const = 0;
	virtual void displayWelcomeMessage(unordered_map<string, unique_ptr<User>>& userCredentials) = 0;
	virtual ~User() = default;
};



class Complaint {
	User* t1;
	Status* s1;
	Department* d1;
	string compla;
	int id;
	string timestamp;

	static int lastUsedComplaintID;

	static void updateLastUsedComplaintID() {
		ofstream outFile("lastUsedComplaintID.txt");
		if (outFile.is_open()) {
			outFile << lastUsedComplaintID;
			outFile.close();
		}
		else {
			cerr << "Unable to open file for writing.\n";
		}
	}

public:
	Complaint(User* T1, Status* S1, Department* D1, string c) {

		if (lastUsedComplaintID == 0) {
			ifstream inFile("lastUsedComplaintID.txt");
			if (inFile.is_open()) {
				inFile >> lastUsedComplaintID;
				inFile.close();
			}
			else {
				cerr << "Unable to open file for reading.\n";
			}
		}

		id = ++lastUsedComplaintID;

		t1 = T1;
		s1 = S1;
		d1 = D1;
		compla = c;
		time_t currentTime = time(nullptr);
		tm localTime;
		localtime_s(&localTime, &currentTime);

		ostringstream oss;
		oss << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
		timestamp = oss.str();

		updateLastUsedComplaintID();
	}

	void writeToComplaintFile() const {
		// string c = "complaint";
		// c += d1->getName();
		// c += ".txt";
		ofstream outFile("complaint.txt", ios::app); // Open file in append mode
		if (outFile.is_open()) {

			outFile << "id: " << id << "\n";
			outFile << "Username: " << t1->getUsername() << "\n";
			outFile << "Complaint: " << compla << "\n";
			outFile << "Department: " << d1->getName() << "\n";
			outFile << "Status: " << s1->getName() << "\n";
			outFile << "Timestamp: " << timestamp << "\n";
			outFile << "-----------------------------\n";
			outFile.close();
		}
		else {
			cerr << "Unable to open file for writing.\n";
		}
	}
};

int Complaint::lastUsedComplaintID = 0;


class Teacher : public User {

public:
	Teacher() {

	}
	Teacher(const string& username, const string& password) : User(username, password) {}
	void fileComplaint() {
		string Comp;
		string dept;
		Status* s1;
		Department* d1;
		cout << "What is your complaint: " << endl;
		cin.ignore();
		getline(cin, Comp);
		cout << "Which department is your complaint directed to: " << endl;
		cin >> dept;
		//cout << Comp << " " << dept << endl;
		if (dept == "IT") {
			d1 = new IT();
		}
		else if (dept == "Admin") {
			d1 = new Admin();

		}
		else if (dept == "Accounts") {
			d1 = new Accounts();
		}
		else {

			return;
		}
		s1 = new New();
		Complaint c1(this, s1, d1, Comp);
		c1.writeToComplaintFile();

	}
	void displayMyComplaints() const {
		ifstream inFile("complaint.txt");
		string line;
		string username = getUsername();
		bool printed = false;

		if (inFile.is_open())
		{

			while (getline(inFile, line))
			{
				if (line == "Username: " + username) {
					for (int i = 0; i < 4; i++)
					{
						cout << line << endl;
						getline(inFile, line);
					}
					cout << line << endl;
					printed = true;
				}
			}
			if (!printed)
			{
				cout << "No complaints found!" << endl;
			}
			inFile.close();
		}
		else {
			cerr << "Unable to open file for reading.\n";
		}
	}

	void recordFeedback() {

	}

	int getUserType() const override {
		return 1; // Teacher
	}
	void displayWelcomeMessage(unordered_map<string, unique_ptr<User>>& userCredentials) {
		system("CLS");
		cout << "\t\t\t\t\tComplaint Management System\n\n";
		cout << "Welcome, Teacher " << getUsername() << "!\n";
		cout << "File a Complaint - Press 1\n";
		cout << "Record Feedback - Press 2\n";
		cout << "View Complaints - Press 3\n";
		cout << "Exit - Press 0\n";

		int choice;
		cin >> choice;

		switch (choice) {
		case 1:
			fileComplaint();
			break;
		case 2:
			recordFeedback();
			break;
		case 3:
			displayMyComplaints();
			break;
		case 0:
			cout << "Exiting...\n";
			return;
		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}

		// Add more functionality specific to the Teacher interface
	}
};


class Employee : public User {

public:
	Employee() {

	}

	//view assigned complaints
	//mark assigned complaint as partially resolved
	Employee(const string& username, const string& password) : User(username, password) {}
	int getUserType() const override {
		return 2; // Employee
	}
	void displayWelcomeMessage(unordered_map<string, unique_ptr<User>>& userCredentials) {
		system("CLS");
		cout << "\t\t\t\t\tComplaint Management System\n\n";
		cout << "Welcome, Employee " << getUsername() << "!\n";
		// Add more functionality specific to the Teacher interface
	}
};


class Manager : public User {
	Department* department;
public:
	Manager() {

	}
	Manager(const string& username, const string& password, Department* dept) : User(username, password), department(dept) {}

	Department* getDepartment() const {
		return department;
	}

	void setDepartment(const string& dept) {
		if (dept == "IT") {
			department = new IT();
		}
		else if (dept == "Admin") {
			department = new Admin();

		}
		else if (dept == "Accounts") {
			department = new Accounts();
		}
		else {
			return;
		}
	}
	void displayComplaints() const {
		ifstream inFile("complaint.txt");
		string line;
		string username, complaintDetails, complaintDepartment, complaintStatus, timestamp;
		bool printed = false;
		int id;
		int i = 0;
		if (inFile.is_open()) {
			while (getline(inFile, line)) {
				//cout << line << endl;
				if (line.find("id: ") != string::npos) {
					id = stoi(line.substr(4));
				}

				getline(inFile, line);
				if (line.find("Username: ") != string::npos) {
					username = line.substr(10);
				}

				// Read complaint details
				getline(inFile, line);
				if (line.find("Complaint: ") != string::npos) {
					complaintDetails = line.substr(11);
				}

				// Read complaint department
				getline(inFile, line);
				if (line.find("Department: ") != string::npos) {
					complaintDepartment = line.substr(12);
				}

				// Read complaint status
				getline(inFile, line);
				if (line.find("Status: ") != string::npos) {
					complaintStatus = line.substr(8);
				}

				// Read timestamp
				getline(inFile, line);
				if (line.find("Timestamp: ") != string::npos) {
					timestamp = line.substr(11);
				}
				//if(i==0)
				getline(inFile, line);
				i++;
				// Check if the manager's department matches the complaint's department
				if (getDepartment()->getName() == complaintDepartment) {
					cout << "id: " << id << endl;
					cout << "Username: " << username << endl;
					cout << "Complaint: " << complaintDetails << endl;
					cout << "Department: " << complaintDepartment << endl;
					cout << "Status: " << complaintStatus << endl;
					cout << "Timestamp: " << timestamp << endl;
					cout << "-------------------------------\n";
					printed = true;
				}
			}

			if (!printed) {
				cout << "No complaints found!" << endl;
			}

			inFile.close();
		}
		else {
			cerr << "Unable to open file for reading.\n";
		}
	}

	void assignEmployees()
	{

		//int temp;
		//cout << "Enter complaint number" << endl;
		//cin >> temp;
		//if(temp > complaintNum)
		//{
		//	cout << "No such complaint exists" << endl;
		//	return;
		//}
		//int count = 0;
		//Employee* e[50];
		////Assignment ass
		//bool enter = true;
		//cout << "Enter employee name for assignment " << endl;
		//string name;
		//getline(cin, name);
		//ifstream inFile("employment.txt");
		//string line;
		//if (inFile.is_open())
		//{
		//	while (getline(inFile, line))
		//	{
		//		if((line.find("employee") != string::npos) && (line.find(department->getName()) != string::npos)&& (line.find(name) != string::npos))
		//		{
		//			string pass = "Nothing";
		//			e[count] = new Employee(name, pass);
		//			count++;
		//		}
		//	}
		//}
		//if(count > 0)
		//{
		//	//Assignment(e,count,&this,)
		//}
		//else
		//{
		//	cout << "No employees found" << endl;
		//}

	}

	void resolveComplaint() {

	}

	int getUserType() const override {
		return 3; // Manager
	}
	void displayWelcomeMessage(unordered_map<string, unique_ptr<User>>& userCredentials) {
		system("CLS");
		cout << "\t\t\t\t\tComplaint Management System\n\n";
		cout << "Welcome, Manager " << getUsername() << "!\n";
		cout << "Assign Employees - Press 1\n";
		cout << "Resolve a complaint - Press 2\n";
		cout << "View Complaints - Press 3\n";
		cout << "Exit - Press 0\n";

		int choice;
		cin >> choice;

		switch (choice) {
		case 1:
			assignEmployees();
			break;
		case 2:
			resolveComplaint();
			break;
		case 3:
			displayComplaints();
			break;
		case 0:
			cout << "Exiting...\n";
			return;
		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}
	}
};

class Adminstrator : public User {

private:
	static int userIDCounter;

	int generateUserID() {
		return userIDCounter++;

	}

	// Helper function to update a file after removing a user
	void updateFileAfterRemoval(const string& filename, const string& usernameToRemove) {
		ifstream inFile(filename);
		ofstream outFile("temp.txt");

		int userID;
		string username, password, userType, dept;
		if (filename == "employment.txt") {
			while (inFile >> userID >> dept >> userType) {
				if (username != usernameToRemove) {
					outFile << userID << " " << dept << " " << userType << "\n";
				}
			}
		}
		else {
			while (inFile >> userID >> username >> password >> userType) {
				if (username != usernameToRemove) {
					outFile << userID << " " << username << " " << password << " " << userType << "\n";
				}
			}
		}

		inFile.close();
		outFile.close();

		remove(filename.c_str());
		rename("temp.txt", filename.c_str());
	}
	void saveUserIDCounter() {
		// Save the userIDCounter to a file
		ofstream file("userIDCounter.txt");
		file << userIDCounter << endl;
		file.close();
	}

	void loadUserIDCounter() {
		// Load the userIDCounter from a file
		ifstream file("userIDCounter.txt");
		if (file.is_open()) {
			file >> userIDCounter;
			file.close();
		}
		else {
			// Handle the case where the file doesn't exist or cannot be opened
			userIDCounter = 2; // Start from 1000 if the file is not available
		}
	}

public:
	Adminstrator() {

	}
	Adminstrator(const string& username, const string& password) : User(username, password) {

	}
	void addEmployee(unordered_map<string, unique_ptr<User>>& userCredentials) {
		string newUsername, newPassword, department;
		cout << "Enter new employee username: ";
		cin >> newUsername;

		// Check if a user with the same username exists
		auto existingUserIt = userCredentials.find(newUsername);
		if (existingUserIt != userCredentials.end()) {
			cout << "A user with the username " << newUsername << " already exists.\n";
			return;
		}

		cout << "Enter new employee password: ";
		cin >> newPassword;
		cout << "Enter department for the new employee: ";
		cin >> department;

		unique_ptr<User> newEmployee(new Employee(newUsername, newPassword));
		userCredentials[newUsername] = move(newEmployee);

		// Update user.txt file
		ofstream userFile("user.txt", ios::app);
		userFile << generateUserID() << " " << newUsername << " " << newPassword << " Employee\n";
		userFile.close();

		// Update employment.txt file
		ofstream employmentFile("employment.txt", ios::app);
		employmentFile << newUsername << " " << department << " Employee\n";
		employmentFile.close();
	}

	void removeEmployee(unordered_map<string, unique_ptr<User>>& userCredentials) {
		string usernameToRemove;
		cout << "Enter username of the employee to remove: ";
		cin >> usernameToRemove;

		auto it = userCredentials.find(usernameToRemove);
		if (it != userCredentials.end()) {
			// Remove employee from the map
			userCredentials.erase(it);

			// Update user.txt file
			updateFileAfterRemoval("user.txt", usernameToRemove);
			updateFileAfterRemoval("employment.txt", usernameToRemove);

			cout << "Employee removed successfully.\n";
		}
		else {
			cout << "Employee not found.\n";
		}
	}

	void addManager(std::unordered_map<std::string, std::unique_ptr<User>>& userCredentials) {
		std::string newUsername, newPassword, departmentName;

		std::cout << "Enter new manager username: ";
		std::cin >> newUsername;

		// Check if a manager with the same username exists
		auto existingManagerIt = userCredentials.find(newUsername);
		if (existingManagerIt != userCredentials.end()) {
			std::cout << "A manager with the username " << newUsername << " already exists.\n";
			return;
		}

		std::cout << "Enter new manager password: ";
		std::cin >> newPassword;
		std::cout << "Enter department for the new manager (IT/Accounts/Admin): ";
		std::cin >> departmentName;

		for (const auto& user : userCredentials) {
			if (user.second->getUserType() == 3 /* Manager */) {
				auto manager = dynamic_cast<Manager*>(user.second.get());
				if (manager && manager->getDepartment()->getName() == departmentName) {
					std::cout << "A manager for the department " << departmentName << " already exists.\n";
					return;
				}
			}
		}

		// Check if a manager for the specified department already exists
		if (departmentName == "Admin" || departmentName == "Accounts" || departmentName == "IT")
		{
			auto newManager = std::make_unique<Manager>(newUsername, newPassword, nullptr);
			newManager->setDepartment(departmentName); // Set the department

			userCredentials[newUsername] = std::move(newManager);
		}
		else {
			cout << "Invalid Department\n";
			return;
		}
		// Update user.txt file
		ofstream userFile("user.txt", ios::app);
		userFile << generateUserID() << " " << newUsername << " " << newPassword << " Manager\n";
		userFile.close();

		// Update employment.txt file
		ofstream employmentFile("employment.txt", ios::app);
		employmentFile << newUsername << " " << departmentName << " Manager\n";
		employmentFile.close();

		cout << "Manager added successfully.\n";
	}

	void removeManager(unordered_map<string, unique_ptr<User>>& userCredentials) {
		string usernameToRemove;
		cout << "Enter username of the manager to remove: ";
		cin >> usernameToRemove;

		auto it = userCredentials.find(usernameToRemove);
		if (it != userCredentials.end()) {
			// Remove manager from the map
			userCredentials.erase(it);

			// Update user.txt file
			updateFileAfterRemoval("user.txt", usernameToRemove);
			updateFileAfterRemoval("employment.txt", usernameToRemove);

			cout << "Manager removed successfully.\n";
		}
		else {
			cout << "Manager not found.\n";
		}
	}

	void addTeacher(unordered_map<string, unique_ptr<User>>& userCredentials) {
		string newUsername, newPassword;
		cout << "Enter new teacher username: ";
		cin >> newUsername;

		// Check if a user with the same username exists
		auto existingUserIt = userCredentials.find(newUsername);
		if (existingUserIt != userCredentials.end()) {
			cout << "A user with the username " << newUsername << " already exists.\n";
			return;
		}

		cout << "Enter new teacher password: ";
		cin >> newPassword;

		unique_ptr<User> newTeacher(new Teacher(newUsername, newPassword));
		userCredentials[newUsername] = move(newTeacher);

		// Update user.txt file
		ofstream userFile("user.txt", ios::app);
		userFile << generateUserID() << " " << newUsername << " " << newPassword << " Teacher\n";
		userFile.close();

		cout << "Teacher added successfully.\n";
	}

	void removeTeacher(unordered_map<string, unique_ptr<User>>& userCredentials) {
		string usernameToRemove;
		cout << "Enter username of the teacher to remove: ";
		cin >> usernameToRemove;

		auto it = userCredentials.find(usernameToRemove);
		if (it != userCredentials.end()) {
			// Remove teacher from the map
			userCredentials.erase(it);

			// Update user.txt file
			updateFileAfterRemoval("user.txt", usernameToRemove);

			cout << "Teacher removed successfully.\n";
		}
		else {
			cout << "Teacher not found.\n";
		}
	}

	int getUserType() const override {
		return 4; // Administrator
	}
	void displayWelcomeMessage(unordered_map<string, unique_ptr<User>>& userCredentials) override {
		system("CLS");
		cout << "\t\t\t\t\tComplaint Management System\n\n";
		cout << "Welcome, Administrator " << getUsername() << "!\n";
		cout << "Add Employee-Press 1\n";
		cout << "Remove Employee-Press 2\n";
		cout << "Add Manager-Press 3\n";
		cout << "Remove Manager-Press 4\n";
		cout << "Add Teacher-Press 5\n";
		cout << "Remove Teacher-Press 6\n";
		cout << "Exit-Press 0\n";
		loadUserIDCounter();
		int choice;
		cin >> choice;
		switch (choice) {
		case 1:
			addEmployee(userCredentials);
			break;
		case 2:
			removeEmployee(userCredentials);
			break;
		case 3:
			addManager(userCredentials);
			break;
		case 4:
			removeManager(userCredentials);
			break;
		case 5:
			addTeacher(userCredentials);
			break;
		case 6:
			removeTeacher(userCredentials);
			break;
		case 0:
			// Exit or perform other actions
			cout << "Exiting...\n";
			return;
		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}
		saveUserIDCounter();
		// Add more functionality specific to the Teacher interface
	}
};
int Adminstrator::userIDCounter = 3;

class CampusDirector : public User {

public:
	CampusDirector() {

	}
	CampusDirector(const string& username, const string& password) : User(username, password) {}
	void viewComplaintsSummary() const {
		map<string, map<string, int>> departmentComplaintStatusCount;
		ifstream inFile("complaints.txt");
		string line;

		if (inFile.is_open()) {
			while (getline(inFile, line)) {
				if (line.find("Department: ") != string::npos) {
					string department = line.substr(12);
					// Read the status
					getline(inFile, line);
					string status = line.substr(8);
					// Increment the count for the department and status
					departmentComplaintStatusCount[department][status]++;
				}
			}
			cout << "Complaints Summary:\n";
			for (const auto& departmentEntry : departmentComplaintStatusCount) {
				const string& department = departmentEntry.first;
				const auto& statusCountMap = departmentEntry.second;
				cout << department << ":\n";
				for (const auto& statusEntry : statusCountMap) {
					const string& status = statusEntry.first;
					int count = statusEntry.second;
					cout << "  " << status << ": " << count << " complaints\n";
				}
			}
			inFile.close();
		}
		else {
			cerr << "Unable to open file for reading.\n";
		}
	}
	//GetDetails()
	int getUserType() const override {
		return 5; // Campus Director
	}
	void displayWelcomeMessage(unordered_map<string, unique_ptr<User>>& userCredentials) {
		system("CLS");
		cout << "\t\t\t\t\tComplaint Management System\n\n";
		cout << "Welcome, Campus Director " << getUsername() << "!\n";

		int choice;
		while (true) {
			cout << "Menu:\n";
			cout << "1. View Complaints Summary\n";
			cout << "2. View Complaints in a Period\n";
			cout << "0. Exit\n";
			cout << "Enter your choice: ";
			cin >> choice;

			switch (choice) {
			case 1:
				viewComplaintsSummary();
				break;
			case 2:
				viewComplaintsInPeriod();
				break;
			case 0:
				cout << "Exiting...\n";
				return;
			default:
				cout << "Invalid choice. Please try again.\n";
			}
		}
	}

	void viewComplaintsInPeriod() {
		std::string startDate, endDate;

		std::cout << "Enter start date (YYYY-MM-DD): ";
		std::cin >> startDate;
		std::cout << "Enter end date (YYYY-MM-DD): ";
		std::cin >> endDate;

		// Assuming the complaints are sorted by timestamp, you can filter them based on the period
		ifstream inFile("complaints.txt");
		std::string line;
		bool printed = false;
		bool check = false;
		int c = 0;
		bool x = true;
		std::streampos stampPos;
		if (inFile.is_open()) {
			stampPos = inFile.tellg();
			while (getline(inFile, line)) {
				if (c % 7 == 0 && !x) {
					stampPos = inFile.tellg();
				}
				x = false;
				if (line.find("Timestamp: ") != std::string::npos) {
					std::string complaintTimestamp = line.substr(11, 10);
					//cout << complaintTimestamp << endl;
					if (complaintTimestamp >= startDate && complaintTimestamp <= endDate) {
						check = true;
						// Display the complaint details
					}
				}
				if (check == true && c % 7 == 6) {
					inFile.seekg(stampPos);  // Move back to the position after Timestamp
					for (int i = 0; i < 6; i++) {
						getline(inFile, line);
						std::cout << line << std::endl;
					}
					/*getline(inFile, line);
					std::cout << line << std::endl;*/
					printed = true;
					check = false;
				}
				c++;
			}
			if (!printed) {
				std::cout << "No complaints found in the specified period." << std::endl;
			}
			inFile.close();
		}
		else {
			std::cerr << "Unable to open file for reading." << std::endl;
		}
	}
};

class Assignment {
	User* employee;
	User* m;
	Complaint* c;
	char* dt;
public:

	Assignment(User* e, int count, User* man, Complaint* com)
	{
		employee = e;
		m = man;
		c = com;
		//time_t now = time(0); // get current dat/time with respect to system  
		//dt = ctime(&now); // convert it into string  
	}


};


// Factory interface for creating users
class UserFactory {
public:
	virtual ~UserFactory() = default;
	virtual User* createUser() = 0;
};

// Concrete factory for creating Teacher instances
class TeacherFactory : public UserFactory {
public:
	User* createUser() override {
		return new Teacher();
	}
};

// Concrete factory for creating Employee instances
class EmployeeFactory : public UserFactory {
public:
	User* createUser() override {
		return new Employee();
	}
};


// Concrete factory for creating Manager instances
class ManagerFactory : public UserFactory {
public:
	User* createUser() override {
		return new Manager();
	}
};

// Concrete factory for creating Administrator instances
class AdministratorFactory : public UserFactory {
public:
	User* createUser() override {
		return new Adminstrator();
	}
};

// Concrete factory for creating CampusDirector instances
class CampusDirectorFactory : public UserFactory {
public:
	User* createUser() override {
		return new CampusDirector();
	}
};




class FileHandler {

};

// Function to initialize the user credentials hashtable from a file
void initializeUserCredentials(unordered_map<string, unique_ptr<User>>& userCredentials, unordered_map<string, UserFactory*>& userFactories) {
	ifstream file("user.txt");
	ifstream employmentFile("employment.txt");

	if (!file.is_open()) {
		cerr << "Error opening user file." << endl;
		exit(EXIT_FAILURE);
	}

	int userID;
	string username, password, userType, deptName;

	while (file >> userID >> username >> password >> userType) {
		auto factoryIt = userFactories.find(userType);

		if (factoryIt != userFactories.end()) {
			// Create the user using the appropriate factory
			unique_ptr<User> user(factoryIt->second->createUser());
			user->setUsername(username);
			user->setPassword(password);
			if (userType == "Manager") {
				// Read the department from the employment file
				if (employmentFile >> username >> deptName >> userType) {
					// Set the department for the manager
					auto manager = dynamic_cast<Manager*>(user.get());
					if (manager) {
						// Assume a manager has a setDepartment method
						manager->setDepartment(deptName); // You may need to modify this based on your implementation
					}
				}
			}
			userCredentials[username] = move(user);
			cout << username << endl;
		}
		else {
			cerr << "Unknown user type: " << userType << endl;
		}
	}
	employmentFile.close();
	file.close();
}
// Function to check credentials using the hashtable
int checkCredentials(const string& username, const string& password, const unordered_map<string, unique_ptr<User>>& userCredentials) {
	auto it = userCredentials.find(username);

	if (it != userCredentials.end()) {
		// User found, check password
		if (it->second->getPassword() == password) {
			//cout << it->second->getUserType() << " ";
			return it->second->getUserType();

		}
	}

	return -1; // Username or password not found
}


void LoginPage() {
	unordered_map<string, unique_ptr<User>> userCredentials;
	unordered_map<string, UserFactory*> userFactories;

	// Register factories for each user type
	userFactories["Teacher"] = new TeacherFactory();
	userFactories["Manager"] = new ManagerFactory();
	userFactories["Employee"] = new EmployeeFactory();
	userFactories["Administrator"] = new AdministratorFactory();
	userFactories["CampusDirector"] = new CampusDirectorFactory();

	cout << "\t\t\t\t\tComplaint Management System\n\n";
	initializeUserCredentials(userCredentials, userFactories);

	string username;
	string password;

	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;

	int userType = checkCredentials(username, password, userCredentials);

	if (userType == -1) {
		cout << "Invalid username or password. Please try again." << endl;
	}
	else {
		cout << "Login successful. User type: " << userType << endl;
		userCredentials[username]->displayWelcomeMessage(userCredentials);
		// Perform further actions based on the user type
	}

	// Smart pointers handle memory cleanup automatically
}

int main()
{
	LoginPage();
	//login interface
	//teacher interface
	//employee interface
	//manager interface
	//administrator interface
	//campus director interface
	//we have a single administrator, hard-coded.
	//the administrator has options to add/remove employees/managers/teachers


	return 0;
}
