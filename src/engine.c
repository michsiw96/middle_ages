/** @file
 * Implementation of game engine based on lists.s
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"

#define throw_error return 42

static int BOARD_SIZE;

static int max_turn_number;
static int turn_counter;

enum units_type {PEASANT, KING, KNIGHT};
enum which_player {PLAYER_ONE = 1, PLAYER_TWO = 2};

static enum which_player which_player_am_i;
static enum which_player which_player_turn = PLAYER_ONE;

// start_game() assert check - prevents from using start_game() twice
short does_game_started = 0;

// variables for init errors check
static short no_init_allowed = 0;
static int init_counter = 0;
static int king1_x = 0;
static int king1_y = 0;
static int king2_x = 0;
static int king2_y = 0;
static enum which_player player_first_init;

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

static unit_list *global_unit_list;

static void add_to_global_unit_list(int x, int y, enum which_player player,
	enum units_type unit_type, int last_move_turn) {
	unit *new_unit = (unit*)malloc(sizeof(unit));
	
	new_unit->x = x;
	new_unit->y = y;
	new_unit->player = player;
	new_unit->unit_type = unit_type;
	new_unit->last_move_turn = last_move_turn;
	
	unit_list *tmp = (unit_list*)malloc(sizeof(unit_list));
	tmp->unit = new_unit;
	tmp->next = global_unit_list;
	
	global_unit_list = tmp;
}

static void remove_unit_from_list(int x, int y) {
	assert(global_unit_list != NULL);
	unit_list *list = global_unit_list;
	
	if (global_unit_list->unit->x == x && global_unit_list->unit->y == y) {
		global_unit_list = global_unit_list->next;
		free(list->unit);
		free(list);
		return;
	}
	
	short tmp_bool = 1;
	while (list->next != NULL && tmp_bool) {
		if (list->next->unit->x == x && list->next->unit->y == y) {
			tmp_bool = 0;
		}
		else {
			list = list->next;
		}
	}
	
	if (list->next == NULL) return;
	else {
		unit_list *tmp_to_free = list->next;
		list->next = list->next->next;
		free(tmp_to_free->unit);
		free(tmp_to_free);
	}
}

static unit_list* find_unit_place_in_list(int x, int y) {
	unit_list *tmp = global_unit_list;
	
	while (tmp != NULL) {
		if (tmp->unit->x == x && tmp->unit->y == y) {
			return tmp;
		}
		tmp = tmp->next;
	}
	
	return NULL;
}

static unit* find_unit_in_list(int x, int y) {
	unit_list* tmp = find_unit_place_in_list(x, y);
	
	if (tmp == NULL) {
		return NULL;
	}
	else {
		return tmp->unit;
	}
}

void start_game() {
	global_unit_list = NULL;
	turn_counter = 0;
	
	assert(!does_game_started);
	
	does_game_started = 1;
}

void end_game() {
	unit_list *tmp;
	
	while (global_unit_list != NULL) {
		tmp = global_unit_list;
		global_unit_list = global_unit_list->next;
		free(tmp->unit);
		free(tmp);
	}
}

int init(int n, int k, int p, int x1, int y1, int x2, int y2) {
	if (no_init_allowed) {
		throw_error;
	}
	
	if (!(p == 1 || p == 2)) {
		throw_error;
	}
	
	which_player_am_i = p;
	
	if (n <= 8) {
		throw_error;
	}
	
	int x_distance;
	
	if (x1 - x2 > 0) {
		x_distance = x1 - x2;
	}
	else {
		x_distance = x2 - x1;
	}
	
	int y_distance;
	
	if (y1 - y2 > 0) {
		y_distance = y1 - y2;
	}
	else {
		y_distance = y2 - y1;
	}
	
	if (x_distance + y_distance < 8) {
		throw_error;
	}
	
	if (init_counter == 0) {
		BOARD_SIZE = n;
		max_turn_number = k;
		
		king1_x = x1;
		king1_y = y1;
		king2_x = x2;
		king2_y = y2;
		
		if (p == 1 || p == 2) {
			player_first_init = p;
		}
		else {
			throw_error;
		}
		
		if (x1 + 3 > n || y1 > n || x2 + 3 > n || y2 > n) {
			throw_error;
		}
		else {
			add_to_global_unit_list(x1, y1, PLAYER_ONE, KING, 0);
			add_to_global_unit_list(x1 + 1, y1, PLAYER_ONE, PEASANT, 0);
			add_to_global_unit_list(x1 + 2, y1, PLAYER_ONE, KNIGHT, 0);
			add_to_global_unit_list(x1 + 3, y1, PLAYER_ONE, KNIGHT, 0);
			add_to_global_unit_list(x2, y2, PLAYER_TWO, KING, 0);
			add_to_global_unit_list(x2 + 1, y2, PLAYER_TWO, PEASANT, 0);
			add_to_global_unit_list(x2 + 2, y2, PLAYER_TWO, KNIGHT, 0);
			add_to_global_unit_list(x2 + 3, y2, PLAYER_TWO, KNIGHT, 0);
		}
		
		turn_counter = 1;
	}
	else {
		throw_error;
	}
	
	++init_counter;
	
	return 0;
}

static void set_unit_position(unit *some_unit, int new_x, int new_y) {
	if (some_unit == NULL) return;
	
	some_unit->x = new_x;
	some_unit->y = new_y;
	some_unit->last_move_turn = turn_counter;
}

//standard check for functions
static unit* standard_check(int x1, int y1, int x2, int y2) {
	if ((x1 - x2) * (x1 - x2) > 1 || (y1 - y2) * (y1 - y2) > 1 ||
		x1 <= 0 || y1 <= 0 || x2 <= 0 || y2 <= 0 ||
		x1 > BOARD_SIZE || y1 > BOARD_SIZE || x2 > BOARD_SIZE || y2 > BOARD_SIZE) {
		return NULL;
	}
		
	unit *moving_unit = find_unit_in_list(x1, y1);
	if (moving_unit == NULL) {
		return NULL;
	}
	
	if (moving_unit->player != which_player_turn) {
		return NULL;
	}
	
	if (moving_unit->last_move_turn == turn_counter) {
		return NULL;
	}
	
	return moving_unit;
}

int move(int x1, int y1, int x2, int y2) {
	no_init_allowed = 1;
	if (init_counter != 1) {
		throw_error;
	}
	
	unit *moving_unit = standard_check(x1, y1, x2, y2);
	if (moving_unit == NULL) {
		throw_error;
	}
	
	unit *possible_unit_on_moved_tile = find_unit_in_list(x2, y2);
	
	if (possible_unit_on_moved_tile == NULL) {
		set_unit_position(moving_unit, x2, y2);
	}
	else if (moving_unit->player == possible_unit_on_moved_tile->player) {
		throw_error;
	}
	else {
		if (moving_unit->unit_type > possible_unit_on_moved_tile->unit_type) {
			enum units_type killed_unit_type = possible_unit_on_moved_tile->unit_type;
			remove_unit_from_list(x2, y2);
			set_unit_position(moving_unit, x2, y2);
			if (killed_unit_type == KING && which_player_turn == PLAYER_ONE) {
				if (which_player_am_i == PLAYER_ONE) return 1;
				else return 2;
			}
			if (killed_unit_type == KING && which_player_turn == PLAYER_TWO) {
				if (which_player_am_i == PLAYER_TWO) return 1;
				else return 2;
			}
		}
		else if (moving_unit->unit_type == possible_unit_on_moved_tile->unit_type) {
			enum units_type player_one_unit_type = moving_unit->unit_type;
			enum units_type player_two_unit_type = possible_unit_on_moved_tile->unit_type;
			remove_unit_from_list(x1, y1);
			remove_unit_from_list(x2, y2);
			if (player_one_unit_type == player_two_unit_type && player_one_unit_type == KING) {
				return 3;
			}
		}
		else {
			enum units_type killing_unit_type = moving_unit->unit_type;
			remove_unit_from_list(x1, y1);
			if (killing_unit_type == KING && which_player_turn == PLAYER_TWO) {
				if (which_player_am_i == PLAYER_ONE) return 1;
				else return 2;
			}
			if (killing_unit_type == KING && which_player_turn == PLAYER_ONE) {
				if (which_player_am_i == PLAYER_TWO) return 1;
				else return 2;
			}
		}
	}
	
	return 0;
}

//standard check for both functions produce_knight and produce_peasant
static short peasant_check(unit *checked_unit, int x2, int y2) {
	no_init_allowed = 1;
	if (init_counter != 1) {
		throw_error;
	}
	
	if (checked_unit == NULL) {
		return 0;
	}
	if (checked_unit->unit_type != PEASANT) {
		return 0;
	}
	if (turn_counter - checked_unit->last_move_turn < 3) {
		return 0;
	}
	if (find_unit_in_list(x2, y2) != NULL) {
		return 0;
	}
	
	return 1;
}

int produce_knight(int x1, int y1, int x2, int y2) {
	unit *actual_unit = standard_check(x1, y1, x2, y2);
	if (!peasant_check(actual_unit, x2, y2)) {
		throw_error;
	}
	else {
		add_to_global_unit_list(x2, y2, which_player_turn, KNIGHT, turn_counter - 1);
		actual_unit->last_move_turn = turn_counter;
	}
	
	return 0;
}

int produce_peasant(int x1, int y1, int x2, int y2) {
	unit *actual_unit = standard_check(x1, y1, x2, y2);
	if (!peasant_check(actual_unit, x2, y2)) {
		throw_error;
	}
	else {
		add_to_global_unit_list(x2, y2, which_player_turn, PEASANT, turn_counter - 1);
		actual_unit->last_move_turn = turn_counter;
	}
	
	return 0;
}

int end_turn() {
	no_init_allowed = 1;
	
	if (init_counter != 1) {
		throw_error;
	}
	
	if (which_player_turn == PLAYER_ONE) {
		which_player_turn = PLAYER_TWO;
	}
	else {
		which_player_turn = PLAYER_ONE;
		++turn_counter;
	}
	
	if (turn_counter > max_turn_number) {
		return 3;
	}
	
	return 0;
}

static int distance_counter(int x1, int y1, int x2, int y2) {
	int x_distance;
	
	if (x1 - x2 > 0) {
		x_distance = x1 - x2;
	}
	else {
		x_distance = x2 - x1;
	}
	
	int y_distance;
	
	if (y1 - y2 > 0) {
		y_distance = y1 - y2;
	}
	else {
		y_distance = y2 - y1;
	}
	
	return x_distance + y_distance;
}

static int* best_move_position(int x, int y) {
	int x_best_move = -1;
	int y_best_move = -1;
	
	int adjacent_tiles[8][2] = { 
		{ x - 1, y}, { x + 1, y}, { x, y - 1 }, { x, y + 1 },
		{ x - 1, y - 1 }, { x + 1, y - 1 }, { x - 1, y + 1 }, { x + 1, y + 1 } };
		 
	int x_closest = -1;
	int y_closest = -1;
	
	int distance_to_closest = -1;
	
	for (unit_list* tmp = global_unit_list; tmp != NULL; tmp = tmp->next) {
		if (tmp->unit->player != which_player_am_i) {
// 			printf("check4\n");
			
			if (distance_to_closest == -1) {
				x_closest = tmp->unit->x;
				y_closest = tmp->unit->y;
				
				distance_to_closest = distance_counter(x, y, tmp->unit->x, tmp->unit->y);
			}
			else {
				if (distance_counter(x, y, tmp->unit->x, tmp->unit->y) < distance_to_closest) {
					x_closest = tmp->unit->x;
					y_closest = tmp->unit->y;
					
					distance_to_closest = distance_counter(x, y, tmp->unit->x, tmp->unit->y);
				}
			}
		}
	}
	
	distance_to_closest = -1;
	
	for (int q = 0; q < 8; q++) { 
		if (adjacent_tiles[q][0] < 0 || adjacent_tiles[q][1] < 0 ||
			adjacent_tiles[q][0] > BOARD_SIZE || adjacent_tiles[q][1] > BOARD_SIZE) {
			continue;
		}
		
		unit* tmp = find_unit_in_list(adjacent_tiles[q][0], adjacent_tiles[q][1]);
		if (tmp != NULL) {
			if (tmp->player == which_player_am_i) {
				continue;
			}
		}
		
		int measured_distance = 
			distance_counter(adjacent_tiles[q][0], adjacent_tiles[q][1], x_closest, y_closest);
			
		if (distance_to_closest == -1) {
			distance_to_closest = measured_distance;
		}
		
		if (measured_distance <= distance_to_closest) {
			x_best_move = adjacent_tiles[q][0];
			y_best_move = adjacent_tiles[q][1];
			
			distance_to_closest = measured_distance;
		}
	}
	
	int *position = (int*)malloc(sizeof(int*) * 2);
	position[0] = x_best_move;
	position[1] = y_best_move;
	
	return position;
}

int move_knights_stage() {
	int error_code = 0;
	
	for (unit_list* tmp = global_unit_list; tmp != NULL; tmp = tmp->next) {
		if (tmp->unit->player == which_player_am_i && tmp->unit->unit_type == KNIGHT) {
			int *position = best_move_position(tmp->unit->x, tmp->unit->y);
			if (position[0] != -1 && position[1] != -1) {
				printf("MOVE %d %d %d %d\n", tmp->unit->x, tmp->unit->y, position[0], position[1]);
				error_code = move(tmp->unit->x, tmp->unit->y, position[0], position[1]);
				fflush(stdout);
				assert(error_code != 42);
			}
			free(position);
			if (error_code != 0) {
				break;
			}
		}
	}
	
	if (error_code == 0) {
		error_code = end_turn();
		printf("END_TURN\n");
		fflush(stdout);
	}
	return error_code;
}

void peasant_production_stage() {
	int error_code = 0;
	
	for (unit_list* tmp = global_unit_list; tmp != NULL; tmp = tmp->next) {
		if (tmp->unit->player == which_player_am_i && tmp->unit->unit_type == PEASANT) {
			int *position = best_move_position(tmp->unit->x, tmp->unit->y);
			if (turn_counter == 3) {
				error_code = produce_peasant(tmp->unit->x, tmp->unit->y, position[0], position[1]);
				if (error_code != 42) {
					printf("PRODUCE_PEASANT %d %d %d %d\n", tmp->unit->x, tmp->unit->y, position[0], position[1]);
					fflush(stdout);
				}
			}
			if (position[0] != -1 && position[1] != -1) {
				error_code = produce_knight(tmp->unit->x, tmp->unit->y, position[0], position[1]);
				if (error_code != 42) {
					printf("PRODUCE_KNIGHT %d %d %d %d\n", tmp->unit->x, tmp->unit->y, position[0], position[1]);
					fflush(stdout);
				}
			}
			free(position);
		}
	}
}

// functions for testing 

static unit_list* for_test_add_to_list(int x, int y, enum which_player player,
	enum units_type unit_type, int last_move_turn, unit_list *list_to_add) {
	
	unit *new_unit = (unit*)malloc(sizeof(unit));
	
	new_unit->x = x;
	new_unit->y = y;
	new_unit->player = player;
	new_unit->unit_type = unit_type;
	new_unit->last_move_turn = last_move_turn;
	
	unit_list *tmp = (unit_list*)malloc(sizeof(unit_list));
	tmp->unit = new_unit;
	tmp->next = list_to_add;
	
	return tmp;
}

static void for_test_prepare_global_variables(int n, int k, enum which_player p, int t) {
	BOARD_SIZE = n;
	max_turn_number = k;
	which_player_am_i = p;
	turn_counter = t;
	init_counter = 1;
	which_player_turn = p;
}

static void for_test_set_global_unit_list(unit_list *list_replacing) {
	global_unit_list = list_replacing;
}