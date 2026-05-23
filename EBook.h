#ifndef EBOOK_H
#define EBOOK_H

#include "Book.h"

class EBook : public Book {
private:
    string fileFormat;    // e.g. "PDF", "EPUB"
    string downloadLink;

public:
    EBook(const string& title,  const string& author,
          const string& genre,  const string& isbn,
          const string& fileFormat, const string& downloadLink);

    void   displayInfo()  const override;
    bool   borrowBook()         override; // always available - digital copy
    bool   isAvailable()  const override; // always true
    string getBookType()  const override; // "EBook"

    string getFileFormat()    const;
    string getDownloadLink()  const;
};

#endif
