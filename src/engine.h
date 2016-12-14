/** 
 * @file
 * Interface of game engine.
 */

#ifndef ENGINE_H
#define ENGINE_H

/**
 * @brief Initializes a game. Needed before first INIT.
 */
void start_game();

/**
 * @brief Frees memory. Needed after finishing game.
 */
void end_game();

/**
 * @brief Initializes a game with size of a board, number of rounds and positions of kings.
 * @param[in] n Size of a board.
 * @param[in] k Maximum number of rounds.
 * @param[in] p Number of player (1 or 2) initializing the game.
 * @param[in] x1 Column number of first king's position
 * @param[in] y1 Row number of first king's position
 * @param[in] x1 Column number of second king's position
 * @param[in] y1 Row number of second king's position
 * @return 0 - Successful init
 * @return 42 - There was an error during the command.
 */
int init(int n, int k, int p, int x1, int y1, int x2, int y2);

/**
 * @brief Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return 0 - Normal move - no king was killed during it.
 * @return 1 - AI won during the move (player two's king was killed and player one's not).
 * @return 2 - AI lost during the move (player one's king was killed and player two's not).
 * @return 3 - Draw (both kings killed each other).
 * @return 42 - There was an error during the command.
 */
int move(int x1, int y1, int x2, int y2);

/**
 * @brief Tells peasant to produce a knight.
 * @param[in] x1 Column number of producing peasant.
 * @param[in] y1 Row number of producing peasant.
 * @param[in] x2 Column number of produced knight.
 * @param[in] y2 Row number of produced knight.
 * @return 0 - Knight was produced successfully.
 * @return 42 - There was an error during the command.
 */
int produce_knight(int x1, int y1, int x2, int y2);

/**
 * @brief Tells peasant to produce a peasant.
 * @param[in] x1 Column number of producing peasant.
 * @param[in] y1 Row number of producing peasant.
 * @param[in] x2 Column number of produced peasant.
 * @param[in] y2 Row number of produced peasant.
 * @return 0 - Peasant was produced successfully.
 * @return 42 - There was an error during the command.
 */
int produce_peasant(int x1, int y1, int x2, int y2);

/**
 * @brief Ends turn.
 * @return 0 - Turn ended normally.
 * @return 3 - Maximum turn reached. Returns state of draw.
 * @return 42 - There was an error during the command.
 */
int end_turn();

int move_knights_stage();

void peasant_production_stage();

#endif /* ENGINE_H */