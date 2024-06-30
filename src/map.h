#ifndef MAP_H
#define MAP_H

/**
 * Initializes the map.
 */
void initMap();

/**
 * Appends a turn to the map.
 * \param turn The turn to append. Must be one of 'L', 'R', 'F', 'B'.
 */
void appendTurn(char turn);

/**
 * Returns the map path.
 * \return The map path.
 */
char *getMapPath();

/**
 * Returns the map length.
 * \return The map length.
 */
int getMapLength();

/**
 * Returns whether the map is finished.
 * \return Whether the map is finished.
 */
int isMapFinished();

/**
 * Sets the map to be finished.
 */
void setMapFinished();

/**
 * Returns the next turn.
 * Only available after the map is finished.
 * \return The char for the next turn.
 */
char getNextTurn();

/**
 * Resets the next turn for the initial one.
 */
void resetNextTurn();

#endif // MAP_H
