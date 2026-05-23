#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "SearchEngine.h"
#include "FineCalculator.h"
using namespace std;

class Book;
class Member;
class Librarian;
class IssueRecord;

class Library {
private:
    vector<Book*>        books;
    vector<Member*>      members;
    vector<Librarian*>   librarians;
    vector<IssueRecord*> issueRecords;   // every transaction ever made

    SearchEngine    searchEngine;
    FineCalculator  fineCalculator;

    string libraryName;

    // Internal lookup helpers
    Book*   findBookByISBN (const string& isbn)  const;
    Book*   findBookByTitle(const string& title) const;
    Member* findMemberByID (const string& id)    const;

public:
    Library(const string& name);

    // ── Book Management ───────────────────────────────────────────────────────
    void addBook   (Book* book);
    void removeBook(const string& isbn);
    void viewAllBooks()       const;
    void viewAvailableBooks() const;

    // ── Member Management ─────────────────────────────────────────────────────
    void addMember      (Member* member);
    void addLibrarian   (Librarian* librarian);
    void removeMember   (const string& memberID);
    void viewAllMembers () const;
    void viewMember     (const string& memberID) const;

    // ── Issue / Return ────────────────────────────────────────────────────────
    void issueBook (const string& isbn, const string& memberID);
    void returnBook(const string& isbn, const string& memberID);
    void viewAllIssues()   const;
    void viewOverdue()     const;

    // ── Search ────────────────────────────────────────────────────────────────
    void searchBooks  (const string& query, const string& field) const;
    void searchMembers(const string& query)                      const;

    // ── Fines ─────────────────────────────────────────────────────────────────
    void processFines()                         ;
    void payFine(const string& memberID, double amount);
    void addFine(const string& memberID, double amount); // used for seeding demo data

    // ── Reports / Dashboard ───────────────────────────────────────────────────
    void generateReport() const;

    // ── Accessor for FineCalculator (needs full record list) ──────────────────
    const vector<IssueRecord*>& getAllIssueRecords() const;

    // 25002500 JSON API methods (used by GUI server) 25002500250025002500250025002500250025002500250025002500250025002500250025002500250025002500250025002500
    string jsonBooks()                                              const;
    string jsonMembers()                                            const;
    string jsonIssues()                                             const;
    string jsonReport()                                             const;
    string jsonSearchBooks(const string& query, const string& field)const;
    string jsonIssueBook  (const string& isbn,  const string& mid)      ;
    string jsonReturnBook (const string& isbn,  const string& mid)      ;
    string jsonAddBook    (const string& type,  const string& title,
                           const string& author,const string& genre,
                           const string& isbn,  int copies,
                           const string& extra1,const string& extra2)   ;
    string jsonAddMember  (const string& type,  const string& name,
                           const string& id,    const string& contact,
                           const string& email, const string& extra)    ;
    string jsonProcessFines()                                           ;
    string jsonPayFine    (const string& memberID, double amount)       ;
    string jsonRemoveMember(const string& memberID)                     ;
    string jsonMemberDetail(const string& memberID)                     const;

    // ── Persistence ───────────────────────────────────────────────────────────
    void saveData() const;   // saves everything to data/ folder
    bool loadData();         // loads from data/ folder, returns false if no data

    ~Library();
};

#endif
