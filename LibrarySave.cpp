// LibrarySave.cpp
// Handles saving all library data to disk and loading it back on startup.
// Data is stored in a /data folder as plain text files:
//   data/books.dat   — all books
//   data/members.dat — all members and librarians
//   data/issues.dat  — all issue records

#include "Library.h"
#include "Book.h"
#include "EBook.h"
#include "ReferenceBook.h"
#include "Member.h"
#include "Student.h"
#include "Faculty.h"
#include "Librarian.h"
#include "IssueRecord.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// ── Helper: split a line by delimiter ────────────────────────────────────────
static vector<string> splitLine(const string& line, char delim = '|') {
    vector<string> tokens;
    string token;
    istringstream ss(line);
    while (getline(ss, token, delim))
        tokens.push_back(token);
    return tokens;
}

// ── SAVE ──────────────────────────────────────────────────────────────────────
void Library::saveData() const {
    fs::create_directories("data");

    // ── Save books ────────────────────────────────────────────────────────────
    ofstream bf("data/books.dat");
    for (auto& b : books) {
        string type = b->getBookType();
        if (type == "EBook") {
            EBook* e = dynamic_cast<EBook*>(b);
            bf << "EBOOK|"
               << b->getTitle()       << "|"
               << b->getAuthor()      << "|"
               << b->getGenre()       << "|"
               << b->getISBN()        << "|"
               << e->getFileFormat()  << "|"
               << e->getDownloadLink()<< "\n";
        } else if (type == "Reference") {
            ReferenceBook* r = dynamic_cast<ReferenceBook*>(b);
            bf << "REFERENCE|"
               << b->getTitle()           << "|"
               << b->getAuthor()          << "|"
               << b->getGenre()           << "|"
               << b->getISBN()            << "|"
               << b->getTotalCopies()     << "|"
               << b->getAvailableCopies() << "|"
               << r->getSection()         << "\n";
        } else {
            bf << "PHYSICAL|"
               << b->getTitle()           << "|"
               << b->getAuthor()          << "|"
               << b->getGenre()           << "|"
               << b->getISBN()            << "|"
               << b->getTotalCopies()     << "|"
               << b->getAvailableCopies() << "\n";
        }
    }

    // ── Save members ──────────────────────────────────────────────────────────
    ofstream mf("data/members.dat");
    for (auto& m : members) {
        if (m->getMemberType() == "Student") {
            Student* s = dynamic_cast<Student*>(m);
            mf << "STUDENT|"
               << m->getName()             << "|"
               << m->getID()               << "|"
               << m->getContact()          << "|"
               << m->getEmail()            << "|"
               << s->getDepartment()       << "|"
               << m->getOutstandingFine()  << "\n";
        } else {
            Faculty* f = dynamic_cast<Faculty*>(m);
            mf << "FACULTY|"
               << m->getName()             << "|"
               << m->getID()               << "|"
               << m->getContact()          << "|"
               << m->getEmail()            << "|"
               << f->getDesignation()      << "|"
               << m->getOutstandingFine()  << "\n";
        }
    }
    for (auto& l : librarians) {
        mf << "LIBRARIAN|"
           << l->getName()       << "|"
           << l->getID()         << "|"
           << l->getContact()    << "|"
           << l->getEmail()      << "|"
           << l->getEmployeeID() << "\n";
    }

    // ── Save issue records ────────────────────────────────────────────────────
    ofstream rf("data/issues.dat");
    for (auto& rec : issueRecords) {
        rf << rec->getRecordID()                            << "|"
           << rec->getBook()->getISBN()                     << "|"
           << rec->getMember()->getID()                     << "|"
           << rec->getIssueDate()                           << "|"
           << rec->getDueDate()                             << "|"
           << rec->getReturnDate()                          << "|"
           << rec->getStatusString()                        << "|"
           << rec->getFineAmount()                          << "\n";
    }
}

