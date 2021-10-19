// Do not modify
#include <stdio.h>
#include <stdlib.h>
#include "logChecker.h"

#define NUM_TESTS 8

void insertionSort(char **array, int size) {
    char *value;
    int i;
    for (i = 1; i < size; i++) {
        value = array[i];
        int j = i;
        while (j > 0 && strcmp(array[j - 1], value) > 0) {
            array[j] = array[j - 1];
            j--;
        }
        array[j] = value;
    }
}

int testNoInvalidsEmpty() {
    int numLogs = 0;
    ScanRecord *logs[] = {};
    char **entries = malloc(sizeof(char *) * numLogs);
    int entryCount = 0;
    char **exits = malloc(sizeof(char *) * numLogs);
    int exitCount = 0;
    getInvalidLogs(logs, numLogs, entries, &entryCount, exits, &exitCount);

    free(entries);
    free(exits);
    if (entryCount != 0) {
        printf("No entries should be found. %d found\n", entryCount);
        return 1;
    }
    if (exitCount != 0) {
        printf("No exits should be found. %d found\n", exitCount);
        return 1;
    }
    return 0;
}

int testNoInvalids() {
    int numLogs = 6;
    ScanRecord *logs[] = {
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT),
            createScanRecord("Mary", ENTER),
            createScanRecord("Mary", EXIT)
    };
    char **entries = malloc(sizeof(char *) * numLogs);
    int entryCount = 0;
    char **exits = malloc(sizeof(char *) * numLogs);
    int exitCount = 0;
    getInvalidLogs(logs, numLogs, entries, &entryCount, exits, &exitCount);

    free(entries);
    free(exits);
    if (entryCount != 0) {
        printf("No entries should be found. %d found\n", entryCount);
        return 1;
    }
    if (exitCount != 0) {
        printf("No exits should be found. %d found\n", exitCount);
        return 1;
    }
    return 0;
}

int testFaultyExits() {
    int numLogs = 5;
    ScanRecord *logs[] = {
            createScanRecord("Paul", ENTER),
            createScanRecord("Mary", ENTER),
            createScanRecord("Mary", EXIT),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT)
    };
    char **entries = malloc(sizeof(char *) * numLogs);
    int entryCount = 0;
    char **exits = malloc(sizeof(char *) * numLogs);
    int exitCount = 0;
    getInvalidLogs(logs, numLogs, entries, &entryCount, exits, &exitCount);

    if (entryCount != 0) {
        free(entries);
        free(exits);
        printf("No entries should be found. %d found\n", entryCount);
        return 1;
    }
    if (exitCount != 1) {
        free(entries);
        free(exits);
        printf("1 missing exit should be found. %d found\n", exitCount);
        return 1;
    }
    if (strcmp(exits[0], "Paul") != 0) {
        free(entries);
        printf("Paul should be the sole exit fault found. %s found instead\n", exits[0]);
        free(exits);
        return 1;
    }
    free(entries);
    free(exits);
    return 0;
}

int testFaultyEntries() {
    int numLogs = 8;
    ScanRecord *logs[] = {
            createScanRecord("Paul", EXIT),
            createScanRecord("Mary", ENTER),
            createScanRecord("Mary", EXIT),
            createScanRecord("Gregory", EXIT),
            createScanRecord("Mary", ENTER),
            createScanRecord("Mary", EXIT),
            createScanRecord("Gregory", ENTER),
            createScanRecord("Gregory", EXIT),
    };
    char **entries = malloc(sizeof(char *) * numLogs);
    int entryCount = 0;
    char **exits = malloc(sizeof(char *) * numLogs);
    int exitCount = 0;
    getInvalidLogs(logs, numLogs, entries, &entryCount, exits, &exitCount);

    if (entryCount != 2) {
        free(entries);
        free(exits);
        printf("2 missing entries should be found. %d found\n", entryCount);
        return 1;
    }
    if (exitCount != 0) {
        free(entries);
        free(exits);
        printf("No exits should be found. %d found\n", exitCount);
        return 1;
    }
    insertionSort(entries, entryCount);
    if (strcmp(entries[0], "Gregory") != 0 || strcmp(entries[1], "Paul") != 0) {
        printf("Gregory and Paul should be found as missing entries. %s and %s found instead\n", entries[0],
               entries[1]);
        free(exits);
        free(entries);
        return 1;
    }
    free(entries);
    free(exits);
    return 0;
}

int testFaultyEntriesAndExits() {
    int numLogs = 10;
    ScanRecord *logs[] = {
            createScanRecord("Paul", EXIT),
            createScanRecord("Mary", ENTER),
            createScanRecord("Mary", EXIT),
            createScanRecord("Ignatius", EXIT),
            createScanRecord("Benedict", ENTER),
            createScanRecord("Benedict", ENTER),
            createScanRecord("Benedict", EXIT),
            createScanRecord("Mary", ENTER),
            createScanRecord("Mary", EXIT),
            createScanRecord("Ignatius", ENTER),
    };
    char **entries = malloc(sizeof(char *) * numLogs);
    int entryCount = 0;
    char **exits = malloc(sizeof(char *) * numLogs);
    int exitCount = 0;
    getInvalidLogs(logs, numLogs, entries, &entryCount, exits, &exitCount);

    if (entryCount != 2) {
        free(entries);
        free(exits);
        printf("2 missing entries should be found. %d found\n", entryCount);
        return 1;
    }
    if (exitCount != 2) {
        free(entries);
        free(exits);
        printf("2 missing exits should be found. %d found\n", exitCount);
        return 1;
    }
    insertionSort(entries, entryCount);
    if (strcmp(entries[0], "Ignatius") != 0 || strcmp(entries[1], "Paul") != 0) {
        printf("Ignatius and Paul should be found as missing entries. %s and %s found instead\n", entries[0],
               entries[1]);
        free(exits);
        free(entries);
        return 1;
    }
    insertionSort(exits, entryCount);
    if (strcmp(exits[0], "Benedict") != 0 || strcmp(exits[1], "Ignatius") != 0) {
        printf("Benedict and Ignatius should be found as missing exits. %s and %s found instead\n", exits[0], exits[1]);
        free(exits);
        free(entries);
        return 1;
    }
    free(entries);
    free(exits);
    return 0;
}

