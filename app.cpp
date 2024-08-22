#include	<iostream>
#include 	<fstream>
#include	<string>
#include	<cstdlib>
#include <iomanip>
#include	"List.h"
#include	"Student.h"

using namespace std;


bool CreateStuList(const char *, List *);
bool DeleteStudent(List *);
bool PrintList(List,int ); 
bool InsertExamResult(const char *, List *);
bool PrintStatistic(List);
bool FilterStudent(List, List *, char *, int, int) ;
bool UpdateIDandPhone(List *);
bool FindPotentialFirstClass(List, List *, char *);
int menu(List &list,List &list1,List &list2);




bool checkDuplicateID(const char tempID_char[], const List& list, Node*& passCur);
bool isValidSevenDigitNumber(const char* );
bool trimesterYearCompare(List* list, const char* id, int tri, int year);



int main() 
{

	List list, list1, list2;
	Student student;
	
	
	menu(list,list,list2);

	return 0;
}





//Function


bool CreateStuList(const char* filename, List* list)
{
	string line;
	Student student;
	ifstream file;
	Node* passCur;
	bool foundDuplicateID;

	file.open(filename);

	if (!file)
	{
		cout << "Error: Unable to open file." << endl;
		return false;
	}

	while (getline(file, line))
	{
	
		if (line.find("Student Id = ") != string::npos)//to find the prefixes  "student id=" in the get line
		{
			string tempID= (line.substr(line.find('=') + 2));//store id found into temporary id to check duplicate

			char tempID_char[8];//ID is 7digit plus 1 for null
			strcpy_s(tempID_char, tempID.c_str());//convert string into char for duplicateID comparing

			if (!checkDuplicateID(tempID_char, *list,passCur))
			{
				strncpy_s(student.id, line.substr(line.find('=') + 2).c_str(), sizeof(student.id));//copy the data behind the '='
				foundDuplicateID = true;//if no duplicate found continue to get the rest of the data
			}

			else
				foundDuplicateID = false;//if duplicate found skip to get the rest of the data
			
		}

		else if (foundDuplicateID&&line.find("Name = ") != string::npos)//will be skip if is duplicate id
		{
			strncpy_s(student.name, line.substr(line.find('=') + 2).c_str(), sizeof(student.name));
			
		}

		else if (foundDuplicateID&&line.find("course = ") != string::npos)
		{
			strncpy_s(student.course, line.substr(line.find('=') + 2).c_str(), sizeof(student.course));
			
		}

		else if (foundDuplicateID && line.find("Phone Number = ") != string::npos)
		{
			strncpy_s(student.phone_no, line.substr(line.find('=') + 2).c_str(), sizeof(student.phone_no));
			if (file)
				list->insert(student);//insert the info get from file into list
		}

	}

	return true;
}
	

bool checkDuplicateID(const char tempID_char[], const List& list,Node*&passCur)//to check for same ID in all nodes
{

	Node* current = list.head;
	while (current != nullptr) {
		if (strcmp(current->item.id, tempID_char) == 0) 
		{
			passCur = current;//to pass current pointer to other functons
			return true; // Duplicate found
		}
		current = current->next;
	}
	return false; // No duplicate found
}

bool DeleteStudent(List* list)
{
	Node* cur;
	cur = list->head;
	int count = 1;
	char tempID[10];

	cout << "Please enter the ID of student that you want to delete\n";
	cin >> tempID;
	while (cur != NULL)
	{

		if (strcmp(tempID,cur->item.id) == 0)//finding same id in list
		{
			list->remove(count);
			cout << "Student information successfully deleted\n";
			return true;
		}
		else
			count++;//record the count of list

		cur = cur->next;

	}
	if (cur == nullptr)
		cout << "There is no same id in the list\n";

	return false;
}

bool isValidSevenDigitNumber(const char* str) //to check is it a seven digit number entered by user
{
	if (strlen(str) != 7) {
		return false;//id eneterd not 7 digit return false
	}

	// Check if each character is a digit
	for (int i = 0; i < 7; ++i) {
		if (!isdigit(str[i])) {
			return false;
		}
	}

	// If all characters are digits and the length is 7, return true
	return true;
}

