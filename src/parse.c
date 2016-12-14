/** @file
 * Implementation of parser.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define throw_parse_error free(parsed_command); return NULL

typedef struct def_command {
	char name[16];
	int data[7];
} command;

// converts string to positive int, returns -1 if not possible
static int string_to_posivite_int(char probably_number[]) {
	int m = (int)strlen(probably_number);
	
	for (int i = 0; i < m; i++) {
		if (probably_number[i] < '0' || probably_number[i] > '9') {
			return -1;
		}
	}
	
	if (probably_number[0] == '0') {
		return -1;
	}
	
	short is_below_max_int = 1;
	
	if (m > 10) {
		is_below_max_int = 0;
	}
	else if (m == 10) {
		char char_max_int[10] = "2147483647";
		for (int i = 0; i < 10; i++) {
			if (char_max_int[i] > probably_number[i]) {
				is_below_max_int = 2;
			}
			if (char_max_int[i] < probably_number[i] && is_below_max_int != 2) {
				is_below_max_int = 0;
			}
		}
	}
	
	if (is_below_max_int == 0) {
		return -1;
	}
	else {
		int res = 0;
		int cipher;
		for (int i = 0; i < m; i++) {
			cipher = ((int)probably_number[i]) - 48;
			res = (10 * res) + cipher;
		}
		return res;
	}
}

//common part of code for every parseable command in parse_command()
static int* extract_data_from_string(char buffer[], int name_end_token, int number_of_data) {
	int *extracted_data = (int*)malloc(sizeof(int) * number_of_data);
	
	int prev_space = -1;
	int actual_space = name_end_token;
	char substring[101];
	
	for (int i = 0; i < number_of_data; i++) {
		prev_space = actual_space;
		actual_space = (int)strcspn(buffer+(prev_space+1), " \n\0") + (prev_space+1);
		
		if (buffer[prev_space] == '\0' || buffer[prev_space] == '\n') {
			free(extracted_data);
			return NULL;
		}
		strncpy(substring, buffer + prev_space + 1, actual_space - prev_space - 1);
		substring[actual_space - prev_space - 1] = '\0';
		extracted_data[i] = string_to_posivite_int(substring);
		if (extracted_data[i] == -1) {
			free(extracted_data);
			return NULL;
		}
	}
	
	if (buffer[actual_space] == '\0' || buffer[actual_space] == '\n') {
		return extracted_data;
	}
	else {
		free(extracted_data);
		return NULL;
	}
}

command* parse_command() {
	command *parsed_command = (command*)malloc(sizeof(command));
	parsed_command->name[0] = '\0';
	
	char buffer[102];
	buffer[101] = '\0';
	if(fgets(buffer, 100, stdin));
	
	int name_end_token = (int)strcspn(buffer, " \n\0");
	if (name_end_token > 16) {
		throw_parse_error;
	}
	else {
		for (int i = 0; i < name_end_token; i++) {
			parsed_command->name[i] = buffer[i];
		}
		parsed_command->name[name_end_token] = '\0';
		if (strcmp(parsed_command->name, "INIT") == 0) {
			int *data = extract_data_from_string(buffer, name_end_token, 7);
			if (data == NULL) {
				throw_parse_error;
			}
			else {
				for (int i = 0; i < 7; i++) {
					parsed_command->data[i] = *(data+i);
				}
				free(data);
				return parsed_command;
			}
		}
		else if (strcmp(parsed_command->name, "MOVE") == 0) {
			int *data = extract_data_from_string(buffer, name_end_token, 4);
			if (data == NULL) {
				throw_parse_error;
			}
			else {
				for (int i = 0; i < 4; i++) {
					parsed_command->data[i] = *(data+i);
				}
				free(data);
				return parsed_command;
			}
		}
		else if (strcmp(parsed_command->name, "PRODUCE_KNIGHT") == 0) {
			int *data = extract_data_from_string(buffer, name_end_token, 4);
			if (data == NULL) {
				throw_parse_error;
			}
			else {
				for (int i = 0; i < 4; i++) {
					parsed_command->data[i] = *(data+i);
				}
				free(data);
				return parsed_command;
			}
		}
		else if (strcmp(parsed_command->name, "PRODUCE_PEASANT") == 0) {
			int *data = extract_data_from_string(buffer, name_end_token, 4);
			if (data == NULL) {
				throw_parse_error;
			}
			else {
				for (int i = 0; i < 4; i++) {
					parsed_command->data[i] = *(data+i);
				}
				free(data);
				return parsed_command;
			}
		}
		else if (strcmp(parsed_command->name, "END_TURN") == 0) {
			if (buffer[8] == '\0' || buffer[8] == '\n') {
				return parsed_command;
			}
			else {
				throw_parse_error;
			}
		}
		else {
			throw_parse_error;
		}
	}
}

short error_check(int error_code) {
	if (error_code == 42) {
		fprintf(stderr, "input error\n");
		return 42;
	}
	if (error_code == 1) {
		//ai won
		return 1;
	}
	if (error_code == 2) {
		//ai lost
		return 3;
	}
	if (error_code == 3) {
		//draw
		return 2;
	}
	return 0;
}

void print_end_turn() {
	printf("END_TURN\n");
	fflush(stdout);
}

void print_produce_peasant(int x1, int y1, int x2, int y2) {
	printf("PRODUCE_PEASANT %d %d %d %d\n", x1, y1, x2, y2);
	fflush(stdout);
}

void print_produce_knight(int x1, int y1, int x2, int y2) {
	printf("PRODUCE_KNIGHT %d %d %d %d\n", x1, y1, x2, y2);
	fflush(stdout);
}

void print_move(int x1, int y1, int x2, int y2) {
	printf("MOVE %d %d %d %d\n", x1, y1, x2, y2);
	fflush(stdout);
}

//function for testing

static void free_command(command *command_to_free) {
	free(command_to_free);
}