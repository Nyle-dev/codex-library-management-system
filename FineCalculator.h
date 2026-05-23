#ifndef FINECALCULATOR_H
#define FINECALCULATOR_H

class IssueRecord;
class Member;

class FineCalculator {
public:
    // Calculates fine based on days overdue and member's fine rate
    double calculateFine(IssueRecord* rec) const;

    // Applies the calculated fine to the member's account
    void applyFine(IssueRecord* rec, Member* member);

    // Checks and updates all active records, applies fines where needed
    void processAllFines(class Library& library);
};

#endif