int testOnlyOneFaultyExitPerEmployee() {
    int numLogs = 4;
    ScanRecord *logs[] = {
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", ENTER),
    };
    char **entries = malloc(sizeof(char *) * numLogs);
    int entryCount = 0;
    char **exits = malloc(sizeof(char *) * numLogs);
    int exitCount = 0;
    getInvalidLogs(logs, numLogs, entries, &entryCount, exits, &exitCount);

    if (entryCount != 0) {
        free(entries);
        free(exits);
        printf("No entries should be found. %d found\n", entryCount);
        return 1;
    }
    if (exitCount != 1) {
        free(entries);
        free(exits);
        printf("1 missing exit should be found. %d found\n", exitCount);
        return 1;
    }
    if (strcmp(exits[0], "Paul") != 0) {
        printf("Paul should be the sole exit fault found. %s found instead\n", exits[0]);
        free(entries);
        free(exits);
        return 1;
    }
    free(entries);
    free(exits);
    return 0;
}

int testOnlyOneFaultyEntryPerEmployee() {
    int numLogs = 6;
    ScanRecord *logs[] = {
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT),
    };
    char **entries = malloc(sizeof(char *) * numLogs);
    int entryCount = 0;
    char **exits = malloc(sizeof(char *) * numLogs);
    int exitCount = 0;
    getInvalidLogs(logs, numLogs, entries, &entryCount, exits, &exitCount);

    if (entryCount != 1) {
        free(entries);
        free(exits);
        printf("1 missing entry should be found. %d found\n", entryCount);
        return 1;
    }
    if (exitCount != 0) {
        free(entries);
        free(exits);
        printf("No exits should be found. %d found\n", exitCount);
        return 1;
    }
    if (strcmp(entries[0], "Paul") != 0) {
        printf("Paul should be the sole exit fault found. %s found instead\n", entries[0]);
        free(entries);
        free(exits);
        return 1;
    }
    free(entries);
    free(exits);
    return 0;
}

int testOnlyOneFaultyEntryAndExitPerEmployee() {
    int numLogs = 10;
    ScanRecord *logs[] = {
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", ENTER),
            createScanRecord("Paul", EXIT),
            createScanRecord("Paul", ENTER),
    };
    char **entries = malloc(sizeof(char *) * numLogs);
    int entryCount = 0;
    char **exits = malloc(sizeof(char *) * numLogs);
    int exitCount = 0;
    getInvalidLogs(logs, numLogs, entries, &entryCount, exits, &exitCount);

    if (entryCount != 1) {
        free(entries);
        free(exits);
        printf("1 missing entry should be found. %d found\n", entryCount);
        return 1;
    }
    if (exitCount != 1) {
        free(entries);
        free(exits);
        printf("1 missing exit should be found. %d found\n", exitCount);
        return 1;
    }
    if (strcmp(entries[0], "Paul") != 0) {
        printf("Paul should be the sole entry fault found. %s found instead\n", entries[0]);
        free(entries);
        free(exits);
        return 1;
    }

    if (strcmp(exits[0], "Paul") != 0) {
        printf("Paul should be the sole exit fault found. %s found instead\n", exits[0]);
        free(entries);
        free(exits);
        return 1;
    }
    free(entries);
    free(exits);
    return 0;
}

void runTests(void) {
    int (*tests[NUM_TESTS])() = {
            testNoInvalidsEmpty,
            testNoInvalids,
            testFaultyExits,
            testFaultyEntries,
            testFaultyEntriesAndExits,
            testOnlyOneFaultyExitPerEmployee,
            testOnlyOneFaultyEntryPerEmployee,
            testOnlyOneFaultyEntryAndExitPerEmployee
    };
    char *testNames[NUM_TESTS] = {
            "Function finds no invalid entries for empty input",
            "Function finds no invalid entries for valid input",
            "Function catches missing exit log",
            "Function catches missing entry logs",
            "Function catches missing entry and exit logs",
            "Only one entry per employee in the exit array",
            "Only one entry per employee in the entry array",
            "Only one entry per employee in both the entry and exit arrays"
    };
    int i = 0, successes = 0, failures = 0;
    for (; i < NUM_TESTS; i++) {
        int error = tests[i]();
        if (error) {
            printf("Test failed: %s\n", testNames[i]);
            failures++;
        } else {
            successes++;
        }
    }
    if (failures) printf("\n---\n");

    printf("Tests passed: %d (%.2f%%)\nTests failed: %d (%.2f%%)\n",
           successes,
           100.0 * successes / NUM_TESTS,
           failures,
           100.0 * failures / NUM_TESTS
    );
}

int main(void) {
    runTests();
}