bool trimesterYearCompare(List* list, int tri, int year) 
{

	int i = 0;
	Node* cur = list->head;
	Exam exam;
	while (cur != nullptr) {
		if (tri == cur->item.exam[i].trimester && year == cur->item.exam[i].year)

			return true; // Duplicate found


		i++;
		cur = cur->next;
	}
	return false; // No duplicate found
}


bool PrintList(List list,int source)
{
	int i = 1;//to show number of student
	Node* cur;
	cout << "Select to print on\nscreen(1)\nfile(2)\nselection:";//choose to print on which source
	cin >> source;
	cout << endl;
	
		if (source == 1)//choose to print on screen
		{
			if (list.empty())//if no student info in the list return flase
				return false;

			cur = list.head;

			while (cur != NULL)
			{
				cout << "******************************************************STUDENT " << i << "******************************************************\n";
				cur->item.print(cout);//print student info
				cout << "\n";
				cout << "--------------------------------------------------PAST EXAM RESULT:--------------------------------------------------\n";
				if (cur->item.exam_cnt == 0)//if student havent take any exam
					cout << "THIS STUDENT HAVEN'T TAKEN ANY EXAM YET\n ";
				else
				{
					for (int j = 0; j < cur->item.exam_cnt; j++)//to print all the result
					{
						cur->item.exam[j].print(cout);
					}
					
				}
					
				cout << "\n******************************************************STUDENT" << i << "******************************************************\n\n\n\n\n";

				i++;
				

				cur = cur->next;
			}
			return true;
		}

		else if (source == 2)//choose to print in file
		{
			ofstream out("student_result.txt");
				
				if (list.empty())
					return false;

			cur = list.head;

			while (cur != NULL)
			{

				out<< "******************************************************STUDENT "<<i<<"******************************************************\n";
				cur->item.print(out);//outfile student info
				out << "\n";
				out<<"--------------------------------------------------PAST EXAM RESULT:--------------------------------------------------\n"; 
				if (cur->item.exam_cnt == 0)//if student havent take any exam
					out << "THIS STUDENT HAVEN'T TAKEN ANY EXAM YET\n ";
				else
					for (int j = 0; j < cur->item.exam_cnt; j++) 
					{
						cur->item.exam[j].print(out);
					}
				out << "\n******************************************************STUDENT"<<i<<"******************************************************\n\n\n\n\n";
				
				i++;
				cur = cur->next;
			}
			return true;

		}

}

bool UpdateIDandPhone(List* list)
{
	char tempID[10], newID[10], newPhone[10];
	char ans;
	int updateSelect;
	bool validID = false;
	Node* passCur = nullptr;//pointer that will carry the modified ID

	cout << "Please key in the ID needed to update\nID:";
	cin >> tempID;


	if (checkDuplicateID(tempID, *list, passCur))
	{
		cout << "ID founded\n";
		cout << "select (1) to update your ID\nSelect (2) to update your phone number\nSelection:";
		cin >> updateSelect;
		if (updateSelect == 1)
		{
			do 
			{
				cout << "please enter your new ID:";
				cin >> newID;
				if (isValidSevenDigitNumber(newID))
				{
					strcpy_s(passCur->item.id, newID);//modify old id using passCur pointer that is passed from the checkDuplicate function
					cout << "\nID succesfully updated";
					passCur->item.print(cout);//print out modified student info
					validID = true;
				}
				else
					cout << "Entered ID must be seven digit nummber\n";
			} while (!validID);//keep repeat if entered ID is not valid
		}

		else if (updateSelect == 2)
		{

			cout << "\nPlease enter your new phone number: ";//to get new number
			cin >> newPhone;

			if (newPhone[0] % 2 == 1)//to check if the first number is odd 
			{
				passCur->item.phone_no[0] = '0';
				passCur->item.phone_no[1] = '1';

			}
			else if (newPhone[0] % 2 == 0)//to check even number 
			{
				passCur->item.phone_no[0] = '0';
				passCur->item.phone_no[1] = '2';
			}

			for (int i = 2, j = 0; i<10&&j<10; i++, j++)//copying the new phone to the list 
			{                                        

				if (newPhone[j] != '-')//continue to replace old phone with new phone if there is no dash
					passCur->item.phone_no[i] = newPhone[j];
				else
					passCur->item.phone_no[i] = passCur->item.phone_no[i-1];//if dash founded,step back one digit to ensure the next digit is entered properly 
			}

			cout << "\nPhone number succesfully updated";
			passCur->item.print(cout);//print out modified student info

		}


	}
	else
		cout << "ID not found";

	return true;

}


