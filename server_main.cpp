// server_main.cpp
// Run this instead of main.cpp when using the GUI
// Opens http://localhost:8080 in your browser

#include "httplib.h"
#include "Library.h"
#include "Book.h"
#include "EBook.h"
#include "ReferenceBook.h"
#include "Student.h"
#include "Faculty.h"
#include "Librarian.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// ── Read index.html from disk ─────────────────────────────────────────────
string readFile(const string& path) {
    ifstream f(path);
    if (!f.is_open()) return "<h1>index.html not found. Place it in the same folder.</h1>";
    ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// ── Simple JSON body parser ───────────────────────────────────────────────
string parseField(const string& body, const string& key) {
    string search = "\"" + key + "\":\"";
    size_t pos = body.find(search);
    if (pos == string::npos) return "";
    pos += search.size();
    size_t end = body.find("\"", pos);
    if (end == string::npos) return "";
    string val = body.substr(pos, end - pos);
    // unescape
    string out;
    for (size_t i = 0; i < val.size(); i++) {
        if (val[i] == '\\' && i+1 < val.size()) { out += val[++i]; }
        else out += val[i];
    }
    return out;
}

int parseIntField(const string& body, const string& key, int def = 1) {
    string search = "\"" + key + "\":";
    size_t pos = body.find(search);
    if (pos == string::npos) return def;
    pos += search.size();
    try { return stoi(body.substr(pos)); } catch (...) { return def; }
}

double parseDblField(const string& body, const string& key, double def = 0.0) {
    string search = "\"" + key + "\":";
    size_t pos = body.find(search);
    if (pos == string::npos) return def;
    pos += search.size();
    try { return stod(body.substr(pos)); } catch (...) { return def; }
}

// ── Seed library data ─────────────────────────────────────────────────────
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
    lib.addBook(new Book("Clean Code",                        "Robert C. Martin",           "Technology",  "978-0132350884", 4));
    lib.addBook(new Book("The Pragmatic Programmer",          "Andrew Hunt",                "Technology",  "978-0135957059", 4));
    lib.addBook(new Book("Design Patterns",                   "Gang of Four",               "Technology",  "978-0201633610", 3));
    lib.addBook(new Book("The Mythical Man-Month",            "Frederick Brooks",           "Technology",  "978-0201835953", 3));
    lib.addBook(new Book("Code Complete",                     "Steve McConnell",            "Technology",  "978-0735619678", 3));
    lib.addBook(new Book("Computer Networks",                 "Andrew Tanenbaum",           "Technology",  "978-0132126953", 3));
    lib.addBook(new Book("Operating System Concepts",         "Abraham Silberschatz",       "Technology",  "978-1118063330", 4));
    lib.addBook(new Book("Database System Concepts",          "Abraham Silberschatz",       "Technology",  "978-0078022159", 4));
    lib.addBook(new Book("Artificial Intelligence",           "Stuart Russell",             "Technology",  "978-0136042594", 3));
    lib.addBook(new Book("The Sealed Nectar",                 "Safiur Rahman Mubarakpuri",  "Islamic",     "978-9960899558", 5));
    lib.addBook(new Book("In the Footsteps of the Prophet",   "Tariq Ramadan",              "Islamic",     "978-0195308808", 4));
    lib.addBook(new Book("Don't Be Sad",                      "Dr. Aid al-Qarni",           "Islamic",     "978-9960892618", 5));
    lib.addBook(new Book("Revive Your Heart",                 "Nouman Ali Khan",            "Islamic",     "978-1847740793", 4));
    lib.addBook(new Book("Aag Ka Darya",                      "Qurratulain Hyder",          "Urdu Fiction","978-8126007547", 3));
    lib.addBook(new Book("Khuda Aur Mohabbat",                "Hashim Nadeem",              "Urdu Fiction","978-9693515466", 5));
    lib.addBook(new Book("Peer-e-Kamil",                      "Umera Ahmed",                "Urdu Fiction","978-9693515473", 5));
    lib.addBook(new Book("Jannat Ke Pattay",                  "Nimra Ahmed",                "Urdu Fiction","978-9693515480", 4));
    lib.addBook(new Book("Raja Gidh",                         "Bano Qudsia",                "Urdu Fiction","978-9693511369", 3));
    lib.addBook(new EBook("Introduction to Algorithms",       "Thomas H. Cormen",           "Technology",  "978-0262033848", "PDF",  "https://library.edu/cormen-algorithms.pdf"));
    lib.addBook(new EBook("Python Crash Course",              "Eric Matthes",               "Technology",  "978-1593279288", "PDF",  "https://library.edu/python-crash-course.pdf"));
    lib.addBook(new EBook("The C++ Programming Language",     "Bjarne Stroustrup",          "Technology",  "978-0321563842", "PDF",  "https://library.edu/cpp-stroustrup.pdf"));
    lib.addBook(new EBook("Deep Learning",                    "Ian Goodfellow",             "Technology",  "978-0262035613", "PDF",  "https://library.edu/deep-learning.pdf"));
    lib.addBook(new EBook("Linear Algebra and Its Applications","Gilbert Strang",           "Mathematics", "978-0030105678", "PDF",  "https://library.edu/linear-algebra.pdf"));
    lib.addBook(new EBook("Calculus",                         "James Stewart",              "Mathematics", "978-1285740621", "PDF",  "https://library.edu/stewart-calculus.pdf"));
    lib.addBook(new EBook("Discrete Mathematics",             "Kenneth Rosen",              "Mathematics", "978-0073383095", "PDF",  "https://library.edu/discrete-math.pdf"));
    lib.addBook(new EBook("Probability and Statistics",       "DeGroot & Schervish",        "Mathematics", "978-0321500465", "PDF",  "https://library.edu/probability.pdf"));
    lib.addBook(new EBook("Microeconomics",                   "Paul Krugman",               "Economics",   "978-1464143878", "EPUB", "https://library.edu/krugman-micro.epub"));
    lib.addBook(new EBook("Macroeconomics",                   "N. Gregory Mankiw",          "Economics",   "978-1319105990", "EPUB", "https://library.edu/mankiw-macro.epub"));
    lib.addBook(new EBook("Sapiens (Digital Edition)",        "Yuval Noah Harari",          "History",     "978-0062316110", "EPUB", "https://library.edu/sapiens-digital.epub"));
    lib.addBook(new EBook("Atomic Habits (Digital)",          "James Clear",                "Self-Help",   "978-0735211308", "EPUB", "https://library.edu/atomic-habits.epub"));
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

    lib.addLibrarian(new Librarian("Mr. Hassan Raza",  "LIB-001", "0300-1234567", "hassan@library.edu",  "EMP-001"));
    lib.addLibrarian(new Librarian("Ms. Ayesha Noor",  "LIB-002", "0301-2345678", "ayesha@library.edu",  "EMP-002"));
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
    lib.addMember(new Faculty("Dr. Usman Ghani",   "FAC-001", "0333-0000001", "usman.g@uni.edu",        "Professor"));
    lib.addMember(new Faculty("Dr. Nadia Rashid",  "FAC-002", "0344-0000002", "nadia.r@uni.edu",        "Associate Professor"));
    lib.addMember(new Faculty("Mr. Imran Siddiqui","FAC-003", "0355-0000003", "imran.s@uni.edu",        "Lecturer"));
    lib.addMember(new Faculty("Ms. Rabia Noor",    "FAC-004", "0366-0000004", "rabia.n@uni.edu",        "Lecturer"));
    lib.addMember(new Faculty("Dr. Kamran Ali",    "FAC-005", "0377-0000005", "kamran.a@uni.edu",       "Professor"));
    lib.addMember(new Faculty("Dr. Saima Shafiq",  "FAC-006", "0388-0000006", "saima.sh@uni.edu",       "Associate Professor"));

    // ── ADDITIONAL STUDENTS ──────────────────────────────────────────────────
    lib.addMember(new Student("Ahmad Raza",        "STU-016", "0311-6060606", "ahmad.r@uni.edu",        "Computer Science"));
    lib.addMember(new Student("Noor Fatima",        "STU-017", "0322-7070707", "noor.f@uni.edu",         "Business Administration"));
    lib.addMember(new Student("Saad Mahmood",       "STU-018", "0333-8080808", "saad.m@uni.edu",         "Software Engineering"));
    lib.addMember(new Student("Iqra Shahid",        "STU-019", "0344-9090909", "iqra.s@uni.edu",         "Mathematics"));
    lib.addMember(new Student("Zubair Ahmed",       "STU-020", "0355-1020304", "zubair.a@uni.edu",       "Physics"));

    // ── ADDITIONAL FACULTY ────────────────────────────────────────────────────
    lib.addMember(new Faculty("Dr. Tariq Mehmood",  "FAC-007", "0399-0000007", "tariq.m@uni.edu",        "Professor"));
    lib.addMember(new Faculty("Ms. Hira Baig",      "FAC-008", "0312-0000008", "hira.b@uni.edu",         "Lecturer"));

    // =========================================================
    // DEMO: Pre-issue books to members
    // =========================================================
    // STU-001 Ali Khan — 2 books borrowed
    lib.issueBook("978-0451524935", "STU-001");   // 1984
    lib.issueBook("978-0743273565", "STU-001");   // The Great Gatsby

    // STU-002 Sara Ahmed — 3 books borrowed (AT LIMIT)
    lib.issueBook("978-0735211292", "STU-002");   // Atomic Habits
    lib.issueBook("978-0062315007", "STU-002");   // The Alchemist
    lib.issueBook("978-0062316097", "STU-002");   // Sapiens

    // STU-004 Fatima Malik — 1 book borrowed
    lib.issueBook("978-0132350884", "STU-004");   // Clean Code

    // STU-007 Hassan Mirza — 2 books borrowed
    lib.issueBook("978-9960892618", "STU-007");   // Don't Be Sad
    lib.issueBook("978-9693515473", "STU-007");   // Peer-e-Kamil

    // STU-011 Hamza Iqbal — 2 books borrowed
    lib.issueBook("978-0547928227", "STU-011");   // The Hobbit
    lib.issueBook("978-0441013593", "STU-011");   // Dune

    // STU-017 Noor Fatima — 1 book borrowed
    lib.issueBook("978-0439708180", "STU-017");   // Harry Potter

    // FAC-001 Dr. Usman — 3 books borrowed
    lib.issueBook("978-0374533557", "FAC-001");   // Thinking Fast and Slow
    lib.issueBook("978-0201633610", "FAC-001");   // Design Patterns
    lib.issueBook("978-0553380163", "FAC-001");   // A Brief History of Time

    // FAC-002 Dr. Nadia — 2 books borrowed
    lib.issueBook("978-0393317558", "FAC-002");   // Guns Germs and Steel
    lib.issueBook("978-0316017930", "FAC-002");   // Outliers

    // FAC-007 Dr. Tariq — 4 books borrowed
    lib.issueBook("978-0062316097", "FAC-007");   // Sapiens (different copy)
    lib.issueBook("978-0136042594", "FAC-007");   // Artificial Intelligence
    lib.issueBook("978-0374533557", "FAC-007");   // Thinking Fast and Slow (another copy)
    lib.issueBook("978-0807014271", "FAC-007");   // Man's Search for Meaning

    // =========================================================
    // DEMO: Add outstanding fines to some members
    // (these members had previously returned books late)
    // =========================================================
    lib.addFine("STU-003", 80.0);    // Bilal Hussain  — Rs. 80
    lib.addFine("STU-006", 120.0);   // Zainab Siddiqui — Rs. 120
    lib.addFine("STU-010", 50.0);    // Maria Baig      — Rs. 50
    lib.addFine("STU-016", 150.0);   // Ahmad Raza      — Rs. 150
    lib.addFine("FAC-003", 200.0);   // Mr. Imran       — Rs. 200
    lib.addFine("STU-020", 30.0);    // Zubair Ahmed    — Rs. 30

    cout << "[✓] Library seeded: 91 books, 28 members." << endl;
    cout << "    Active borrows : 17 issue records" << endl;
    cout << "    Members w/fines: 6" << endl;
}

