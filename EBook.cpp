#include "EBook.h"
#include <iostream>

// EBooks have no physical copies so totalCopies is set to 999 (unlimited)
EBook::EBook(const string& title,  const string& author,
             const string& genre,  const string& isbn,
             const string& fileFormat, const string& downloadLink)
    : Book(title, author, genre, isbn, 999),
      fileFormat(fileFormat), downloadLink(downloadLink) {}

void EBook::displayInfo() const {
    cout << "------------------------------" << endl;
    cout << "Type     : EBook"               << endl;
    cout << "Title    : " << title           << endl;
    cout << "Author   : " << author          << endl;
    cout << "Genre    : " << genre           << endl;
    cout << "ISBN     : " << isbn            << endl;
    cout << "Format   : " << fileFormat      << endl;
    cout << "Download : " << downloadLink    << endl;
    cout << "Status   : Always Available (Digital)" << endl;
    cout << "------------------------------" << endl;
}

// Digital — borrowing just records the transaction, copies never run out
bool EBook::borrowBook() {
    return true;
}

bool   EBook::isAvailable() const { return true; }
string EBook::getBookType() const { return "EBook"; }

string EBook::getFileFormat()   const { return fileFormat; }
string EBook::getDownloadLink() const { return downloadLink; }