bool InsertExamResult(const char* filename, List* list) {
	ifstream file(filename);
	if (!file) {
		cout << "Error: Unable to open file." << endl;
		return false;
	}

	Student student;
	Exam exam;
	Subject subject;

	string line;

	while (file >> line) 
	{
		Node* cur = list->head;
		int i = 0;
		strcpy_s(student.id, line.c_str());//get id in file

		bool studentFound = false;//to check if is it student id while getting line from file

		while (cur != NULL) {
			if (cur->item.compareID(student)) //check for same id in list
			{
				studentFound = true;
				i = cur->item.exam_cnt;
				break;
			}
			cur = cur->next;
		}

		if (!studentFound) 
		{
			if(isValidSevenDigitNumber(student.id))//check if the data get is id or not
			cout << "Error: Student with ID " << student.id << " not found in the list. Skipping." << endl;//confirm data get is ID ,ready to display 
			
			while (file >> line)
			{
				if (!checkDuplicateID(line.c_str(), *list, cur)) //to check if the ID is in list or not,if not then skip to next data
				{
					break; // Next student record found
				}
			}
			continue; // Continue with the next iteration of the loop
		}

		// Process the exam results for the current student
		file >> exam.trimester >> exam.year;
		if (trimesterYearCompare(list, student.id, exam.trimester, exam.year)) {//to check if is there any same trimester result has been key in
			cout << "Error: Exam result for trimester " << exam.trimester << " and year " << exam.year << " already exists for student with ID " << student.id << ". Skipping." << endl;

			// Skip reading the subjects for this exam
			int numOfSubjects;//get the number of subject 
			file >> numOfSubjects;
			for (int j = 0; j < numOfSubjects; j++) {//repeat this action according to how many subject taken this trimester
				string dummy;
				file >> dummy >> dummy >> dummy >> dummy; // Skip subject details as (sub code,sub name,credits,marks)
			}
			continue; // Continue with the next iteration of the loop
		}

		// Proceed with reading and storing the exam result
		cur->item.exam[i].trimester = exam.trimester;
		cur->item.exam[i].year = exam.year;

		file >> cur->item.exam[i].numOfSubjects;
		for (int j = 0; j < cur->item.exam[i].numOfSubjects; j++) {//key in the result of current student by(sub code,sub name,credits,marks)
			file >> cur->item.exam[i].sub[j].subject_code >> cur->item.exam[i].sub[j].subject_name;
			file >> cur->item.exam[i].sub[j].credit_hours >> cur->item.exam[i].sub[j].marks;
			cur->item.exam[i].calculateGPA();//calculate gpa
		}

		// Update exam count and calculate CGPA
		cur->item.exam_cnt = i;
		cur->item.exam_cnt++;
		cur->item.calculateCurrentCGPA();
	}

	return true;
}

bool trimesterYearCompare(List* list, const char* id, int tri, int year) {
	Node* cur = list->head;
	while (cur != nullptr) {
		if (strcmp(cur->item.id, id) == 0) {
			for (int i = 0; i < cur->item.exam_cnt; ++i) {
				if (cur->item.exam[i].trimester == tri && cur->item.exam[i].year == year) {
					return true; // Exam result for the same trimester and year exists
				}
			}
			break; // Found the student, no need to continue searching
		}
		cur = cur->next; // Move to the next student
	}
	return false; // No duplicate found
}

