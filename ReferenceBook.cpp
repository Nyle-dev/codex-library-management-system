#include "ReferenceBook.h"
#include <iostream>

ReferenceBook::ReferenceBook(const string& title,  const string& author,
                             const string& genre,  const string& isbn,
                             int totalCopies,      const string& section)
    : Book(title, author, genre, isbn, totalCopies), section(section) {}

void ReferenceBook::displayInfo() const {
    cout << "------------------------------"             << endl;
    cout << "Type     : Reference Book"                  << endl;
    cout << "Title    : " << title                       << endl;
    cout << "Author   : " << author                      << endl;
    cout << "Genre    : " << genre                       << endl;
    cout << "ISBN     : " << isbn                        << endl;
    cout << "Section  : " << section                     << endl;
    cout << "Copies   : " << totalCopies                 << endl;
    cout << "Status   : In-Library Use Only (Cannot Borrow)" << endl;
    cout << "------------------------------"             << endl;
}

// Blocked entirely - reference books cannot leave the library
bool ReferenceBook::borrowBook() {
    cout << "\"" << title << "\" is a reference book and cannot be borrowed." << endl;
    return false;
}

string ReferenceBook::getBookType() const { return "Reference"; }
string ReferenceBook::getSection()  const { return section; }
