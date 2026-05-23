#include "Library.h"
#include "Book.h"
#include "Member.h"
#include "Librarian.h"
#include "IssueRecord.h"
#include "Student.h"
#include "Faculty.h"
#include <iostream>
#include <algorithm>

// ── Constructor / Destructor ──────────────────────────────────────────────────

Library::Library(const string& name) : libraryName(name) {}

Library::~Library() {
    for (auto& b  : books)        delete b;
    for (auto& m  : members)      delete m;
    for (auto& l  : librarians)   delete l;
    for (auto& r  : issueRecords) delete r;
}

// ── Internal helpers ──────────────────────────────────────────────────────────

Book* Library::findBookByISBN(const string& isbn) const {
    for (auto& b : books)
        if (b->getISBN() == isbn) return b;
    return nullptr;
}

Book* Library::findBookByTitle(const string& title) const {
    for (auto& b : books)
        if (b->getTitle() == title) return b;
    return nullptr;
}

Member* Library::findMemberByID(const string& id) const {
    for (auto& m : members)
        if (m->getID() == id) return m;
    return nullptr;
}

// ── Book Management ───────────────────────────────────────────────────────────

void Library::addBook(Book* book) {
    books.push_back(book);
    cout << "[+] Book added: \"" << book->getTitle()
         << "\" (" << book->getBookType() << ")" << endl;
}

void Library::removeBook(const string& isbn) {
    auto it = find_if(books.begin(), books.end(),
                      [&isbn](Book* b){ return b->getISBN() == isbn; });
    if (it == books.end()) {
        cout << "[-] Book with ISBN " << isbn << " not found." << endl;
        return;
    }
    cout << "[-] Removed book: \"" << (*it)->getTitle() << "\"" << endl;
    delete *it;
    books.erase(it);
}

void Library::viewAllBooks() const {
    cout << "\n===== All Books in " << libraryName
         << " (" << books.size() << " total) =====" << endl;
    if (books.empty()) { cout << "  No books registered." << endl; return; }
    for (auto& b : books) b->displayInfo();
}

void Library::viewAvailableBooks() const {
    cout << "\n===== Available Books =====" << endl;
    int count = 0;
    for (auto& b : books) {
        if (b->isAvailable()) { b->displayInfo(); count++; }
    }
    if (count == 0) cout << "  No books currently available." << endl;
}

// ── Member Management ─────────────────────────────────────────────────────────

void Library::addMember(Member* member) {
    members.push_back(member);
    cout << "[+] Member added: " << member->getName()
         << " (" << member->getMemberType() << ", ID: "
         << member->getID() << ")" << endl;
}

void Library::addLibrarian(Librarian* librarian) {
    librarians.push_back(librarian);
    cout << "[+] Librarian added: " << librarian->getName()
         << " (EmpID: " << librarian->getEmployeeID() << ")" << endl;
}

void Library::removeMember(const string& memberID) {
    auto it = find_if(members.begin(), members.end(),
                      [&memberID](Member* m){ return m->getID() == memberID; });
    if (it == members.end()) {
        cout << "[-] Member ID " << memberID << " not found." << endl;
        return;
    }
    if ((*it)->getCurrentBorrowCount() > 0) {
        cout << "[-] Cannot remove " << (*it)->getName()
             << " — they still have borrowed books." << endl;
        return;
    }
    cout << "[-] Removed member: " << (*it)->getName() << endl;
    delete *it;
    members.erase(it);
}

void Library::viewAllMembers() const {
    cout << "\n===== Registered Members (" << members.size() << ") =====" << endl;
    if (members.empty()) { cout << "  No members registered." << endl; return; }
    for (auto& m : members) m->displayInfo();
}

void Library::viewMember(const string& memberID) const {
    Member* m = findMemberByID(memberID);
    if (!m) { cout << "Member ID " << memberID << " not found." << endl; return; }
    m->displayInfo();
    m->viewBorrowedBooks();
    m->viewFines();
}

// ── Issue / Return ────────────────────────────────────────────────────────────

void Library::issueBook(const string& isbn, const string& memberID) {
    Book*   book   = findBookByISBN(isbn);
    Member* member = findMemberByID(memberID);

    if (!book) {
        cout << "[!] Book with ISBN \"" << isbn << "\" not found." << endl;
        return;
    }
    if (!member) {
        cout << "[!] Member ID \"" << memberID << "\" not found." << endl;
        return;
    }
    if (!member->canBorrow()) {
        if (member->getCurrentBorrowCount() >= member->getBorrowLimit())
            cout << "[!] " << member->getName()
                 << " has reached their borrow limit of "
                 << member->getBorrowLimit() << " books." << endl;
        else
            cout << "[!] " << member->getName()
                 << " has outstanding fines. Please clear Rs. "
                 << member->getOutstandingFine() << " before borrowing." << endl;
        return;
    }
    if (!book->borrowBook()) {
        // borrowBook() already prints the unavailability message
        return;
    }

    // Create the transaction record
    IssueRecord* rec = new IssueRecord(book, member, member->getDueDays());
    issueRecords.push_back(rec);
    member->addActiveIssue(rec);

    cout << "[✓] Issued \"" << book->getTitle() << "\" to "
         << member->getName() << endl;
    cout << "    Record ID : " << rec->getRecordID()              << endl;
    cout << "    Issue Date: " << IssueRecord::formatDate(rec->getIssueDate()) << endl;
    cout << "    Due Date  : " << IssueRecord::formatDate(rec->getDueDate())   << endl;
}

