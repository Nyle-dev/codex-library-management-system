#include "IssueRecord.h"
#include "Book.h"
#include "Member.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int IssueRecord::recordCounter = 1;

IssueRecord::IssueRecord(Book* book, Member* member, int dueDays)
    : book(book), member(member), returnDate(0), fineAmount(0.0),
      status(IssueStatus::ACTIVE) {

    issueDate = time(nullptr); // current time

    // due date = now + dueDays converted to seconds
    dueDate = issueDate + (dueDays * 24 * 60 * 60);

    // Build record ID: REC-001, REC-002 ...
    ostringstream oss;
    oss << "REC-" << setw(3) << setfill('0') << recordCounter++;
    recordID = oss.str();
}

// ── Load constructor ─────────────────────────────────────────────────────
IssueRecord::IssueRecord(Book* book, Member* member,
                         time_t issueDate, time_t dueDate, time_t returnDate,
                         IssueStatus status, double fine, const string& recordID)
    : book(book), member(member),
      issueDate(issueDate), dueDate(dueDate), returnDate(returnDate),
      fineAmount(fine), status(status), recordID(recordID) {}

void IssueRecord::setCounter(int n) { recordCounter = n; }

void IssueRecord::displayInfo() const {
    cout << "  Record  : " << recordID                      << endl;
    cout << "  Book    : " << book->getTitle()              << endl;
    cout << "  Member  : " << member->getName()
         << " (" << member->getID() << ")"                  << endl;
    cout << "  Issued  : " << formatDate(issueDate)         << endl;
    cout << "  Due     : " << formatDate(dueDate)           << endl;
    if (status == IssueStatus::RETURNED) {
        cout << "  Returned: " << formatDate(returnDate)    << endl;
        cout << "  Fine    : Rs. " << fineAmount            << endl;
    }
    cout << "  Status  : " << getStatusString()             << endl;
    cout << "  ----------"                                  << endl;
}

void IssueRecord::markReturned(double fine) {
    returnDate = time(nullptr);
    fineAmount = fine;
    status     = IssueStatus::RETURNED;
}

void IssueRecord::updateStatus() {
    if (status == IssueStatus::RETURNED) return;
    time_t now = time(nullptr);
    if (now > dueDate)
        status = IssueStatus::OVERDUE;
}

int IssueRecord::getDaysOverdue() const {
    if (status == IssueStatus::RETURNED) return 0;
    time_t now = time(nullptr);
    if (now <= dueDate) return 0;
    return (int)((now - dueDate) / (24 * 60 * 60));
}

string IssueRecord::formatDate(time_t t) {
    if (t == 0) return "N/A";
    struct tm* tm_info = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm_info);
    return string(buf);
}

string IssueRecord::getStatusString() const {
    switch (status) {
        case IssueStatus::ACTIVE:   return "Active";
        case IssueStatus::RETURNED: return "Returned";
        case IssueStatus::OVERDUE:  return "OVERDUE";
        default:                    return "Unknown";
    }
}

Book*       IssueRecord::getBook()       const { return book; }
Member*     IssueRecord::getMember()     const { return member; }
time_t      IssueRecord::getIssueDate()  const { return issueDate; }
time_t      IssueRecord::getDueDate()    const { return dueDate; }
time_t      IssueRecord::getReturnDate() const { return returnDate; }
double      IssueRecord::getFineAmount() const { return fineAmount; }
IssueStatus IssueRecord::getStatus()     const { return status; }
string      IssueRecord::getRecordID()   const { return recordID; }
