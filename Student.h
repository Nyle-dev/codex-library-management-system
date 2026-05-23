#ifndef STUDENT_H
#define STUDENT_H

#include "Member.h"

class Student : public Member {
private:
    string department;

public:
    Student(const string& name, const string& idNumber,
            const string& contact, const string& email,
            const string& department);

    int    getBorrowLimit() const override;   // 3 books
    int    getDueDays()     const override;   // 14 days
    string getMemberType()  const override;   // "Student"
    double getFineRate()    const override;   // Rs. 10/day

    void   displayInfo()    const override;

    string getDepartment()  const;
};

#endif
