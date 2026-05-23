#include "FineCalculator.h"
#include "IssueRecord.h"
#include "Member.h"
#include "Book.h"
#include "Library.h"
#include <iostream>

double FineCalculator::calculateFine(IssueRecord* rec) const {
    int daysOverdue = rec->getDaysOverdue();
    if (daysOverdue <= 0) return 0.0;

    // Polymorphism: getFineRate() returns different values for Student vs Faculty
    double rate = rec->getMember()->getFineRate();
    return daysOverdue * rate;
}

void FineCalculator::applyFine(IssueRecord* rec, Member* member) {
    double fine = calculateFine(rec);
    if (fine > 0) {
        member->addFine(fine);
        cout << "Fine of Rs. " << fine << " applied to "
             << member->getName() << " for overdue book \""
             << rec->getBook()->getTitle() << "\"" << endl;
    }
}

void FineCalculator::processAllFines(Library& library) {
    cout << "\n--- Processing Fines for All Active Issues ---" << endl;
    int processed = 0;
    for (auto& rec : library.getAllIssueRecords()) {
        rec->updateStatus();
        if (rec->getStatus() == IssueStatus::OVERDUE) {
            applyFine(rec, rec->getMember());
            processed++;
        }
    }
    if (processed == 0)
        cout << "No overdue records found." << endl;
    cout << "----------------------------------------------" << endl;
}
