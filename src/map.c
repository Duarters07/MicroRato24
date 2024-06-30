#include "map.h"
#include "mr32.h"

#define true 1
#define false 0

struct {
    int length;
    int finished;
    char *nextTurn;
    char path[1000];
} map;

/**
 * Initializes the map.
 */
void initMap() {
    map.length = 0;
    map.finished = false;
    map.nextTurn = map.path;
}

/**
 * Simplifies the map path.
 */
void simplifyMap() {
    if (map.length < 3)
        return;
    char *pathPtr = map.path;
    char *lastPtr = pathPtr + map.length - 1;
    char *secondLastPtr = lastPtr - 1;
    char *thirdLastPtr = secondLastPtr - 1;
    if (*secondLastPtr != 'B')
        return;

    if (*lastPtr == 'L') {
        if (*thirdLastPtr == 'L') {
            *thirdLastPtr = 'F';
        } else if (*thirdLastPtr == 'F') {
            *thirdLastPtr = 'R';
        } else if (*thirdLastPtr == 'R') {
            *thirdLastPtr = 'B';
        } else
            return;
    } else if (*lastPtr == 'F') {
        if (*thirdLastPtr == 'F') {
            *thirdLastPtr = 'B';
        } else if (*thirdLastPtr == 'L') {
            *thirdLastPtr = 'R';
        } else if (*thirdLastPtr == 'R') {
            *thirdLastPtr = 'L';
        } else
            return;
    } else if (*lastPtr == 'R') {
        if (*thirdLastPtr == 'L') {
            *thirdLastPtr = 'B';
        } else if (*thirdLastPtr == 'F') {
            *thirdLastPtr = 'L';
        } else if (*thirdLastPtr == 'R') {
            *thirdLastPtr = 'F';
        } else
            return;
    }
    *secondLastPtr = '\0';
    *lastPtr = '\0';
    map.length -= 2;
    simplifyMap();
}

/**
 * Appends a turn to the map.
 * \param turn The turn to append. Must be one of 'L', 'R', 'F', 'B'.
 */
void appendTurn(char turn) {
    if (map.finished)
        return;
    if (turn != 'L' && turn != 'R' && turn != 'F' && turn != 'B')
        return;
    // printf("Appending %c\n", turn);
    char *pathPtr = map.path;
    char *appendPtr = pathPtr + map.length;
    *appendPtr = turn;
    map.length++;
    // printf("Map: %s\n", map.path);
    simplifyMap();
    // printf("Map: %s\n\n", map.path);
}

/**
 * Returns the map path.
 * \return The map path.
 */
char *getMapPath() { return map.path; }

/**
 * Returns the map length.
 * \return The map length.
 */
int getMapLength() { return map.length; }

/**
 * Returns whether the map is finished.
 * \return Whether the map is finished.
 */
int isMapFinished() { return map.finished; }

/**
 * Sets the map to be finished.
 */
void setMapFinished() { map.finished = true; }

/**
 * Returns the next turn.
 * Only available after the map is finished.
 * \return The char for the next turn.
 */
char getNextTurn() {
    if (!map.finished)
        return '\0';
    printf("Popping: %c\n", *map.nextTurn);
    printf("Map: %s\n", map.path);
    return *map.nextTurn++;
}

/**
 * Resets the next turn for the initial one.
 */
void resetNextTurn() { map.nextTurn = map.path; }
