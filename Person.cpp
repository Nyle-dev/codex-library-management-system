#include "Person.h"
#include <iostream>

Person::Person(const string& name, const string& idNumber,
               const string& contact, const string& email)
    : name(name), idNumber(idNumber), contact(contact), email(email) {}

string Person::getName()    const { return name; }
string Person::getID()      const { return idNumber; }
string Person::getContact() const { return contact; }
string Person::getEmail()   const { return email; }
