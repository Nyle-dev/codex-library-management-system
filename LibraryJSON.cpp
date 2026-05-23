// LibraryJSON.cpp
// JSON serialization methods for the GUI server
// These are separate from Library.cpp to keep things clean

#include "Library.h"
#include "Book.h"
#include "EBook.h"
#include "ReferenceBook.h"
#include "Member.h"
#include "Student.h"
#include "Faculty.h"
#include "Librarian.h"
#include "IssueRecord.h"
#include <sstream>
#include <iostream>
#include <algorithm>

// ── Escape quotes in strings for safe JSON ────────────────────────────────
static string je(const string& s) {
    string out;
    for (char c : s) {
        if      (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else                out += c;
    }
    return out;
}

// ── Books JSON ────────────────────────────────────────────────────────────
string Library::jsonBooks() const {
    ostringstream o;
    o << "{\"success\":true,\"books\":[";
    for (size_t i = 0; i < books.size(); i++) {
        Book* b = books[i];
        o << "{"
          << "\"title\":\""           << je(b->getTitle())           << "\","
          << "\"author\":\""          << je(b->getAuthor())          << "\","
          << "\"genre\":\""           << je(b->getGenre())           << "\","
          << "\"isbn\":\""            << je(b->getISBN())            << "\","
          << "\"type\":\""            << je(b->getBookType())        << "\","
          << "\"totalCopies\":"       << b->getTotalCopies()         << ","
          << "\"availableCopies\":"   << b->getAvailableCopies()     << ","
          << "\"available\":"         << (b->isAvailable() ? "true" : "false")
          << "}";
        if (i + 1 < books.size()) o << ",";
    }
    o << "]}";
    return o.str();
}

// ── Members JSON ──────────────────────────────────────────────────────────
string Library::jsonMembers() const {
    ostringstream o;
    o << "{\"success\":true,\"members\":[";
    for (size_t i = 0; i < members.size(); i++) {
        Member* m = members[i];
        o << "{"
          << "\"name\":\""        << je(m->getName())          << "\","
          << "\"id\":\""          << je(m->getID())            << "\","
          << "\"type\":\""        << je(m->getMemberType())    << "\","
          << "\"contact\":\""     << je(m->getContact())       << "\","
          << "\"email\":\""       << je(m->getEmail())         << "\","
          << "\"borrowCount\":"   << m->getCurrentBorrowCount()<< ","
          << "\"borrowLimit\":"   << m->getBorrowLimit()       << ","
          << "\"fine\":"          << m->getOutstandingFine()
          << "}";
        if (i + 1 < members.size()) o << ",";
    }
    o << "]}";
    return o.str();
}

// ── Issues JSON ───────────────────────────────────────────────────────────
string Library::jsonIssues() const {
    ostringstream o;
    o << "{\"success\":true,\"issues\":[";
    for (size_t i = 0; i < issueRecords.size(); i++) {
        IssueRecord* r = issueRecords[i];
        const_cast<IssueRecord*>(r)->updateStatus();
        o << "{"
          << "\"recordId\":\""   << je(r->getRecordID())                     << "\","
          << "\"bookTitle\":\""  << je(r->getBook()->getTitle())              << "\","
          << "\"bookIsbn\":\""   << je(r->getBook()->getISBN())               << "\","
          << "\"memberName\":\"" << je(r->getMember()->getName())             << "\","
          << "\"memberId\":\""   << je(r->getMember()->getID())               << "\","
          << "\"issueDate\":\""  << IssueRecord::formatDate(r->getIssueDate())<< "\","
          << "\"dueDate\":\""    << IssueRecord::formatDate(r->getDueDate())  << "\","
          << "\"status\":\""     << je(r->getStatusString())                  << "\","
          << "\"daysOverdue\":"  << r->getDaysOverdue()                       << ","
          << "\"fine\":"         << r->getFineAmount()
          << "}";
        if (i + 1 < issueRecords.size()) o << ",";
    }
    o << "]}";
    return o.str();
}

// ── Report JSON ───────────────────────────────────────────────────────────
string Library::jsonReport() const {
    int totalBooks = 0, totalCopies = 0, availableCopies = 0;
    int activeIssues = 0, overdueIssues = 0;
    int students = 0, faculty = 0;
    double totalFines = 0.0;

    for (auto& b : books) {
        totalBooks++;
        if (b->getBookType() != "EBook") {
            totalCopies     += b->getTotalCopies();
            availableCopies += b->getAvailableCopies();
        }
    }

    for (auto& rec : issueRecords) {
        const_cast<IssueRecord*>(rec)->updateStatus();
        if (rec->getStatus() == IssueStatus::ACTIVE ||
            rec->getStatus() == IssueStatus::OVERDUE) activeIssues++;
        if (rec->getStatus() == IssueStatus::OVERDUE) overdueIssues++;
        totalFines += rec->getFineAmount();
    }

    for (auto& m : members) {
        totalFines += m->getOutstandingFine();
        if (m->getMemberType() == "Student") students++;
        else if (m->getMemberType() == "Faculty") faculty++;
    }

    ostringstream o;
    o << "{"
      << "\"success\":true,"
      << "\"totalBooks\":"      << totalBooks       << ","
      << "\"totalCopies\":"     << totalCopies      << ","
      << "\"availableCopies\":" << availableCopies  << ","
      << "\"totalMembers\":"    << members.size()   << ","
      << "\"students\":"        << students         << ","
      << "\"faculty\":"         << faculty          << ","
      << "\"librarians\":"      << librarians.size()<< ","
      << "\"activeIssues\":"    << activeIssues     << ","
      << "\"overdueIssues\":"   << overdueIssues    << ","
      << "\"totalFines\":"      << totalFines
      << "}";
    return o.str();
}

// ── Search JSON ───────────────────────────────────────────────────────────
string Library::jsonSearchBooks(const string& query, const string& field) const {
    vector<Book*> results;
    if      (field == "title" )     results = searchEngine.searchByTitle (books, query);
    else if (field == "author")     results = searchEngine.searchByAuthor(books, query);
    else if (field == "genre" )     results = searchEngine.searchByGenre (books, query);
    else if (field == "isbn"  )     results = searchEngine.searchByISBN  (books, query);
    else if (field == "available")  results = searchEngine.searchAvailableOnly(books);

    ostringstream o;
    o << "{\"success\":true,\"books\":[";
    for (size_t i = 0; i < results.size(); i++) {
        Book* b = results[i];
        o << "{"
          << "\"title\":\""         << je(b->getTitle())       << "\","
          << "\"author\":\""        << je(b->getAuthor())      << "\","
          << "\"genre\":\""         << je(b->getGenre())       << "\","
          << "\"isbn\":\""          << je(b->getISBN())        << "\","
          << "\"type\":\""          << je(b->getBookType())    << "\","
          << "\"availableCopies\":" << b->getAvailableCopies() << ","
          << "\"totalCopies\":"     << b->getTotalCopies()     << ","
          << "\"available\":"       << (b->isAvailable() ? "true" : "false")
          << "}";
        if (i + 1 < results.size()) o << ",";
    }
    o << "]}";
    return o.str();
}

// ── Issue Book JSON ───────────────────────────────────────────────────────
string Library::jsonIssueBook(const string& isbn, const string& mid) {
    // Capture cout output by redirecting, or just replicate logic with return
    Book*   book   = findBookByISBN(isbn);
    Member* member = findMemberByID(mid);

    if (!book)   return "{\"success\":false,\"message\":\"Book with ISBN '" + isbn + "' not found.\"}";
    if (!member) return "{\"success\":false,\"message\":\"Member ID '" + mid + "' not found.\"}";

    if (!member->canBorrow()) {
        if (member->getCurrentBorrowCount() >= member->getBorrowLimit())
            return "{\"success\":false,\"message\":\"" + member->getName() +
                   " has reached their borrow limit of " +
                   to_string(member->getBorrowLimit()) + " books.\"}";
        else
            return "{\"success\":false,\"message\":\"" + member->getName() +
                   " has outstanding fines of Rs. " +
                   to_string((int)member->getOutstandingFine()) +
                   ". Please clear fines before borrowing.\"}";
    }

    if (!book->borrowBook())
        return "{\"success\":false,\"message\":\"No copies of '" + book->getTitle() + "' are currently available.\"}";

    IssueRecord* rec = new IssueRecord(book, member, member->getDueDays());
    issueRecords.push_back(rec);
    member->addActiveIssue(rec);

    ostringstream o;
    o << "{\"success\":true,\"message\":\"Successfully issued '" << je(book->getTitle())
      << "' to " << je(member->getName()) << ".\","
      << "\"recordId\":\""  << je(rec->getRecordID()) << "\","
      << "\"dueDate\":\""   << IssueRecord::formatDate(rec->getDueDate()) << "\"}";
    return o.str();
}

// ── Return Book JSON ──────────────────────────────────────────────────────
string Library::jsonReturnBook(const string& isbn, const string& mid) {
    Member* member = findMemberByID(mid);
    if (!member) return "{\"success\":false,\"message\":\"Member ID '" + mid + "' not found.\"}";

    IssueRecord* targetRec = nullptr;
    for (auto& rec : member->getActiveIssues()) {
        if (rec->getBook()->getISBN() == isbn) { targetRec = rec; break; }
    }

    if (!targetRec)
        return "{\"success\":false,\"message\":\"No active issue found for this book under " +
               member->getName() + ".\"}";

    double fine = fineCalculator.calculateFine(targetRec);
    targetRec->getBook()->returnBook();
    targetRec->markReturned(fine);
    member->closeIssue(targetRec);
    if (fine > 0) member->addFine(fine);

    ostringstream o;
    o << "{\"success\":true,\"message\":\"";
    if (fine > 0)
        o << "'" << je(targetRec->getBook()->getTitle()) << "' returned. Fine of Rs. "
          << fine << " applied to " << je(member->getName()) << ".";
    else
        o << "'" << je(targetRec->getBook()->getTitle()) << "' returned by "
          << je(member->getName()) << " — on time, no fine.";
    o << "\",\"fine\":" << fine << "}";
    return o.str();
}

// ── Add Book JSON ─────────────────────────────────────────────────────────
string Library::jsonAddBook(const string& type, const string& title,
                             const string& author, const string& genre,
                             const string& isbn, int copies,
                             const string& extra1, const string& extra2) {
    if (title.empty() || author.empty() || isbn.empty())
        return "{\"success\":false,\"message\":\"Title, Author and ISBN are required.\"}";

    if (findBookByISBN(isbn))
        return "{\"success\":false,\"message\":\"A book with this ISBN already exists.\"}";

    if (type == "ebook")
        books.push_back(new EBook(title, author, genre, isbn, extra1, extra2));
    else if (type == "reference")
        books.push_back(new ReferenceBook(title, author, genre, isbn, copies, extra1));
    else
        books.push_back(new Book(title, author, genre, isbn, copies));

    return "{\"success\":true,\"message\":\"Book '" + je(title) + "' added successfully.\"}";
}

// ── Add Member JSON ───────────────────────────────────────────────────────
string Library::jsonAddMember(const string& type, const string& name,
                               const string& id, const string& contact,
                               const string& email, const string& extra) {
    if (name.empty() || id.empty())
        return "{\"success\":false,\"message\":\"Name and ID are required.\"}";

    if (findMemberByID(id))
        return "{\"success\":false,\"message\":\"Member ID '" + id + "' already exists.\"}";

    if (type == "faculty")
        members.push_back(new Faculty(name, id, contact, email, extra));
    else if (type == "librarian") {
        librarians.push_back(new Librarian(name, id, contact, email, extra));
        return "{\"success\":true,\"message\":\"Librarian '" + je(name) + "' registered.\"}";
    } else
        members.push_back(new Student(name, id, contact, email, extra));

    return "{\"success\":true,\"message\":\"" + je(name) + " registered successfully.\"}";
}

// ── Process Fines JSON ────────────────────────────────────────────────────
string Library::jsonProcessFines() {
    int count = 0;
    for (auto& rec : issueRecords) {
        rec->updateStatus();
        if (rec->getStatus() == IssueStatus::OVERDUE) {
            double fine = fineCalculator.calculateFine(rec);
            if (fine > 0) { rec->getMember()->addFine(fine); count++; }
        }
    }
    return "{\"success\":true,\"message\":\"Fines processed. " +
           to_string(count) + " overdue record(s) found.\"}";
}

// ── Pay Fine JSON ─────────────────────────────────────────────────────────
string Library::jsonPayFine(const string& memberID, double amount) {
    Member* member = findMemberByID(memberID);
    if (!member) return "{\"success\":false,\"message\":\"Member ID '" + memberID + "' not found.\"}";
    if (member->getOutstandingFine() == 0)
        return "{\"success\":false,\"message\":\"" + member->getName() + " has no outstanding fines.\"}";

    member->payFine(amount);
    ostringstream o;
    o << "{\"success\":true,\"message\":\"Rs. " << amount << " paid by "
      << je(member->getName()) << ". Remaining: Rs. "
      << member->getOutstandingFine() << ".\"}";
    return o.str();
}

// ── Remove Member JSON ────────────────────────────────────────────────────
string Library::jsonRemoveMember(const string& memberID) {
    auto it = find_if(members.begin(), members.end(),
                      [&memberID](Member* m){ return m->getID() == memberID; });
    if (it == members.end())
        return "{\"success\":false,\"message\":\"Member ID not found.\"}";

    if ((*it)->getCurrentBorrowCount() > 0)
        return "{\"success\":false,\"message\":\"Cannot remove " + je((*it)->getName()) +
               " — they still have " + to_string((*it)->getCurrentBorrowCount()) +
               " book(s) borrowed. Return all books first.\"}";

    string name = (*it)->getName();
    delete *it;
    members.erase(it);
    return "{\"success\":true,\"message\":\"Member '" + je(name) + "' removed successfully.\"}";
}

// ── Member Detail JSON ────────────────────────────────────────────────────
string Library::jsonMemberDetail(const string& memberID) const {
    Member* m = findMemberByID(memberID);
    if (!m) return "{\"success\":false,\"message\":\"Member not found.\"}";

    ostringstream o;
    o << "{"
      << "\"success\":true,"
      << "\"name\":\""       << je(m->getName())           << "\","
      << "\"id\":\""         << je(m->getID())             << "\","
      << "\"type\":\""       << je(m->getMemberType())     << "\","
      << "\"contact\":\""    << je(m->getContact())        << "\","
      << "\"email\":\""      << je(m->getEmail())          << "\","
      << "\"borrowCount\":"  << m->getCurrentBorrowCount() << ","
      << "\"borrowLimit\":"  << m->getBorrowLimit()        << ","
      << "\"fine\":"         << m->getOutstandingFine()    << ","
      << "\"activeBooks\":[";

    const auto& active = m->getActiveIssues();
    for (size_t i = 0; i < active.size(); i++) {
        o << "{"
          << "\"title\":\""     << je(active[i]->getBook()->getTitle())               << "\","
          << "\"isbn\":\""      << je(active[i]->getBook()->getISBN())                << "\","
          << "\"issueDate\":\"" << IssueRecord::formatDate(active[i]->getIssueDate()) << "\","
          << "\"dueDate\":\""   << IssueRecord::formatDate(active[i]->getDueDate())   << "\","
          << "\"status\":\""    << je(active[i]->getStatusString())                   << "\""
          << "}";
        if (i + 1 < active.size()) o << ",";
    }
    o << "]}";
    return o.str();
}
