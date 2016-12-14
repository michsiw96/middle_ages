/** @file
 * File containing tests for middle_ages AI.
 */

#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>

#include <stdlib.h>
#include <string.h>

#include "cmocka.h"

char mock_stdin[256];

enum units_type {PEASANT, KING, KNIGHT};
enum which_player {PLAYER_ONE = 1, PLAYER_TWO = 2};

typedef struct unit {
	int x;
	int y;
	enum which_player player;
	enum units_type unit_type;
	int last_move_turn;
} unit;

typedef struct unit_list {
	struct unit *unit;
	struct unit_list *next;
} unit_list;

typedef struct def_command {
	char name[16];
	int data[7];
} command;

extern int move(int x1, int y1, int x2, int y2);

extern unit_list* for_test_add_to_list(int x, int y, enum which_player player,
	enum units_type unit_type, int last_move_turn, unit_list *list_to_add);

extern void for_test_prepare_global_variables(int n, int k, enum which_player p, int t);

extern void end_game();

extern void set_unit_position(unit *some_unit, int new_x, int new_y);

extern void for_test_set_global_unit_list(unit_list *list_replacing);

extern command* parse_command();

extern void free_command(command *command_to_free);

/**
 * @brief Mock function for fgets.
 */
char* mock_fgets(char* buffer, int MAX_SIZE, FILE * const file) {
	assert_true(file == stdin);
	if (strlen(mock_stdin) > MAX_SIZE) return NULL;
	strcpy(buffer, mock_stdin);
	return buffer;
}

// parse tests

/**
 * @brief Test with invalid input, two spaces in a row. (parse_command test)
 */
