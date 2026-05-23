#include "Faculty.h"
#include <iostream>

Faculty::Faculty(const string& name, const string& idNumber,
                 const string& contact, const string& email,
                 const string& designation)
    : Member(name, idNumber, contact, email), designation(designation) {}

int    Faculty::getBorrowLimit() const { return 10; }
int    Faculty::getDueDays()     const { return 30; }
string Faculty::getMemberType()  const { return "Faculty"; }
double Faculty::getFineRate()    const { return 5.0; } // Rs. 5 per day

string Faculty::getDesignation() const { return designation; }

void Faculty::displayInfo() const {
    Member::displayInfo();
    cout << "Designation: " << designation  << endl;
    cout << "Due Period : " << getDueDays()  << " days" << endl;
    cout << "Fine Rate  : Rs. " << getFineRate() << "/day" << endl;
    cout << "------------------------------" << endl;
}
