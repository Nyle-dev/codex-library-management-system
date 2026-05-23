#include <iostream>
#include <limits>
#include "Library.h"
#include "Book.h"
#include "EBook.h"
#include "ReferenceBook.h"
#include "Student.h"
#include "Faculty.h"
#include "Librarian.h"

using namespace std;

// ── Helpers ───────────────────────────────────────────────────────────────────

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printHeader(const string& title) {
    cout << "\n╔══════════════════════════════════════════╗" << endl;
    cout << "║  " << title;
    int padding = 41 - (int)title.length();
    for (int i = 0; i < padding; i++) cout << " ";
    cout << "║" << endl;
    cout << "╚══════════════════════════════════════════╝" << endl;
}

int getChoice(int min, int max) {
    int choice;
    while (true) {
        cout << "Enter choice: ";
        if (cin >> choice && choice >= min && choice <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Invalid input. Enter a number between "
             << min << " and " << max << ".\n";
    }
}

// ── Sub-menus ─────────────────────────────────────────────────────────────────

void bookManagementMenu(Library& lib) {
    while (true) {
        printHeader("BOOK MANAGEMENT");
        cout << "  1. Add Physical Book\n";
        cout << "  2. Add EBook\n";
        cout << "  3. Add Reference Book\n";
        cout << "  4. Remove Book (by ISBN)\n";
        cout << "  5. View All Books\n";
        cout << "  6. View Available Books Only\n";
        cout << "  0. Back\n";

        int choice = getChoice(0, 6);
        if (choice == 0) break;

        string title, author, genre, isbn, format, link, section;
        int copies;

        switch (choice) {
        case 1:
            printHeader("ADD PHYSICAL BOOK");
            cout << "Title  : "; getline(cin, title);
            cout << "Author : "; getline(cin, author);
            cout << "Genre  : "; getline(cin, genre);
            cout << "ISBN   : "; getline(cin, isbn);
            cout << "Copies : "; cin >> copies;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            lib.addBook(new Book(title, author, genre, isbn, copies));
            break;

        case 2:
            printHeader("ADD EBOOK");
            cout << "Title          : "; getline(cin, title);
            cout << "Author         : "; getline(cin, author);
            cout << "Genre          : "; getline(cin, genre);
            cout << "ISBN           : "; getline(cin, isbn);
            cout << "Format(PDF etc): "; getline(cin, format);
            cout << "Download Link  : "; getline(cin, link);
            lib.addBook(new EBook(title, author, genre, isbn, format, link));
            break;

        case 3:
            printHeader("ADD REFERENCE BOOK");
            cout << "Title   : "; getline(cin, title);
            cout << "Author  : "; getline(cin, author);
            cout << "Genre   : "; getline(cin, genre);
            cout << "ISBN    : "; getline(cin, isbn);
            cout << "Copies  : "; cin >> copies;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Section : "; getline(cin, section);
            lib.addBook(new ReferenceBook(title, author, genre, isbn, copies, section));
            break;

        case 4:
            printHeader("REMOVE BOOK");
            cout << "Enter ISBN: "; getline(cin, isbn);
            lib.removeBook(isbn);
            break;

        case 5: lib.viewAllBooks();       break;
        case 6: lib.viewAvailableBooks(); break;
        }
        pauseScreen();
    }
}

void memberManagementMenu(Library& lib) {
    while (true) {
        printHeader("MEMBER MANAGEMENT");
        cout << "  1. Register Student\n";
        cout << "  2. Register Faculty\n";
        cout << "  3. Register Librarian\n";
        cout << "  4. Remove Member\n";
        cout << "  5. View All Members\n";
        cout << "  6. View Member Details\n";
        cout << "  0. Back\n";

        int choice = getChoice(0, 6);
        if (choice == 0) break;

        string name, id, contact, email, extra;

        switch (choice) {
        case 1:
            printHeader("REGISTER STUDENT");
            cout << "Name       : "; getline(cin, name);
            cout << "Student ID : "; getline(cin, id);
            cout << "Contact    : "; getline(cin, contact);
            cout << "Email      : "; getline(cin, email);
            cout << "Department : "; getline(cin, extra);
            lib.addMember(new Student(name, id, contact, email, extra));
            break;

        case 2:
            printHeader("REGISTER FACULTY");
            cout << "Name        : "; getline(cin, name);
            cout << "Faculty ID  : "; getline(cin, id);
            cout << "Contact     : "; getline(cin, contact);
            cout << "Email       : "; getline(cin, email);
            cout << "Designation : "; getline(cin, extra);
            lib.addMember(new Faculty(name, id, contact, email, extra));
            break;

        case 3:
            printHeader("REGISTER LIBRARIAN");
            cout << "Name        : "; getline(cin, name);
            cout << "ID          : "; getline(cin, id);
            cout << "Contact     : "; getline(cin, contact);
            cout << "Email       : "; getline(cin, email);
            cout << "Employee ID : "; getline(cin, extra);
            lib.addLibrarian(new Librarian(name, id, contact, email, extra));
            break;

        case 4:
            printHeader("REMOVE MEMBER");
            cout << "Enter Member ID: "; getline(cin, id);
            lib.removeMember(id);
            break;

        case 5: lib.viewAllMembers(); break;

        case 6:
            printHeader("VIEW MEMBER DETAILS");
            cout << "Enter Member ID: "; getline(cin, id);
            lib.viewMember(id);
            break;
        }
        pauseScreen();
    }
}

void issueReturnMenu(Library& lib) {
    while (true) {
        printHeader("ISSUE / RETURN SYSTEM");
        cout << "  1. Issue Book to Member\n";
        cout << "  2. Return Book from Member\n";
        cout << "  3. View All Issue Records\n";
        cout << "  4. View Overdue Issues\n";
        cout << "  0. Back\n";

        int choice = getChoice(0, 4);
        if (choice == 0) break;

        string isbn, memberID;

        switch (choice) {
        case 1:
            printHeader("ISSUE BOOK");
            cout << "Enter Book ISBN : "; getline(cin, isbn);
            cout << "Enter Member ID : "; getline(cin, memberID);
            lib.issueBook(isbn, memberID);
            break;

        case 2:
            printHeader("RETURN BOOK");
            cout << "Enter Book ISBN : "; getline(cin, isbn);
            cout << "Enter Member ID : "; getline(cin, memberID);
            lib.returnBook(isbn, memberID);
            break;

        case 3: lib.viewAllIssues(); break;
        case 4: lib.viewOverdue();   break;
        }
        pauseScreen();
    }
}

void searchMenu(Library& lib) {
    while (true) {
        printHeader("SEARCH SYSTEM");
        cout << "  1. Search Books by Title\n";
        cout << "  2. Search Books by Author\n";
        cout << "  3. Search Books by Genre\n";
        cout << "  4. Search Books by ISBN\n";
        cout << "  5. Show All Available Books\n";
        cout << "  6. Search Member by Name or ID\n";
        cout << "  0. Back\n";

        int choice = getChoice(0, 6);
        if (choice == 0) break;

        string query;

        switch (choice) {
        case 1: cout << "Title keyword  : "; getline(cin, query); lib.searchBooks(query, "title");    break;
        case 2: cout << "Author keyword : "; getline(cin, query); lib.searchBooks(query, "author");   break;
        case 3: cout << "Genre keyword  : "; getline(cin, query); lib.searchBooks(query, "genre");    break;
        case 4: cout << "ISBN           : "; getline(cin, query); lib.searchBooks(query, "isbn");     break;
        case 5:                                                    lib.searchBooks("",    "available");break;
        case 6: cout << "Name or ID     : "; getline(cin, query); lib.searchMembers(query);           break;
        }
        pauseScreen();
    }
}

void finesMenu(Library& lib) {
    while (true) {
        printHeader("FINES MANAGEMENT");
        cout << "  1. Process All Fines (scan for overdue)\n";
        cout << "  2. Pay Fine for Member\n";
        cout << "  3. View Member Fine Status\n";
        cout << "  0. Back\n";

        int choice = getChoice(0, 3);
        if (choice == 0) break;

        string memberID;
        double amount;

        switch (choice) {
        case 1:
            lib.processFines();
            break;

        case 2:
            printHeader("PAY FINE");
            cout << "Enter Member ID  : "; getline(cin, memberID);
            cout << "Amount (Rs.)     : "; cin >> amount;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            lib.payFine(memberID, amount);
            break;

        case 3:
            printHeader("MEMBER FINE STATUS");
            cout << "Enter Member ID: "; getline(cin, memberID);
            lib.viewMember(memberID);
            break;
        }
        pauseScreen();
    }
}

void reportsMenu(Library& lib) {
    while (true) {
        printHeader("REPORTS & DASHBOARD");
        cout << "  1. Full Library Report\n";
        cout << "  2. View Overdue Issues\n";
        cout << "  3. View All Available Books\n";
        cout << "  4. View All Members\n";
        cout << "  5. View All Issue Records\n";
        cout << "  0. Back\n";

        int choice = getChoice(0, 5);
        if (choice == 0) break;

        switch (choice) {
        case 1: lib.generateReport();     break;
        case 2: lib.viewOverdue();        break;
        case 3: lib.viewAvailableBooks(); break;
        case 4: lib.viewAllMembers();     break;
        case 5: lib.viewAllIssues();      break;
        }
        pauseScreen();
    }
}

// ── Seed data — large realistic library dataset ───────────────────────────────

void seedLibrary(Library& lib) {

    // ── FICTION ──────────────────────────────────────────────────────────────
    lib.addBook(new Book("1984",                              "George Orwell",              "Fiction",     "978-0451524935", 5));
    lib.addBook(new Book("Animal Farm",                       "George Orwell",              "Fiction",     "978-0451526342", 4));
    lib.addBook(new Book("Brave New World",                   "Aldous Huxley",              "Fiction",     "978-0060850524", 4));
    lib.addBook(new Book("The Great Gatsby",                  "F. Scott Fitzgerald",        "Fiction",     "978-0743273565", 5));
    lib.addBook(new Book("To Kill a Mockingbird",             "Harper Lee",                 "Fiction",     "978-0061935466", 4));
    lib.addBook(new Book("The Catcher in the Rye",            "J.D. Salinger",              "Fiction",     "978-0316769174", 3));
    lib.addBook(new Book("Lord of the Flies",                 "William Golding",            "Fiction",     "978-0399501487", 4));
    lib.addBook(new Book("The Alchemist",                     "Paulo Coelho",               "Fiction",     "978-0062315007", 5));
    lib.addBook(new Book("The Kite Runner",                   "Khaled Hosseini",            "Fiction",     "978-1594631931", 4));
    lib.addBook(new Book("A Thousand Splendid Suns",          "Khaled Hosseini",            "Fiction",     "978-1594483073", 3));
    lib.addBook(new Book("Crime and Punishment",              "Fyodor Dostoevsky",          "Fiction",     "978-0486415871", 3));
    lib.addBook(new Book("The Brothers Karamazov",            "Fyodor Dostoevsky",          "Fiction",     "978-0374528379", 2));
    lib.addBook(new Book("Pride and Prejudice",               "Jane Austen",                "Fiction",     "978-0141439518", 5));
    lib.addBook(new Book("Sense and Sensibility",             "Jane Austen",                "Fiction",     "978-0141439662", 3));
    lib.addBook(new Book("Jane Eyre",                         "Charlotte Bronte",           "Fiction",     "978-0142437209", 3));
    lib.addBook(new Book("Wuthering Heights",                 "Emily Bronte",               "Fiction",     "978-0141439556", 3));
    lib.addBook(new Book("The Old Man and the Sea",           "Ernest Hemingway",           "Fiction",     "978-0684801223", 4));
    lib.addBook(new Book("Of Mice and Men",                   "John Steinbeck",             "Fiction",     "978-0140177398", 4));
    lib.addBook(new Book("The Grapes of Wrath",               "John Steinbeck",             "Fiction",     "978-0143039433", 3));
    lib.addBook(new Book("Fahrenheit 451",                    "Ray Bradbury",               "Fiction",     "978-1451673319", 4));
    lib.addBook(new Book("Slaughterhouse-Five",               "Kurt Vonnegut",              "Fiction",     "978-0440180296", 3));
    lib.addBook(new Book("The Road",                          "Cormac McCarthy",            "Fiction",     "978-0307387899", 3));
    lib.addBook(new Book("Life of Pi",                        "Yann Martel",                "Fiction",     "978-0156027328", 4));
    lib.addBook(new Book("The Handmaid's Tale",               "Margaret Atwood",            "Fiction",     "978-0385490818", 4));
    lib.addBook(new Book("Midnight's Children",               "Salman Rushdie",             "Fiction",     "978-0812976533", 2));

    // ── SCIENCE FICTION & FANTASY ─────────────────────────────────────────────
    lib.addBook(new Book("Dune",                              "Frank Herbert",              "Sci-Fi",      "978-0441013593", 4));
    lib.addBook(new Book("Foundation",                        "Isaac Asimov",               "Sci-Fi",      "978-0553293357", 4));
    lib.addBook(new Book("The Hitchhiker's Guide to the Galaxy","Douglas Adams",            "Sci-Fi",      "978-0345391803", 5));
    lib.addBook(new Book("Ender's Game",                      "Orson Scott Card",           "Sci-Fi",      "978-0812550702", 4));
    lib.addBook(new Book("Neuromancer",                       "William Gibson",             "Sci-Fi",      "978-0441569595", 3));
    lib.addBook(new Book("The Lord of the Rings",             "J.R.R. Tolkien",             "Fantasy",     "978-0544003415", 5));
    lib.addBook(new Book("The Hobbit",                        "J.R.R. Tolkien",             "Fantasy",     "978-0547928227", 6));
    lib.addBook(new Book("Harry Potter and the Sorcerer's Stone","J.K. Rowling",            "Fantasy",     "978-0439708180", 6));
    lib.addBook(new Book("A Game of Thrones",                 "George R.R. Martin",         "Fantasy",     "978-0553593716", 4));
    lib.addBook(new Book("The Name of the Wind",              "Patrick Rothfuss",           "Fantasy",     "978-0756404741", 3));

    // ── NON-FICTION & HISTORY ─────────────────────────────────────────────────
    lib.addBook(new Book("Sapiens",                           "Yuval Noah Harari",          "History",     "978-0062316097", 5));
    lib.addBook(new Book("Homo Deus",                         "Yuval Noah Harari",          "History",     "978-0062464316", 4));
    lib.addBook(new Book("21 Lessons for the 21st Century",   "Yuval Noah Harari",          "Non-Fiction", "978-0525512172", 3));
    lib.addBook(new Book("Guns Germs and Steel",              "Jared Diamond",              "History",     "978-0393317558", 3));
    lib.addBook(new Book("The Silk Roads",                    "Peter Frankopan",            "History",     "978-1101912379", 3));
    lib.addBook(new Book("A Brief History of Time",           "Stephen Hawking",            "Science",     "978-0553380163", 5));
    lib.addBook(new Book("The Selfish Gene",                  "Richard Dawkins",            "Science",     "978-0198788607", 3));
    lib.addBook(new Book("Thinking Fast and Slow",            "Daniel Kahneman",            "Psychology",  "978-0374533557", 5));
    lib.addBook(new Book("The Power of Habit",                "Charles Duhigg",             "Psychology",  "978-0812981605", 4));
    lib.addBook(new Book("Atomic Habits",                     "James Clear",                "Self-Help",   "978-0735211292", 6));
    lib.addBook(new Book("Deep Work",                         "Cal Newport",                "Self-Help",   "978-1455586691", 4));
    lib.addBook(new Book("The 7 Habits of Highly Effective People","Stephen Covey",         "Self-Help",   "978-0743269513", 5));
    lib.addBook(new Book("How to Win Friends and Influence People","Dale Carnegie",          "Self-Help",   "978-0671027032", 5));
    lib.addBook(new Book("Man's Search for Meaning",          "Viktor Frankl",              "Psychology",  "978-0807014271", 4));
    lib.addBook(new Book("Outliers",                          "Malcolm Gladwell",           "Non-Fiction", "978-0316017930", 4));
    lib.addBook(new Book("The Tipping Point",                 "Malcolm Gladwell",           "Non-Fiction", "978-0316346627", 3));
    lib.addBook(new Book("Freakonomics",                      "Steven Levitt",              "Economics",   "978-0060731335", 3));
    lib.addBook(new Book("The Wealth of Nations",             "Adam Smith",                 "Economics",   "978-0679783367", 2));
    lib.addBook(new Book("Capital in the 21st Century",       "Thomas Piketty",             "Economics",   "978-0674430006", 2));

    // ── TECHNOLOGY & COMPUTER SCIENCE ────────────────────────────────────────
    lib.addBook(new Book("Clean Code",                        "Robert C. Martin",           "Technology",  "978-0132350884", 4));
    lib.addBook(new Book("The Pragmatic Programmer",          "Andrew Hunt",                "Technology",  "978-0135957059", 4));
    lib.addBook(new Book("Design Patterns",                   "Gang of Four",               "Technology",  "978-0201633610", 3));
    lib.addBook(new Book("The Mythical Man-Month",            "Frederick Brooks",           "Technology",  "978-0201835953", 3));
    lib.addBook(new Book("Code Complete",                     "Steve McConnell",            "Technology",  "978-0735619678", 3));
    lib.addBook(new Book("Computer Networks",                 "Andrew Tanenbaum",           "Technology",  "978-0132126953", 3));
    lib.addBook(new Book("Operating System Concepts",         "Abraham Silberschatz",       "Technology",  "978-1118063330", 4));
    lib.addBook(new Book("Database System Concepts",          "Abraham Silberschatz",       "Technology",  "978-0078022159", 4));
    lib.addBook(new Book("Artificial Intelligence",           "Stuart Russell",             "Technology",  "978-0136042594", 3));

    // ── ISLAMIC ───────────────────────────────────────────────────────────────
    lib.addBook(new Book("The Sealed Nectar",                 "Safiur Rahman Mubarakpuri",  "Islamic",     "978-9960899558", 5));
    lib.addBook(new Book("In the Footsteps of the Prophet",   "Tariq Ramadan",              "Islamic",     "978-0195308808", 4));
    lib.addBook(new Book("Don't Be Sad",                      "Dr. Aid al-Qarni",           "Islamic",     "978-9960892618", 5));
    lib.addBook(new Book("Revive Your Heart",                 "Nouman Ali Khan",            "Islamic",     "978-1847740793", 4));

    // ── URDU LITERATURE ───────────────────────────────────────────────────────
    lib.addBook(new Book("Aag Ka Darya",                      "Qurratulain Hyder",          "Urdu Fiction","978-8126007547", 3));
    lib.addBook(new Book("Khuda Aur Mohabbat",                "Hashim Nadeem",              "Urdu Fiction","978-9693515466", 5));
    lib.addBook(new Book("Peer-e-Kamil",                      "Umera Ahmed",                "Urdu Fiction","978-9693515473", 5));
    lib.addBook(new Book("Jannat Ke Pattay",                  "Nimra Ahmed",                "Urdu Fiction","978-9693515480", 4));
    lib.addBook(new Book("Raja Gidh",                         "Bano Qudsia",                "Urdu Fiction","978-9693511369", 3));

    // ── EBOOKS ────────────────────────────────────────────────────────────────
    lib.addBook(new EBook("Introduction to Algorithms",       "Thomas H. Cormen",           "Technology",  "978-0262033848", "PDF",  "https://library.edu/cormen-algorithms.pdf"));
    lib.addBook(new EBook("Python Crash Course",              "Eric Matthes",               "Technology",  "978-1593279288", "PDF",  "https://library.edu/python-crash-course.pdf"));
    lib.addBook(new EBook("The C++ Programming Language",     "Bjarne Stroustrup",          "Technology",  "978-0321563842", "PDF",  "https://library.edu/cpp-stroustrup.pdf"));
    lib.addBook(new EBook("Deep Learning",                    "Ian Goodfellow",             "Technology",  "978-0262035613", "PDF",  "https://library.edu/deep-learning.pdf"));
    lib.addBook(new EBook("Linear Algebra and Its Applications","Gilbert Strang",           "Mathematics", "978-0030105678", "PDF",  "https://library.edu/linear-algebra-strang.pdf"));
    lib.addBook(new EBook("Calculus",                         "James Stewart",              "Mathematics", "978-1285740621", "PDF",  "https://library.edu/stewart-calculus.pdf"));
    lib.addBook(new EBook("Discrete Mathematics",             "Kenneth Rosen",              "Mathematics", "978-0073383095", "PDF",  "https://library.edu/discrete-math-rosen.pdf"));
    lib.addBook(new EBook("Probability and Statistics",       "DeGroot & Schervish",        "Mathematics", "978-0321500465", "PDF",  "https://library.edu/probability-statistics.pdf"));
    lib.addBook(new EBook("Microeconomics",                   "Paul Krugman",               "Economics",   "978-1464143878", "EPUB", "https://library.edu/krugman-micro.epub"));
    lib.addBook(new EBook("Macroeconomics",                   "N. Gregory Mankiw",          "Economics",   "978-1319105990", "EPUB", "https://library.edu/mankiw-macro.epub"));
    lib.addBook(new EBook("Sapiens (Digital Edition)",        "Yuval Noah Harari",          "History",     "978-0062316110", "EPUB", "https://library.edu/sapiens-digital.epub"));
    lib.addBook(new EBook("Atomic Habits (Digital)",          "James Clear",                "Self-Help",   "978-0735211308", "EPUB", "https://library.edu/atomic-habits-digital.epub"));

    // ── REFERENCE BOOKS ───────────────────────────────────────────────────────
    lib.addBook(new ReferenceBook("Oxford English Dictionary",        "Oxford Press",        "Reference",   "978-0198611868", 3, "Language"));
    lib.addBook(new ReferenceBook("Encyclopedia Britannica Vol.1",    "Britannica",          "Reference",   "978-1593392925", 2, "General"));
    lib.addBook(new ReferenceBook("Encyclopedia Britannica Vol.2",    "Britannica",          "Reference",   "978-1593392932", 2, "General"));
    lib.addBook(new ReferenceBook("World Atlas 2024",                 "National Geographic", "Reference",   "978-1426221323", 2, "Geography"));
    lib.addBook(new ReferenceBook("Black's Law Dictionary",           "Bryan Garner",        "Reference",   "978-0314199683", 2, "Law"));
    lib.addBook(new ReferenceBook("Merck Medical Manual",             "Merck Publishing",    "Reference",   "978-0911910186", 2, "Medical"));
    lib.addBook(new ReferenceBook("CRC Handbook of Chemistry",        "CRC Press",           "Reference",   "978-1439880494", 2, "Science"));
    lib.addBook(new ReferenceBook("Oxford Dictionary of Mathematics", "Oxford Press",        "Reference",   "978-0199679591", 2, "Mathematics"));
    lib.addBook(new ReferenceBook("The Cambridge History of Islam",   "Cambridge University","Reference",   "978-0521291354", 2, "History"));
    lib.addBook(new ReferenceBook("IEEE Standards Reference",         "IEEE",                "Reference",   "978-0738181684", 2, "Technology"));

    // ── LIBRARIANS ────────────────────────────────────────────────────────────
    lib.addLibrarian(new Librarian("Mr. Hassan Raza",  "LIB-001", "0300-1234567", "hassan@library.edu",  "EMP-001"));
    lib.addLibrarian(new Librarian("Ms. Ayesha Noor",  "LIB-002", "0301-2345678", "ayesha@library.edu",  "EMP-002"));

    // ── STUDENTS ──────────────────────────────────────────────────────────────
    lib.addMember(new Student("Ali Khan",          "STU-001", "0311-1111111", "ali.khan@uni.edu",       "Computer Science"));
    lib.addMember(new Student("Sara Ahmed",        "STU-002", "0322-2222222", "sara.ahmed@uni.edu",     "Electrical Engineering"));
    lib.addMember(new Student("Bilal Hussain",     "STU-003", "0333-3333333", "bilal.h@uni.edu",        "Mechanical Engineering"));
    lib.addMember(new Student("Fatima Malik",      "STU-004", "0344-4444444", "fatima.m@uni.edu",       "Business Administration"));
    lib.addMember(new Student("Umar Farooq",       "STU-005", "0355-5555555", "umar.f@uni.edu",         "Computer Science"));
    lib.addMember(new Student("Zainab Siddiqui",   "STU-006", "0366-6666666", "zainab.s@uni.edu",       "Mathematics"));
    lib.addMember(new Student("Hassan Mirza",      "STU-007", "0377-7777777", "hassan.m@uni.edu",       "Physics"));
    lib.addMember(new Student("Amna Qureshi",      "STU-008", "0388-8888888", "amna.q@uni.edu",         "Chemistry"));
    lib.addMember(new Student("Talha Sheikh",      "STU-009", "0399-9999999", "talha.s@uni.edu",        "Civil Engineering"));
    lib.addMember(new Student("Maria Baig",        "STU-010", "0312-1010101", "maria.b@uni.edu",        "Architecture"));
    lib.addMember(new Student("Hamza Iqbal",       "STU-011", "0323-1111222", "hamza.i@uni.edu",        "Computer Science"));
    lib.addMember(new Student("Sana Riaz",         "STU-012", "0334-2222333", "sana.r@uni.edu",         "Biotechnology"));
    lib.addMember(new Student("Kashif Mehmood",    "STU-013", "0345-3333444", "kashif.m@uni.edu",       "Software Engineering"));
    lib.addMember(new Student("Hina Tariq",        "STU-014", "0356-4444555", "hina.t@uni.edu",         "Electrical Engineering"));
    lib.addMember(new Student("Asad Javed",        "STU-015", "0367-5555666", "asad.j@uni.edu",         "Computer Science"));

    // ── FACULTY ───────────────────────────────────────────────────────────────
    lib.addMember(new Faculty("Dr. Usman Ghani",   "FAC-001", "0333-0000001", "usman.g@uni.edu",        "Professor"));
    lib.addMember(new Faculty("Dr. Nadia Rashid",  "FAC-002", "0344-0000002", "nadia.r@uni.edu",        "Associate Professor"));
    lib.addMember(new Faculty("Mr. Imran Siddiqui","FAC-003", "0355-0000003", "imran.s@uni.edu",        "Lecturer"));
    lib.addMember(new Faculty("Ms. Rabia Noor",    "FAC-004", "0366-0000004", "rabia.n@uni.edu",        "Lecturer"));
    lib.addMember(new Faculty("Dr. Kamran Ali",    "FAC-005", "0377-0000005", "kamran.a@uni.edu",       "Professor"));
    lib.addMember(new Faculty("Dr. Saima Shafiq",  "FAC-006", "0388-0000006", "saima.sh@uni.edu",       "Associate Professor"));

    cout << "\n[✓] Library loaded successfully!" << endl;
    cout << "    Physical Books : 69 titles" << endl;
    cout << "    EBooks         : 12 titles" << endl;
    cout << "    Reference Books: 10 titles" << endl;
    cout << "    Students       : 15" << endl;
    cout << "    Faculty        : 6" << endl;
    cout << "    Librarians     : 2" << endl;
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main() {
    Library library("National University Library");
    seedLibrary(library);

    while (true) {
        cout << "\n\n";
        cout << "╔══════════════════════════════════════════╗\n";
        cout << "║     NATIONAL UNIVERSITY LIBRARY          ║\n";
        cout << "║          Management System               ║\n";
        cout << "╠══════════════════════════════════════════╣\n";
        cout << "║  1.  Book Management                     ║\n";
        cout << "║  2.  Member Management                   ║\n";
        cout << "║  3.  Issue / Return Books                ║\n";
        cout << "║  4.  Search                              ║\n";
        cout << "║  5.  Fines                               ║\n";
        cout << "║  6.  Reports & Dashboard                 ║\n";
        cout << "║  0.  Exit                                ║\n";
        cout << "╚══════════════════════════════════════════╝\n";

        int choice = getChoice(0, 6);

        switch (choice) {
        case 1: bookManagementMenu(library);   break;
        case 2: memberManagementMenu(library); break;
        case 3: issueReturnMenu(library);      break;
        case 4: searchMenu(library);           break;
        case 5: finesMenu(library);            break;
        case 6: reportsMenu(library);          break;
        case 0:
            cout << "\n[✓] Exiting Library System. Goodbye!\n\n";
            return 0;
        }
    }
}
