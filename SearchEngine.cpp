#include "SearchEngine.h"
#include "Book.h"
#include "Member.h"
#include <iostream>
#include <algorithm>
#include <cctype>

// ── private helper ────────────────────────────────────────────────────────────
bool SearchEngine::containsIgnoreCase(const string& source, const string& query) const {
    string src = source, qry = query;
    transform(src.begin(), src.end(), src.begin(), ::tolower);
    transform(qry.begin(), qry.end(), qry.begin(), ::tolower);
    return src.find(qry) != string::npos;
}

// ── Book searches ─────────────────────────────────────────────────────────────
vector<Book*> SearchEngine::searchByTitle(const vector<Book*>& books,
                                          const string& query) const {
    vector<Book*> results;
    for (auto& b : books)
        if (containsIgnoreCase(b->getTitle(), query))
            results.push_back(b);
    return results;
}

vector<Book*> SearchEngine::searchByAuthor(const vector<Book*>& books,
                                           const string& query) const {
    vector<Book*> results;
    for (auto& b : books)
        if (containsIgnoreCase(b->getAuthor(), query))
            results.push_back(b);
    return results;
}

vector<Book*> SearchEngine::searchByGenre(const vector<Book*>& books,
                                          const string& query) const {
    vector<Book*> results;
    for (auto& b : books)
        if (containsIgnoreCase(b->getGenre(), query))
            results.push_back(b);
    return results;
}

vector<Book*> SearchEngine::searchByISBN(const vector<Book*>& books,
                                         const string& query) const {
    vector<Book*> results;
    for (auto& b : books)
        if (b->getISBN() == query)
            results.push_back(b);
    return results;
}

vector<Book*> SearchEngine::searchAvailableOnly(const vector<Book*>& books) const {
    vector<Book*> results;
    for (auto& b : books)
        if (b->isAvailable())
            results.push_back(b);
    return results;
}

// ── Member searches ───────────────────────────────────────────────────────────
vector<Member*> SearchEngine::searchMemberByName(const vector<Member*>& members,
                                                 const string& query) const {
    vector<Member*> results;
    for (auto& m : members)
        if (containsIgnoreCase(m->getName(), query))
            results.push_back(m);
    return results;
}

vector<Member*> SearchEngine::searchMemberByID(const vector<Member*>& members,
                                               const string& id) const {
    vector<Member*> results;
    for (auto& m : members)
        if (m->getID() == id)
            results.push_back(m);
    return results;
}

// ── Display helpers ───────────────────────────────────────────────────────────
void SearchEngine::displayBookResults(const vector<Book*>& results) const {
    if (results.empty()) {
        cout << "  No books found matching your search." << endl;
        return;
    }
    cout << "  Found " << results.size() << " result(s):" << endl;
    for (auto& b : results)
        b->displayInfo();
}

void SearchEngine::displayMemberResults(const vector<Member*>& results) const {
    if (results.empty()) {
        cout << "  No members found matching your search." << endl;
        return;
    }
    cout << "  Found " << results.size() << " result(s):" << endl;
    for (auto& m : results)
        m->displayInfo();
}
