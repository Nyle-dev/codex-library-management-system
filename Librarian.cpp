#include "Librarian.h"
#include <iostream>

Librarian::Librarian(const string& name, const string& idNumber,
                     const string& contact, const string& email,
                     const string& employeeID)
    : Person(name, idNumber, contact, email), employeeID(employeeID) {}

void Librarian::displayInfo() const {
    cout << "------------------------------" << endl;
    cout << "Type       : Librarian"         << endl;
    cout << "Name       : " << name          << endl;
    cout << "ID         : " << idNumber      << endl;
    cout << "Employee ID: " << employeeID    << endl;
    cout << "Contact    : " << contact       << endl;
    cout << "Email      : " << email         << endl;
    cout << "Access     : Admin"             << endl;
    cout << "------------------------------" << endl;
}

string Librarian::getEmployeeID()  const { return employeeID; }
bool   Librarian::hasAdminAccess() const { return true; }