bool FilterStudent(List list, List* list2, char* course, int year, int totalcredit)
{
	char yearID[4]; // get the first 2 digits from student id to represent as year enrollment
	int source = 1;
	bool fullfill = false;//to check any student fullfill filter conditions

	if (list.empty()) {//check for empty in list 1
		cout << "List1 is empty." << endl;
		return false;
	}
	if (!list2->empty()) {//check if list2 is not empty
		cout << "List2 is not empty. Please ensure list2 is empty before calling FilterStudent." << endl;
		return false;
	}

	cout << "Filter selection\n";
	cout << "\nCourse:";
	cin >> course;
	cout << "\nEnroll year:";
	cin >> year;
	cout << "\nTotal Credits Earned:";
	cin >> totalcredit;

	for (int i = 0; course[i] != '\0'; i++) {//in case user enter lower case
		course[i] = toupper(course[i]);
	}

	Node* cur = list.head;
	while (cur != nullptr)
	{
		if ((strcmp(cur->item.course, course) == 0) && cur->item.totalCreditsEarned == totalcredit) //comparing if there is same course and credit in list
		{
			yearID[0] = cur->item.id[0]; // get first digit from list
			yearID[1] = cur->item.id[1]; // second digit

			if (yearID[0] == to_string(year)[2] && yearID[1] == to_string(year)[3]) //compare first 2 digit in ID with last 2 digits in year
			{
				list2->insert(cur->item); // store current student to list2
				fullfill = true;//at least one student fullfill filter conditions
			}
		}
		cur = cur->next;
	}

	if(fullfill)
	PrintList(*list2, source);// Print the contents of list2 after all students are inserted
	else
		cout<< "No students meet the filtering criteria." << endl;
	while(!list2->empty())//while list2 is not empty
	list2->remove(1);//continue to delete the first node until list2 is empty

	return true;
}

bool FindPotentialFirstClass(List list, List* list2, char* course)
{
	int source = 1;//default for PrintList function
	int pass = 0;//record trimester that gets gpa 3.75 or above and earned 12 credits
	bool potentialStu = false;//to determine if current student have first class potential
	int semCredit = 0;//credits earned in one trimester that have gpa 3.75 or above
	bool fullfill = false;//check if there is student have first class potential

	

	if (list.empty()) {//check for empty in list 1
		cout << "List1 is empty.\n" << endl;
		return false;
	}
	if (!list2->empty()) {//check if list2 is not empty
		cout << "List2 is not empty. Please ensure list2 is empty before calling FilterStudent.\n" << endl;
		return false;
	}

	Node* cur = list.head;
	while (cur != nullptr)
	{
		if (cur->item.exam_cnt < 3)//check if student took at least 3 exam or not
		{
			cur = cur->next;//skip to next student
			continue;//continue to next student
		}
		pass = 0;//initialize pass value for next student
		for (int i =0; i < 10;i++)
		{
			if (cur->item.exam[i].gpa < 3.5)
				break;//exit the checking gpa for every trimester loop
			else if (cur->item.exam[i].gpa >= 3.75)
			{
				semCredit = 0;//reset to 0 
				for (int j = 0; j < 6; j++)
				{
					semCredit += cur->item.exam[i].sub[j].credit_hours;//record total credit for one trimester
				}
				if (semCredit >= 12)
				{
					pass++;
					potentialStu = true;
				}
			}
		}
		if (potentialStu == true && pass >= 3)
		{
			list2->insert(cur->item); // store current student to list2
			fullfill = true;
		}
		cur = cur->next;
	}
	
	if(fullfill)
		PrintList(*list2, source);// Print the contents of list2 
	else
		cout << "There is no student in CS that has potential to get first class\n";
	while (!list2->empty())
		list2->remove(1);//continue to delete the first node until list2 is empty

	return true;
}

