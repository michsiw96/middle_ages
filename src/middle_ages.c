/** @file
 * Main source file of the AI - uses functions from parse.h/c and engine.h/c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parse.h"
#include "engine.h"

#define throw_input_error_code end_game(); fflush(stdout); return 42

/**
 * @brief Main function of the AI.
 * @return 0 - AI won the game.
 * @return 1 - Draw in the game.
 * @return 2 - AI lost the game.
 * @return 42 - There was some input error.
 */
int main() {
	command *new_command;
	short error_code, is_game_still_played = 1, ai_exit_code;
	short is_ai_turn, which_player_is_ai;
	
	start_game();
	while (is_game_still_played) {
		if (is_ai_turn == 1) {
			is_ai_turn = 0;
			peasant_production_stage();
			error_code = error_check(move_knights_stage());
			if (error_code == 42) {
				throw_input_error_code;
			}
			else if (error_code == 1 || error_code == 2 || error_code == 3) {
				is_game_still_played = 0;
				ai_exit_code = error_code - 1;
				continue;
			}
			else if (error_code != 0) {
				assert(0);
			}
		}
		
		new_command = parse_command();
		
		if (new_command->name == NULL) {
			error_check(42);
			free(new_command);
			throw_input_error_code;
		}
		else if (strcmp(new_command->name, "INIT") == 0) {
			error_code = error_check(init(new_command->data[0],
			                              new_command->data[1],
			                              new_command->data[2],
			                              new_command->data[3],
			                              new_command->data[4],
			                              new_command->data[5],
			                              new_command->data[6]));
			if (new_command->data[2] == 1) {
				is_ai_turn = 1;
				which_player_is_ai = 1;
			}
			else {
				is_ai_turn = 0;
				which_player_is_ai = 2;
			}
			free(new_command);
			if (error_code == 42) {
				throw_input_error_code;
			}
			else if (error_code == 1 || error_code == 2 || error_code == 3) {
				is_game_still_played = 0;
				ai_exit_code = error_code - 1;
			}
			else if (error_code != 0) {
				assert(0);
			}
		}
		else if (strcmp(new_command->name, "MOVE") == 0) {
			error_code = error_check(move(new_command->data[0],
			                              new_command->data[1],
			                              new_command->data[2],
			                              new_command->data[3]));
			free(new_command);
			if (error_code == 42) {
				throw_input_error_code;
			}
			else if (error_code == 1 || error_code == 2 || error_code == 3) {
				is_game_still_played = 0;
				ai_exit_code = error_code - 1;
			}
			else if (error_code != 0) {
				assert(0);
			}
		}
		else if (strcmp(new_command->name, "PRODUCE_KNIGHT") == 0) {
			error_code = error_check(produce_knight(new_command->data[0],
			                                        new_command->data[1],
			                                        new_command->data[2],
			                                        new_command->data[3]));
			free(new_command);
			if (error_code == 42) {
				throw_input_error_code;
			}
			else if (error_code == 1 || error_code == 2 || error_code == 3) {
				is_game_still_played = 0;
				ai_exit_code = error_code - 1;
			}
			else if (error_code != 0) {
				assert(0);
			}
		}
		else if (strcmp(new_command->name, "PRODUCE_PEASANT") == 0) {
			error_code = error_check(produce_peasant(new_command->data[0],
			                                         new_command->data[1],
			                                         new_command->data[2],
			                                         new_command->data[3]));
			free(new_command);
			if (error_code == 42) {
				throw_input_error_code;
			}
			else if (error_code == 1 || error_code == 2 || error_code == 3) {
				is_game_still_played = 0;
				ai_exit_code = error_code - 1;
			}
			else if (error_code != 0) {
				assert(0);
			}
		}
		else if (strcmp(new_command->name, "END_TURN") == 0) {
			is_ai_turn = 1;
			error_code = error_check(end_turn());
			free(new_command);
			if (error_code == 42) {
				throw_input_error_code;
			}
			else if (error_code == 1 || error_code == 2 || error_code == 3) {
				is_game_still_played = 0;
				ai_exit_code = error_code - 1;
			}
			else if (error_code != 0) {
				assert(0);
			}
		}
		else {
			assert(1);
		}
	}
	
	end_game();
	fflush(stdout);
	return ai_exit_code;
}