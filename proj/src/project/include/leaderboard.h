#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

#include "font.h"
#include "project_functions.h"

/**
 * @defgroup leaderboard leaderboard
 * @ingroup proj
 * @brief Leaderboard struct and functions
 *
 * @{
 */

typedef struct leaderboard leaderboard_t;

typedef struct entry entry_t;

/**
 * Leaderboard constructor
 * @param font font struct pointer
 * @return Leaderboard struct pointer
 */
leaderboard_t* (leaderboard_constructor)(font_t *font);
/**
 * Leaderboard destructor
 * @param lb Leaderboard struct pointer
 */
void (leaderboard_destructor)(leaderboard_t *lb);
/**
 * Draw Leaderboard to the screen
 * @param lb Leaderboard struct pointer
 */
void (leaderboard_draw)(leaderboard_t *lb);
/**
 * Draw end game screen depending on if player is alive or dead
 * @param lb Leaderboard struct pointer
 * @param playerName player string name
 * @param alive true is player alive false otherwise
 */
void (gameended_draw)(leaderboard_t *lb, char* playerName, bool alive);
/**
 * Save new entry with name and time score to the Leaderboard
 * @param lb Leaderboard struct pointer
 * @param playerName c string of the player name
 * @param timeCounter time used by the player
 */
void (leaderboard_save_file)(leaderboard_t *lb, char* playerName, int timeCounter);
/**
 * Sort the leaderboard entries using qsort funtion and entry struct
 * @param lb Leaderboard struct pointer
 */
void(leaderboard_sort_entries)(leaderboard_t *lb);
/**
 * Entry constructor
 * @param time time in the leaderboard entry
 * @param line c string of the entire entry line 
 */
entry_t* (entry_constructor)(int time, char* line);
/**
 * Entry destructor 
 */
void (entry_destructor)(entry_t* entry);

/**
 * @}
 */
#endif