static void parse_test_two_spaces_in_a_row(void **state) {
	strcpy(mock_stdin, "MOVE 1  2 1 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with invalid input, argument not a number. (parse_command test)
 */
static void parse_test_argument_not_a_number(void **state) {
	strcpy(mock_stdin, "MOVE 1 a 1 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with invalid input, not enough arguments. (parse_command test)
 */
static void parse_test_not_enough_arguments(void **state) {
	strcpy(mock_stdin, "MOVE 1 1 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with invalid input, too much arguments. (parse_command test)
 */
static void parse_test_too_much_arguments(void **state) {
	strcpy(mock_stdin, "MOVE 1 5 2 1 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with invalid input, wrong command. (parse_command test)
 */
static void parse_test_wrong_command(void **state) {
	strcpy(mock_stdin, "MOVRE 1 2 1 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with invalid input, good command, 
 * but lowercase (parse_command test)
 */
static void parse_test_case_sensivite_command(void **state) {
	strcpy(mock_stdin, "move 1 2 1 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with invalid input, too big argument. (parse_command test)
 */
static void parse_test_too_big_argument(void **state) {
	strcpy(mock_stdin, "MOVE 1 7259387325998 1 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with invalid input, argument close to max int, 
 * but too big. (parse_command test)
 */
static void parse_test_almost_max_int(void **state) {
	strcpy(mock_stdin, "MOVE 1 1 2147483648 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with invalid input, negative argument. (parse_command test)
 */
static void parse_test_negative_argument(void **state) {
	strcpy(mock_stdin, "MOVE -1 2 1 3\n");
	
	command *tmp = parse_command();
	
	assert_null(tmp);
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with valid input, small arguments. (parse_command test)
 */
static void parse_test_right_command_1(void **state) {
	strcpy(mock_stdin, "MOVE 1 2 2 3\n");
	
	command *tmp = parse_command();
	
	if (tmp != NULL) {
		assert_int_equal(strcmp(tmp->name, "MOVE"), 0);
		assert_int_equal(tmp->data[0], 1);
		assert_int_equal(tmp->data[1], 2);
		assert_int_equal(tmp->data[2], 2);
		assert_int_equal(tmp->data[3], 3);
	} else {
		assert_non_null(tmp);
	}
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with valid input, medium arguments. (parse_command test)
 */
static void parse_test_right_command_2(void **state) {
	strcpy(mock_stdin, "MOVE 5214212 5214212 5214211 5214211\n");
	
	command *tmp = parse_command();
	
	if (tmp != NULL) {
		assert_int_equal(strcmp(tmp->name, "MOVE"), 0);
		assert_int_equal(tmp->data[0], 5214212);
		assert_int_equal(tmp->data[1], 5214212);
		assert_int_equal(tmp->data[2], 5214211);
		assert_int_equal(tmp->data[3], 5214211);
	} else {
		assert_non_null(tmp);
	}
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

/**
 * @brief Test with valid input, big arguments. (parse_command test)
 */
static void parse_test_right_command_3(void **state) {
	strcpy(mock_stdin, "MOVE 2147483647 2147483647 2147483646 2147483646\n");
	
	command *tmp = parse_command();
	
	if (tmp != NULL) {
		assert_int_equal(strcmp(tmp->name, "MOVE"), 0);
		assert_int_equal(tmp->data[0], 2147483647);
		assert_int_equal(tmp->data[1], 2147483647);
		assert_int_equal(tmp->data[2], 2147483646);
		assert_int_equal(tmp->data[3], 2147483646);
	} else {
		assert_non_null(tmp);
	}
	
	if (tmp != NULL) {
		free_command(tmp);
	}
}

// set_unit_position

/**
 * @brief Test setting unit's position to a
 * valid position. (set_unit_position test)
 */
static void test_set_unit_position_not_null(void **state) {
	unit *unit_to_set = (unit*)malloc(sizeof(unit));
	
	unit_to_set->x = 5;
	unit_to_set->y = 3;
	unit_to_set->player = PLAYER_TWO;
	unit_to_set->unit_type = PEASANT;
	unit_to_set->last_move_turn = 3;
	
	set_unit_position(unit_to_set, 8, 6);
	
	assert_int_equal(unit_to_set->x, 8);
	assert_int_equal(unit_to_set->y, 6);
	
	free(unit_to_set);
}

/**
 * @brief Test setting unit's position to an
 * invalid position. (set_unit_position test)
 */
static void test_set_unit_position_null(void **state) {
	unit *unit_to_set = NULL;
	
	set_unit_position(unit_to_set, 123, 456);
	assert_null(unit_to_set);
}

// MOVE gameplay tests

/**
 * @brief Test with invalid behaviour, move out 
 * of the left border. (move test)
 */
static void test_move_out_of_borders_left(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(1, 5, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(1, 5, 0, 5), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, move out 
 * of the right border. (move test)
 */
static void test_move_out_of_borders_right(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(10, 5, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(10, 5, 11, 5), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, move out 
 * of the up border. (move test)
 */
static void test_move_out_of_borders_up(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(5, 1, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(5, 1, 5, 0), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, move out 
 * of the bottom border. (move test)
 */
static void test_move_out_of_borders_bottom(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(5, 10, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(5, 10, 6, 11), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, move with not
 * own unit. (move test)
 */
static void test_move_wrong_player_unit(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(1, 1, PLAYER_TWO, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(1, 1, 1, 2), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, attack on
 * own unit. (move test)
 */
static void test_attack_on_own_unit(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_TWO, 1);
	tmp = for_test_add_to_list(1, 1, PLAYER_TWO, KNIGHT, 0, tmp);
	tmp = for_test_add_to_list(1, 2, PLAYER_TWO, KING, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(1, 1, 1, 2), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, move too far
 */
static void test_move_too_far(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(3, 9, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(3, 9, 2, 7), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, move to the same
 * spot. (move test)
 */
static void test_move_to_the_same_spot(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(7, 8, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(7, 8, 7, 8), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with valid behaviour, knight vs knight
 * fight. (move test)
 */
static void test_knight_vs_knight(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(4, 5, PLAYER_ONE, KNIGHT, 0, tmp);
	tmp = for_test_add_to_list(4, 6, PLAYER_TWO, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(4, 5, 4, 6), 0);
	assert_int_equal(move(4, 6, 4, 7), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with valid behaviour, knight vs peasant
 * fight. (move test)
 */
static void test_knight_vs_peasant(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(4, 5, PLAYER_ONE, KNIGHT, 0, tmp);
	tmp = for_test_add_to_list(4, 6, PLAYER_TWO, PEASANT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(4, 5, 4, 6), 0);
	assert_int_equal(move(4, 6, 4, 7), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with valid behaviour, knight vs king
 * fight - player 1 win. (move test)
 */
static void test_knight_vs_king_player_1_win(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(4, 5, PLAYER_ONE, KNIGHT, 0, tmp);
	tmp = for_test_add_to_list(4, 6, PLAYER_TWO, KING, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(4, 5, 4, 6), 1);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with valid behaviour, knight vs king
 * fight - player 2 win. (move test)
 */
static void test_knight_vs_king_player_2_win(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_TWO, 1);
	tmp = for_test_add_to_list(4, 5, PLAYER_TWO, KING, 0, tmp);
	tmp = for_test_add_to_list(4, 6, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(4, 5, 4, 6), 2);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with valid behaviour, peasant vs king
 * fight. (move test)
 */
static void test_peasant_vs_king(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(4, 5, PLAYER_ONE, PEASANT, 0, tmp);
	tmp = for_test_add_to_list(4, 6, PLAYER_TWO, KING, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(4, 5, 4, 6), 0);
	assert_int_equal(move(4, 6, 4, 7), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with valid behaviour, king vs king
 * fight. (move test)
 */
static void test_king_vs_king(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(4, 5, PLAYER_ONE, KING, 0, tmp);
	tmp = for_test_add_to_list(4, 6, PLAYER_TWO, KING, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(4, 5, 4, 6), 3);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with valid behaviour, normal, 
 * ordinary move. (move test)
 */
static void test_normal_move(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(1, 1, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(1, 1, 2, 2), 0);
	assert_int_equal(move(2, 2, 3, 3), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, move with the same
 * unit twice in one turn. (move test)
 */
static void test_move_two_times_same_unit(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(1, 1, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(1, 1, 2, 2), 0);
	assert_int_equal(move(2, 2, 3, 3), 42);
	
	tmp = for_test_add_to_list(4, 4, PLAYER_TWO, PEASANT, 2, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(4, 4, 5, 5), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

/**
 * @brief Test with invalid behaviour, move with not
 * existing unit. (mvoe test)
 */
static void test_move_not_existing_unit(void **state) {
	unit_list *tmp = NULL;
	for_test_prepare_global_variables(10, 20, PLAYER_ONE, 1);
	tmp = for_test_add_to_list(1, 1, PLAYER_ONE, KNIGHT, 0, tmp);
	for_test_set_global_unit_list(tmp);
	
	assert_int_equal(move(2, 2, 3, 3), 42);
	
	end_game();
	for_test_set_global_unit_list(NULL);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(parse_test_almost_max_int),
		cmocka_unit_test(parse_test_argument_not_a_number),
		cmocka_unit_test(parse_test_case_sensivite_command),
		cmocka_unit_test(parse_test_negative_argument),
		cmocka_unit_test(parse_test_not_enough_arguments),
		cmocka_unit_test(parse_test_right_command_1),
		cmocka_unit_test(parse_test_right_command_2),
		cmocka_unit_test(parse_test_right_command_3),
		cmocka_unit_test(parse_test_too_big_argument),
		cmocka_unit_test(parse_test_too_much_arguments),
		cmocka_unit_test(parse_test_two_spaces_in_a_row),
		cmocka_unit_test(parse_test_wrong_command),
		cmocka_unit_test(test_set_unit_position_not_null),
		cmocka_unit_test(test_set_unit_position_null),
		cmocka_unit_test(test_move_out_of_borders_left),
		cmocka_unit_test(test_move_out_of_borders_right),
		cmocka_unit_test(test_move_out_of_borders_up),
		cmocka_unit_test(test_move_out_of_borders_bottom),
		cmocka_unit_test(test_move_wrong_player_unit),
		cmocka_unit_test(test_attack_on_own_unit),
		cmocka_unit_test(test_move_too_far),
		cmocka_unit_test(test_move_to_the_same_spot),
		cmocka_unit_test(test_knight_vs_knight),
		cmocka_unit_test(test_knight_vs_peasant),
		cmocka_unit_test(test_knight_vs_king_player_1_win),
		cmocka_unit_test(test_knight_vs_king_player_2_win),
		cmocka_unit_test(test_peasant_vs_king),
		cmocka_unit_test(test_king_vs_king),
		cmocka_unit_test(test_normal_move),
		cmocka_unit_test(test_move_two_times_same_unit),
		cmocka_unit_test(test_move_not_existing_unit)
	};
	
	return cmocka_run_group_tests(tests, NULL, NULL);
}