#include "Book.h"
#include <iostream>

Book::Book(const string& title, const string& author,
           const string& genre,  const string& isbn,
           int totalCopies)
    : title(title), author(author), genre(genre), isbn(isbn),
      totalCopies(totalCopies), availableCopies(totalCopies) {}

void Book::displayInfo() const {
    cout << "------------------------------"           << endl;
    cout << "Type     : " << getBookType()             << endl;
    cout << "Title    : " << title                     << endl;
    cout << "Author   : " << author                    << endl;
    cout << "Genre    : " << genre                     << endl;
    cout << "ISBN     : " << isbn                      << endl;
    cout << "Copies   : " << availableCopies
         << " / "         << totalCopies << " available" << endl;
    cout << "------------------------------"           << endl;
}

bool Book::borrowBook() {
    if (availableCopies > 0) {
        availableCopies--;
        return true;
    }
    cout << "No copies of \"" << title << "\" are currently available." << endl;
    return false;
}

void Book::returnBook() {
    if (availableCopies < totalCopies) {
        availableCopies++;
    }
}

bool   Book::isAvailable()  const { return availableCopies > 0; }
string Book::getBookType()  const { return "Physical"; }
string Book::getTitle()     const { return title; }
string Book::getAuthor()    const { return author; }
string Book::getGenre()     const { return genre; }
string Book::getISBN()      const { return isbn; }
int    Book::getTotalCopies()     const { return totalCopies; }
int    Book::getAvailableCopies() const { return availableCopies; }
void   Book::setAvailableCopies(int n)  { availableCopies = n; }
