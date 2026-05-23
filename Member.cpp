#include "Member.h"
#include "IssueRecord.h"
#include "Book.h"
#include <iostream>
#include <algorithm>

Member::Member(const string& name, const string& idNumber,
               const string& contact, const string& email)
    : Person(name, idNumber, contact, email), outstandingFine(0.0) {}

void Member::displayInfo() const {
    cout << "------------------------------" << endl;
    cout << "Type    : " << getMemberType() << endl;
    cout << "Name    : " << name            << endl;
    cout << "ID      : " << idNumber        << endl;
    cout << "Contact : " << contact         << endl;
    cout << "Email   : " << email           << endl;
    cout << "Borrowed: " << getCurrentBorrowCount()
         << " / " << getBorrowLimit()        << endl;
    cout << "Fine    : Rs. " << outstandingFine << endl;
    cout << "------------------------------" << endl;
}

bool Member::canBorrow() const {
    // Cannot borrow if at limit or has unpaid fines
    return (getCurrentBorrowCount() < getBorrowLimit()) && (outstandingFine == 0.0);
}

int Member::getCurrentBorrowCount() const {
    return (int)activeIssues.size();
}

void Member::addActiveIssue(IssueRecord* rec) {
    activeIssues.push_back(rec);
}

void Member::closeIssue(IssueRecord* rec) {
    auto it = find(activeIssues.begin(), activeIssues.end(), rec);
    if (it != activeIssues.end()) {
        activeIssues.erase(it);
        borrowHistory.push_back(rec);
    }
}

void Member::addFine(double amount) {
    outstandingFine += amount;
}

void Member::payFine(double amount) {
    outstandingFine -= amount;
    if (outstandingFine < 0) outstandingFine = 0;
}

double Member::getOutstandingFine() const {
    return outstandingFine;
}

const vector<IssueRecord*>& Member::getActiveIssues() const {
    return activeIssues;
}

const vector<IssueRecord*>& Member::getBorrowHistory() const {
    return borrowHistory;
}

void Member::viewBorrowedBooks() const {
    cout << "\n--- Currently Borrowed by " << name << " ---" << endl;
    if (activeIssues.empty()) {
        cout << "No books currently borrowed." << endl;
        return;
    }
    for (auto& rec : activeIssues) {
        rec->displayInfo();
    }
}

void Member::viewHistory() const {
    cout << "\n--- Borrow History of " << name << " ---" << endl;
    if (borrowHistory.empty()) {
        cout << "No borrow history found." << endl;
        return;
    }
    for (auto& rec : borrowHistory) {
        rec->displayInfo();
    }
}

void Member::viewFines() const {
    cout << "\n--- Fines for " << name << " ---" << endl;
    cout << "Outstanding Fine: Rs. " << outstandingFine << endl;
}
