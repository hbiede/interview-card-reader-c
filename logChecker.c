#include "logChecker.h"

void getInvalidLogs(ScanRecord **records, int numRecords, char **missingEntryLogEmployees, int *numMissingEntries, char **missingExitLogEmployees, int *numMissingExits) {

}

ScanRecord* createScanRecord(char* employee, ScanDirection scan) {
    ScanRecord *sr = (ScanRecord*) malloc(sizeof(ScanRecord));
    sr->employee = (char *) malloc(sizeof(char) * (strlen(employee) + 1));
    strcpy(sr->employee, employee);
    sr->scan = scan;
    return sr;
}