// ── LOAD ──────────────────────────────────────────────────────────────────────
bool Library::loadData() {
    ifstream bf("data/books.dat");
    if (!bf.is_open()) return false; // No saved data exists yet

    string line;

    // ── Load books ────────────────────────────────────────────────────────────
    while (getline(bf, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.empty()) continue;
        try {
            if (t[0] == "PHYSICAL" && t.size() >= 7) {
                Book* b = new Book(t[1], t[2], t[3], t[4], stoi(t[5]));
                b->setAvailableCopies(stoi(t[6]));   // restore exact available count
                books.push_back(b);
            } else if (t[0] == "EBOOK" && t.size() >= 7) {
                books.push_back(new EBook(t[1], t[2], t[3], t[4], t[5], t[6]));
            } else if (t[0] == "REFERENCE" && t.size() >= 8) {
                books.push_back(new ReferenceBook(t[1], t[2], t[3], t[4],
                                                  stoi(t[5]), t[7]));
            }
        } catch (...) { continue; }
    }

    // ── Load members ──────────────────────────────────────────────────────────
    ifstream mf("data/members.dat");
    if (mf.is_open()) {
        while (getline(mf, line)) {
            if (line.empty()) continue;
            auto t = splitLine(line);
            if (t.empty()) continue;
            try {
                if (t[0] == "STUDENT" && t.size() >= 7) {
                    Student* s = new Student(t[1], t[2], t[3], t[4], t[5]);
                    double fine = stod(t[6]);
                    if (fine > 0) s->addFine(fine);
                    members.push_back(s);
                } else if (t[0] == "FACULTY" && t.size() >= 7) {
                    Faculty* f = new Faculty(t[1], t[2], t[3], t[4], t[5]);
                    double fine = stod(t[6]);
                    if (fine > 0) f->addFine(fine);
                    members.push_back(f);
                } else if (t[0] == "LIBRARIAN" && t.size() >= 6) {
                    librarians.push_back(new Librarian(t[1], t[2], t[3], t[4], t[5]));
                }
            } catch (...) { continue; }
        }
    }

    // ── Load issue records ────────────────────────────────────────────────────
    ifstream rf("data/issues.dat");
    if (rf.is_open()) {
        int maxRecNum = 0;

        while (getline(rf, line)) {
            if (line.empty()) continue;
            auto t = splitLine(line);
            if (t.size() < 8) continue;

            try {
                string      recID      = t[0];
                string      isbn       = t[1];
                string      memberID   = t[2];
                time_t      issueDate  = (time_t)stoll(t[3]);
                time_t      dueDate    = (time_t)stoll(t[4]);
                time_t      returnDate = (time_t)stoll(t[5]);
                string      statusStr  = t[6];
                double      fine       = stod(t[7]);

                Book*   book   = findBookByISBN(isbn);
                Member* member = findMemberByID(memberID);
                if (!book || !member) continue;

                IssueStatus status = IssueStatus::ACTIVE;
                if      (statusStr == "Returned") status = IssueStatus::RETURNED;
                else if (statusStr == "OVERDUE")  status = IssueStatus::OVERDUE;

                // Use the load constructor — restores exact dates and status
                IssueRecord* rec = new IssueRecord(book, member,
                                                   issueDate, dueDate, returnDate,
                                                   status, fine, recID);
                issueRecords.push_back(rec);

                // Restore member's active / history lists
                if (status == IssueStatus::ACTIVE || status == IssueStatus::OVERDUE) {
                    member->addActiveIssue(rec);
                } else {
                    // Returned: temporarily add then close so it lands in history
                    member->addActiveIssue(rec);
                    member->closeIssue(rec);
                }

                // Track highest record number to restore the counter
                try {
                    int n = stoi(recID.substr(4));
                    maxRecNum = max(maxRecNum, n);
                } catch (...) {}

            } catch (...) { continue; }
        }

        // Restore counter so new records continue from the right number
        IssueRecord::setCounter(maxRecNum + 1);
    }

    cout << "[✓] Saved data loaded — "
         << books.size()   << " books, "
         << members.size() << " members, "
         << issueRecords.size() << " issue records." << endl;
    return true;
}
