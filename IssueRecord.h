#ifndef ISSUERECORD_H
#define ISSUERECORD_H

#include <string>
#include <ctime>
using namespace std;

class Book;
class Member;

enum class IssueStatus { ACTIVE, RETURNED, OVERDUE };

class IssueRecord {
private:
    Book*       book;
    Member*     member;
    time_t      issueDate;
    time_t      dueDate;
    time_t      returnDate;
    double      fineAmount;
    IssueStatus status;
    string      recordID;     // unique ID e.g. "REC-001"

    static int  recordCounter; // auto-increments for each new record

public:
    // Normal constructor — used when issuing a book
    IssueRecord(Book* book, Member* member, int dueDays);

    // Load constructor — used when restoring from saved data
    IssueRecord(Book* book, Member* member,
                time_t issueDate, time_t dueDate, time_t returnDate,
                IssueStatus status, double fine, const string& recordID);

    // Restore counter after loading saved records
    static void setCounter(int n);

    void displayInfo() const;

    // Getters
    Book*       getBook()       const;
    Member*     getMember()     const;
    time_t      getIssueDate()  const;
    time_t      getDueDate()    const;
    time_t      getReturnDate() const;
    double      getFineAmount() const;
    IssueStatus getStatus()     const;
    string      getRecordID()   const;

    // Called when book is returned
    void markReturned(double fine);

    // Checks if past due date and updates status
    void updateStatus();

    // Returns days overdue (0 if not overdue)
    int getDaysOverdue() const;

    // Helpers
    static string formatDate(time_t t);
    string        getStatusString() const;
};

#endif
