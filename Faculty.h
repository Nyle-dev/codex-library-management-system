#ifndef FACULTY_H
#define FACULTY_H

#include "Member.h"

class Faculty : public Member {
private:
    string designation; // e.g. "Lecturer", "Professor"

public:
    Faculty(const string& name, const string& idNumber,
            const string& contact, const string& email,
            const string& designation);

    int    getBorrowLimit() const override;   // 10 books
    int    getDueDays()     const override;   // 30 days
    string getMemberType()  const override;   // "Faculty"
    double getFineRate()    const override;   // Rs. 5/day

    void   displayInfo()    const override;

    string getDesignation() const;
};

#endif
