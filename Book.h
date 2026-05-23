#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
protected:
    string title;
    string author;
    string genre;
    string isbn;
    int    totalCopies;
    int    availableCopies;

public:
    Book(const string& title, const string& author,
         const string& genre,  const string& isbn,
         int totalCopies);

    virtual void   displayInfo()    const;
    virtual bool   borrowBook();             // returns false if none available
    virtual void   returnBook();
    virtual bool   isAvailable()    const;
    virtual string getBookType()    const;   // "Physical", "EBook", "Reference"

    string getTitle()          const;
    string getAuthor()         const;
    string getGenre()          const;
    string getISBN()           const;
    int    getTotalCopies()    const;
    int    getAvailableCopies() const;
    void   setAvailableCopies(int n);  // used when loading saved data

    virtual ~Book() = default;
};

#endif
