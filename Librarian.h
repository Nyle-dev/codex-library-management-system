#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "Person.h"

class Librarian : public Person {
private:
    string employeeID;

public:
    Librarian(const string& name, const string& idNumber,
              const string& contact, const string& email,
              const string& employeeID);

    void   displayInfo()   const override;
    string getEmployeeID() const;
    bool   hasAdminAccess() const; // always true for librarian
};

#endif
