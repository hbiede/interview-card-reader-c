#ifndef INTERVIEW_CARD_READER_C_LOGCHECKER_H
#define INTERVIEW_CARD_READER_C_LOGCHECKER_H
#include <string.h>
#include <stdlib.h>

typedef enum {
    ENTER,
    EXIT,
} ScanDirection;

typedef struct {
    char *employee;
    ScanDirection scan;
} ScanRecord;

/**
 * Creates a `ScanRecord` pointer
 * @param employee The name of the employee
 * @param scan The direction the scan went (in or out)
 * @return a pointer to a new `ScanRecord`
 */
ScanRecord* createScanRecord(char* employee, ScanDirection scan);

/**
 * Takes an array of card scan logs, where each log contains the name of the employee
 * and the type of log (enter or exit).
 * The log begins at the beginning of the day and ends at the end of the day.
 * The room is physically clear at the start and end of each day.
 * Any missing logs are a result of a faulty card or card reader, however each entry is true.
 * Therefore, in the case of an exit with no preceding entry, the employee should
 * be appended to the missingEntries array, to note that their card did not read upon
 * entry of the room.
 *
 * @param records A list of scan log pointers
 * @param numRecords Number of scan logs in `records`
 * @param missingEntryLogEmployees Array in which to insert the missing entry log names
 * @param numMissingEntries Pointer which should yield the number of missing entries found. Given as 0 initially
 * @param missingExitLogEmployees Array in which to insert the missing exit log names
 * @param numMissingExits Pointer which should yield the number of missing exits found. Given as 0 initially
 */
void getInvalidLogs(ScanRecord **records, int numRecords, char **missingEntryLogEmployees, int *numMissingEntries,
                    char **missingExitLogEmployees, int *numMissingExits);

#endif
