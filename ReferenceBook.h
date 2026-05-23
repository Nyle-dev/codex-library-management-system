#ifndef REFERENCEBOOK_H
#define REFERENCEBOOK_H

#include "Book.h"

class ReferenceBook : public Book {
private:
    string section; // e.g. "Science", "Law", "Medical"

public:
    ReferenceBook(const string& title,  const string& author,
                  const string& genre,  const string& isbn,
                  int totalCopies,      const string& section);

    void   displayInfo() const override;
    bool   borrowBook()        override; // always blocked
    string getBookType() const override; // "Reference"

    string getSection()  const;
};

#endif