// ── Main ──────────────────────────────────────────────────────────────────
int main() {
    Library library("National University Library");

    // Try to load saved data first. If no save file exists, use seed data.
    if (!library.loadData()) {
        cout << "[!] No saved data found. Loading default seed data..." << endl;
        seedLibrary(library);
        library.saveData(); // Save seed data so it persists from now on
        cout << "[✓] Seed data saved." << endl;
    }

    httplib::Server svr;

    // CORS — allow browser to call the server
    auto cors = [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin",  "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };

    // Serve index.html at root
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(readFile("index.html"), "text/html");
    });

    // ── API Routes ────────────────────────────────────────────────────────

    svr.Get("/api/books", [&](const httplib::Request&, httplib::Response& res) {
        cors({}, res);
        res.set_content(library.jsonBooks(), "application/json");
    });

    svr.Get("/api/members", [&](const httplib::Request&, httplib::Response& res) {
        cors({}, res);
        res.set_content(library.jsonMembers(), "application/json");
    });

    svr.Get("/api/issues", [&](const httplib::Request&, httplib::Response& res) {
        cors({}, res);
        res.set_content(library.jsonIssues(), "application/json");
    });

    svr.Get("/api/report", [&](const httplib::Request&, httplib::Response& res) {
        cors({}, res);
        res.set_content(library.jsonReport(), "application/json");
    });

    svr.Get("/api/search", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string q     = req.has_param("q")     ? req.get_param_value("q")     : "";
        string field = req.has_param("field") ? req.get_param_value("field") : "title";
        res.set_content(library.jsonSearchBooks(q, field), "application/json");
    });

    svr.Post("/api/issue", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string isbn = parseField(req.body, "isbn");
        string mid  = parseField(req.body, "memberId");
        string result = library.jsonIssueBook(isbn, mid);
        library.saveData();
        res.set_content(result, "application/json");
    });

    svr.Post("/api/return", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string isbn = parseField(req.body, "isbn");
        string mid  = parseField(req.body, "memberId");
        string result = library.jsonReturnBook(isbn, mid);
        library.saveData();
        res.set_content(result, "application/json");
    });

    svr.Post("/api/books/add", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string type   = parseField(req.body, "type");
        string title  = parseField(req.body, "title");
        string author = parseField(req.body, "author");
        string genre  = parseField(req.body, "genre");
        string isbn   = parseField(req.body, "isbn");
        int    copies = parseIntField(req.body, "copies", 1);
        string extra1 = parseField(req.body, type == "ebook" ? "format" : "section");
        string extra2 = parseField(req.body, "link");
        string result = library.jsonAddBook(type, title, author, genre, isbn, copies, extra1, extra2);
        library.saveData();
        res.set_content(result, "application/json");
    });

    svr.Post("/api/members/add", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string type    = parseField(req.body, "type");
        string name    = parseField(req.body, "name");
        string id      = parseField(req.body, "id");
        string contact = parseField(req.body, "contact");
        string email   = parseField(req.body, "email");
        string extra   = parseField(req.body, type == "student"   ? "dept"  :
                                              type == "faculty"   ? "desig" : "empid");
        string result = library.jsonAddMember(type, name, id, contact, email, extra);
        library.saveData();
        res.set_content(result, "application/json");
    });

    svr.Post("/api/fines/process", [&](const httplib::Request&, httplib::Response& res) {
        cors({}, res);
        string result = library.jsonProcessFines();
        library.saveData();
        res.set_content(result, "application/json");
    });

    svr.Post("/api/fines/pay", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string mid    = parseField(req.body, "memberId");
        double amount = parseDblField(req.body, "amount");
        string result = library.jsonPayFine(mid, amount);
        library.saveData();
        res.set_content(result, "application/json");
    });

    svr.Delete("/api/members/remove", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string mid = parseField(req.body, "memberId");
        string result = library.jsonRemoveMember(mid);
        library.saveData();
        res.set_content(result, "application/json");
    });

    svr.Post("/api/members/remove", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string mid = parseField(req.body, "memberId");
        string result = library.jsonRemoveMember(mid);
        library.saveData();
        res.set_content(result, "application/json");
    });

    svr.Get("/api/members/detail", [&](const httplib::Request& req, httplib::Response& res) {
        cors({}, res);
        string mid = req.has_param("id") ? req.get_param_value("id") : "";
        res.set_content(library.jsonMemberDetail(mid), "application/json");
    });

    // Handle OPTIONS preflight for CORS
    svr.Options(".*", [&](const httplib::Request&, httplib::Response& res) {
        cors({}, res);
        res.status = 204;
    });

    cout << "\n========================================\n";
    cout << "  Library Management System — GUI Mode\n";
    cout << "========================================\n";
    cout << "  Server running at: http://localhost:8080\n";
    cout << "  Open that URL in your browser.\n";
    cout << "  Press Ctrl+C to stop the server.\n";
    cout << "========================================\n\n";

    svr.listen("localhost", 8080);
    return 0;
}