bool PrintStatistic(List list)
{
	Node* cur = list.head;
	double subSum = 0;
	double totalCH = 0;
	double averageSub = 0;
	double totalAverageSub = 0;
	double averageSubSem = 0;
	double averageCGPA = 0, totalCGPA = 0, averageCH = 0, averageCHSem = 0;
	int csCount = 0, iaCount = 0, ibCount = 0, cnCount = 0, ctCount = 0;

	if (list.empty())

		return false;

	while (cur != NULL)
	{
		subSum = 0;
		if (strcmp(cur->item.course, "CS")==0)//using str compare to keep track of number
			csCount++;
		else if (strcmp(cur->item.course, "IA")==0)
			iaCount++;
		else if (strcmp(cur->item.course, "IB")==0)
			ibCount++;
		else if (strcmp(cur->item.course, "CN")==0)
			cnCount++;
		else
			ctCount++;


		if (cur->item.exam_cnt > 0) {
			for (int i = 0; i < cur->item.exam_cnt; i++) {
				subSum += cur->item.exam[i].numOfSubjects;//loop to calculate total subject for one student using cur pointer
			}


			averageSubSem = subSum / cur->item.exam_cnt;//average subject taken per semester for one student

			averageCHSem = cur->item.totalCreditsEarned / cur->item.exam_cnt;//average credit hour per semester
		}
		else {
			averageSubSem = 0;//to prevent identifier divide by 0 if the student havent attempt exam
			averageCHSem = 0;
		}
		totalCGPA += cur->item.current_cgpa;//sum up all the cgpa
		totalCH += averageCHSem;//total credit hour earned for all student
		totalAverageSub += averageSubSem;//to sum all the average subject per semester for all student
		cur = cur->next;//next student
	}
	int totalStudent = list.size();
	averageSub = totalAverageSub / totalStudent;//divide by student count
	averageCGPA = totalCGPA / totalStudent;
	averageCH = totalCH / totalStudent;


	cout << "Total Students: " << totalStudent << endl;
	cout << "\t CS Students - " << csCount << endl;
	cout << "\t IA Students - " << iaCount << endl;
	cout << "\t IB Students - " << ibCount << endl;
	cout << "\t CN Students - " << cnCount << endl;
	cout << "\t CT Students - " << ctCount << endl;
	cout << "Average CGPA: " << averageCGPA << endl;
	cout << "Average Subject Taken Per Semester: " << averageSub << endl;
	cout << "Average Credits Earned Per Semester: " << averageCH << endl;


	return true;
}

int menu(List &list,List &list1,List &list2)
{
	int choice, year=0, totalcredit=0;
	int source = 1;
	char* course=new char[3];
	bool menuRepeat = true;

	do
	{
		cout << "Menu" << endl;
		cout << "1. Create student list" << endl
			<< "2. Delete Student" << endl
			<< "3. Print student list " << endl
			<< "4. Insert exam result " << endl
			<< "5. Print Exam Statistic" << endl
			<< "6. Filter Student " << endl
			<< "7. Update Student’s ID and Phone " << endl
			<< "8. Find Potential First Class Student" << endl
			<< "9. Exit" << endl;

		cout << "Enter your selection :";
		cin >> choice;


		if (cin.fail()) // If input is not an integer
		{
			cout << "Invalid input. Please enter an integer." << endl;
			cin.clear(); // Clear error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
			continue; // Restart the loop
		}

		switch (choice)
		{
		case 1:
			CreateStuList("student.txt", &list); 
			cout << "Student list successfully created\n";
			system("pause");
			break;
		case 2:
			DeleteStudent(&list);
			system("pause");
			break;
		case 3:
			
			PrintList(list, source); 
			system("pause");
			break;
		case 4:
			if (InsertExamResult("exam.txt", &list))
				cout << "Student's exam result successfully inserted\n";
			else
				cout << "Student not in list";
			system("pause");
			break;
		case 5:
			PrintStatistic(list);
			if (PrintStatistic(list))
				cout << "Print successfully\n";
			else
				cout << "List is empty\n";
			system("pause");
			break;
		case 6:
			FilterStudent(list, &list2, course, year, totalcredit);
			system("pause");
			break;
		case 7:
			UpdateIDandPhone(&list); 
			system("pause");
			break;
		case 8:
			FindPotentialFirstClass(list,&list2,course);
			system("pause");
			break;
		case 9:
			return 0;
		default:
			cout << "Your selection must be between 1 to 9 " << endl;
		}
	} while (menuRepeat == true);

	return 0; // Indicate successful completion
}
