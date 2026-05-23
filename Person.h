#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class Person {
protected:
    string name;
    string idNumber;
    string contact;
    string email;

public:
    Person(const string& name, const string& idNumber,
           const string& contact, const string& email);

    virtual void displayInfo() const = 0; // pure virtual - forces every subclass to implement it

    string getName()    const;
    string getID()      const;
    string getContact() const;
    string getEmail()   const;

    virtual ~Person() = default;
};

#endif
