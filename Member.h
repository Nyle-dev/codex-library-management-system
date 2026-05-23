#ifndef MEMBER_H
#define MEMBER_H

#include "Person.h"
#include <vector>
#include <string>
using namespace std;

// Forward declaration - IssueRecord and Book are defined later
class Book;
class IssueRecord;

class Member : public Person {
protected:
    vector<IssueRecord*> activeIssues;   // currently borrowed
    vector<IssueRecord*> borrowHistory;  // all past records
    double outstandingFine;

public:
    Member(const string& name, const string& idNumber,
           const string& contact, const string& email);

    // Pure virtuals - Student and Faculty must define these
    virtual int    getBorrowLimit() const = 0;
    virtual int    getDueDays()     const = 0;
    virtual string getMemberType()  const = 0;
    virtual double getFineRate()    const = 0;

    void displayInfo() const override;

    bool   canBorrow()                      const;
    int    getCurrentBorrowCount()          const;
    void   addActiveIssue(IssueRecord* rec);
    void   closeIssue(IssueRecord* rec);     // moves from active to history
    void   addFine(double amount);
    void   payFine(double amount);

    double getOutstandingFine()             const;
    const  vector<IssueRecord*>& getActiveIssues()  const;
    const  vector<IssueRecord*>& getBorrowHistory() const;

    void   viewBorrowedBooks()  const;
    void   viewHistory()        const;
    void   viewFines()          const;

    virtual ~Member() = default;
};

#endif