void Library::returnBook(const string& isbn, const string& memberID) {
    Member* member = findMemberByID(memberID);
    if (!member) {
        cout << "[!] Member ID \"" << memberID << "\" not found." << endl;
        return;
    }

    // Find the active issue record for this book + member
    IssueRecord* targetRec = nullptr;
    for (auto& rec : member->getActiveIssues()) {
        if (rec->getBook()->getISBN() == isbn) {
            targetRec = rec;
            break;
        }
    }

    if (!targetRec) {
        cout << "[!] No active issue found for ISBN \"" << isbn
             << "\" under member " << member->getName() << endl;
        return;
    }

    // Calculate fine before marking returned
    double fine = fineCalculator.calculateFine(targetRec);

    targetRec->getBook()->returnBook();
    targetRec->markReturned(fine);
    member->closeIssue(targetRec);

    if (fine > 0) {
        member->addFine(fine);
        cout << "[✓] \"" << targetRec->getBook()->getTitle()
             << "\" returned by " << member->getName() << endl;
        cout << "    Days Overdue: " << targetRec->getDaysOverdue() << endl;
        cout << "    Fine Applied: Rs. " << fine << endl;
    } else {
        cout << "[✓] \"" << targetRec->getBook()->getTitle()
             << "\" returned by " << member->getName()
             << " — on time, no fine." << endl;
    }
}

void Library::viewAllIssues() const {
    cout << "\n===== All Issue Records (" << issueRecords.size() << ") =====" << endl;
    if (issueRecords.empty()) { cout << "  No records found." << endl; return; }
    for (auto& rec : issueRecords) rec->displayInfo();
}

void Library::viewOverdue() const {
    cout << "\n===== Overdue Issues =====" << endl;
    int count = 0;
    for (auto& rec : issueRecords) {
        const_cast<IssueRecord*>(rec)->updateStatus();
        if (rec->getStatus() == IssueStatus::OVERDUE) {
            rec->displayInfo();
            count++;
        }
    }
    if (count == 0) cout << "  No overdue records." << endl;
}

// ── Search ────────────────────────────────────────────────────────────────────

void Library::searchBooks(const string& query, const string& field) const {
    cout << "\n===== Search Results =====" << endl;
    vector<Book*> results;

    if      (field == "title" ) results = searchEngine.searchByTitle (books, query);
    else if (field == "author") results = searchEngine.searchByAuthor(books, query);
    else if (field == "genre" ) results = searchEngine.searchByGenre (books, query);
    else if (field == "isbn"  ) results = searchEngine.searchByISBN  (books, query);
    else if (field == "available") results = searchEngine.searchAvailableOnly(books);
    else {
        cout << "  Unknown search field. Use: title, author, genre, isbn, available" << endl;
        return;
    }
    searchEngine.displayBookResults(results);
}

void Library::searchMembers(const string& query) const {
    cout << "\n===== Member Search Results =====" << endl;
    // Try ID first, then name
    auto results = searchEngine.searchMemberByID(members, query);
    if (results.empty())
        results = searchEngine.searchMemberByName(members, query);
    searchEngine.displayMemberResults(results);
}

// ── Fines ─────────────────────────────────────────────────────────────────────

void Library::processFines() {
    fineCalculator.processAllFines(*this);
}

void Library::addFine(const string& memberID, double amount) {
    Member* member = findMemberByID(memberID);
    if (member) member->addFine(amount);
}

void Library::payFine(const string& memberID, double amount) {
    Member* member = findMemberByID(memberID);
    if (!member) {
        cout << "[!] Member ID \"" << memberID << "\" not found." << endl;
        return;
    }
    if (member->getOutstandingFine() == 0) {
        cout << "    " << member->getName() << " has no outstanding fines." << endl;
        return;
    }
    member->payFine(amount);
    cout << "[✓] Rs. " << amount << " fine paid by " << member->getName() << endl;
    cout << "    Remaining Fine: Rs. " << member->getOutstandingFine() << endl;
}

// ── Reports / Dashboard ───────────────────────────────────────────────────────

void Library::generateReport() const {
    int totalBooks      = 0, availableBooks  = 0;
    int totalCopies     = 0, availableCopies = 0;
    int activeIssues    = 0, overdueIssues   = 0;
    double totalFines   = 0.0;

    for (auto& b : books) {
        totalBooks++;
        totalCopies     += b->getTotalCopies();
        availableCopies += b->getAvailableCopies();
        if (b->isAvailable()) availableBooks++;
    }

    for (auto& rec : issueRecords) {
        const_cast<IssueRecord*>(rec)->updateStatus();
        if (rec->getStatus() == IssueStatus::ACTIVE ||
            rec->getStatus() == IssueStatus::OVERDUE) {
            activeIssues++;
        }
        if (rec->getStatus() == IssueStatus::OVERDUE) overdueIssues++;
        totalFines += rec->getFineAmount();
    }

    for (auto& m : members)
        totalFines += m->getOutstandingFine();

    cout << "\n========================================" << endl;
    cout << "        LIBRARY REPORT — " << libraryName  << endl;
    cout << "========================================" << endl;
    cout << "  Total Book Titles   : " << totalBooks      << endl;
    cout << "  Total Copies        : " << totalCopies     << endl;
    cout << "  Available Copies    : " << availableCopies << endl;
    cout << "  Registered Members  : " << members.size()  << endl;
    cout << "  Librarians          : " << librarians.size()<< endl;
    cout << "  Active Issues       : " << activeIssues    << endl;
    cout << "  Overdue Issues      : " << overdueIssues   << endl;
    cout << "  Total Fines (Rs.)   : " << totalFines      << endl;
    cout << "========================================\n"  << endl;
}

const vector<IssueRecord*>& Library::getAllIssueRecords() const {
    return issueRecords;
}
