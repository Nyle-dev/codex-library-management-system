#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <vector>
#include <string>
using namespace std;

class Book;
class Member;

class SearchEngine {
public:
    // Book searches - all return matching results
    vector<Book*> searchByTitle      (const vector<Book*>& books, const string& query) const;
    vector<Book*> searchByAuthor     (const vector<Book*>& books, const string& query) const;
    vector<Book*> searchByGenre      (const vector<Book*>& books, const string& query) const;
    vector<Book*> searchByISBN       (const vector<Book*>& books, const string& query) const;
    vector<Book*> searchAvailableOnly(const vector<Book*>& books)                      const;

    // Member searches
    vector<Member*> searchMemberByName(const vector<Member*>& members, const string& query) const;
    vector<Member*> searchMemberByID  (const vector<Member*>& members, const string& id)    const;

    // Prints results cleanly
    void displayBookResults  (const vector<Book*>&   results) const;
    void displayMemberResults(const vector<Member*>& results) const;

private:
    // Case-insensitive comparison helper
    bool containsIgnoreCase(const string& source, const string& query) const;
};

#endif
