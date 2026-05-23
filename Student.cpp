#include "Student.h"
#include <iostream>

Student::Student(const string& name, const string& idNumber,
                 const string& contact, const string& email,
                 const string& department)
    : Member(name, idNumber, contact, email), department(department) {}

int    Student::getBorrowLimit() const { return 3; }
int    Student::getDueDays()     const { return 14; }
string Student::getMemberType()  const { return "Student"; }
double Student::getFineRate()    const { return 10.0; } // Rs. 10 per day

string Student::getDepartment()  const { return department; }

void Student::displayInfo() const {
    Member::displayInfo(); // reuse base display
    cout << "Department: " << department  << endl;
    cout << "Due Period: " << getDueDays() << " days" << endl;
    cout << "Fine Rate : Rs. " << getFineRate() << "/day" << endl;
    cout << "------------------------------" << endl;
}
