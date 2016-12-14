/** @file
 * Interface of parser.
 */

#ifndef PARSE_H
#define PARSE_H

/**
 * Structing for holding commands
 * name - Name of command (possible commands: INIT, MOVE, PRODUCE_PEASANT, PRODUCE_KNIGHT, END_TURN)
 * data - Array of arguments for command
 */
typedef struct def_command {
	char name[16];
	int data[7];
} command;


/** 
 * Reads a command.
 * Returns command with data points using 'command' structure.
 */
command* parse_command();

/**
 * Checks if error code is one of following numbers: 1, 2, 3, 42. Any of these codes mean the end of program.
 * If 42, prints on stderr "input error"
 * @param[in] error_code Checked error code.
 * @return 0 if no suspicious error code detected, game should go on.
 * @return 1 if AI won, game should stop.
 * @return 2 if there was draw, game should stop.
 * @return 3 if AI lost, game should stop.
 * @return 42 if input error detected, game should stop.
 */
short error_check(int error_code);

/**
 * Prints "END_TURN" with newline
 */
void print_end_turn();

void print_produce_peasant(int x1, int y1, int x2, int y2);

void print_produce_knight(int x1, int y1, int x2, int y2);

void print_move(int x1, int y1, int x2, int y2);
#endif /* PARSE_H */