#!/bin/bash

BOARD_SIZE=10
MAX_TURN=100
WAIT_TIME=1

PARAM_ACQUIRED=(0 0 0 0)

P1_PARAM=1,1
P2_PARAM=1,9

AI1=0
AI2=0

MAX_INT=2147483647

function do_nothing {
	echo "" > /dev/null
}

function error_usage {
	echo "error"
	exit 1
}

function check_borders {
	if (( $1 >= 1 && $1 <= MAX_INT )); then
		echo 1
	else
		echo 0
	fi
}

function check_int {
	if ! [[ "$1" =~ ^[0-9]+$ ]] || ! (( $(check_borders $1) )); then
		error_usage
	fi
}

# read input params in any order

while [[ $# -ge 2 ]]; do
	case "$1" in
		"-n"  ) BOARD_SIZE="$2";;
		"-k"  ) MAX_TURN="$2";;
		"-s"  ) WAIT_TIME="$2";;
		"-p1" ) PARAM_ACQUIRED[0]=1; P1_PARAM="$2";;
		"-p2" ) PARAM_ACQUIRED[1]=1; P2_PARAM="$2";;
		"-ai1") PARAM_ACQUIRED[2]=1; AI1="$2";;
		"-ai2") PARAM_ACQUIRED[3]=1; AI2="$2";;
	*) error_usage;;
	esac

    shift 2
done

if [[ $# != 0 ]]; then
	error_usage
fi

# check if ints below max_int

check_int $BOARD_SIZE
check_int $MAX_TURN
if ! (( WAIT_TIME == 0 )); then
	check_int $WAIT_TIME
fi
if ((${PARAM_ACQUIRED[0]} == 1)); then
	if ! [[ "$P1_PARAM" =~ ^[0-9]+,[0-9]+$ ]]; then
		error_usage
	else
		P1_X=${P1_PARAM%,*}
		P1_Y=${P1_PARAM#*,}
		
		check_int $P1_X
		check_int $P1_Y
	fi
fi
if ((${PARAM_ACQUIRED[1]} == 1)); then
	if ! [[ "$P2_PARAM" =~ ^[0-9]+,[0-9]+$ ]]; then
		error_usage
	else
		P2_X=${P2_PARAM%,*}
		P2_Y=${P2_PARAM#*,}
		
		check_int $P2_X
		check_int $P2_Y
	fi
fi
if ((${PARAM_ACQUIRED[2]} == 1)); then
	if [ ! -x $AI1 ]; then
		error_usage
	fi
fi
if ((${PARAM_ACQUIRED[3]} == 1)); then
	if [ ! -x $AI2 ]; then
		error_usage
	fi
fi

# now that we know params are parsed correctly,
# we can check if they match the rules of the game

if (( BOARD_SIZE < 9 )); then
	error_usage
fi

function rand_with_range {
	echo $(( $(( $(( $((RANDOM<<15)) + RANDOM )) % $(($2 - $1 + 1)) )) + $1))
}

function abs {
	if (( $1 < 0 )); then
		echo $(( (-1) * $1 ))
	else
		echo $1
	fi
}

# randomizing functions grant equal chance for possible fields on board

function generate_first_position {
	local possible_x=$(rand_with_range 1 $((BOARD_SIZE - 3)) )
	
	if (( possible_x < BOARD_SIZE - 11 )) || (( possible_x >= 9 )); then
		local possible_y=$(rand_with_range 1 BOARD_SIZE)
		echo $possible_x,$possible_y
	else
		local possible_y1=$(rand_with_range 1 $((BOARD_SIZE - 8)) )
		local possible_y2=$(rand_with_range 9 BOARD_SIZE)
		
		local y_choice=$(rand_with_range 0 1)
		
		case "$y_choice" in
			"0") echo $possible_x,$possible_y1;;
			"1") echo $possible_x,$possible_y2;;
		esac
	fi
}

function validate_given_from_user_position {
	if (( $2 > BOARD_SIZE )) || (( $1 > BOARD_SIZE - 3 )); then
		error_usage
	elif (( BOARD_SIZE - 3 - $1 < 8 )) && (( BOARD_SIZE - $2 < 8 )) &&
       (( $1 - 1 < 8 )) && (( $2 - 1 < 8 )); then
		error_usage
	fi
}

function generate_second_position {
	local possible_x=$(rand_with_range 1 $((BOARD_SIZE - 3)) )
	
	if (( $(abs $(( possible_x - $1 )) ) >= 8 )); then
		local possible_y=$(rand_with_range 1 BOARD_SIZE)
		echo $possible_x,$possible_y
		echo "lel" > error.out
	else
		local y_choice=$(rand_with_range 0 1)
		echo $y_choice > error.out
		
		if (( y_choice == 1 )); then
			if (( $2 + 8 <= BOARD_SIZE )); then
				local possible_y=$(rand_with_range $(( $2 + 8 )) $(( BOARD_SIZE )) )
				echo $possible_x,$possible_y
			else
				local possible_y=$(rand_with_range 1 $(( $2 - 8 )) )
				echo $possible_x,$possible_y
			fi
		else
			if (( $2 - 8 >= 1 )); then
				local possible_y=$(rand_with_range 1 $(( $2 - 8 )) )
				echo $possible_x,$possible_y
			else
				local possible_y=$(rand_with_range $(( $2 + 8 )) $(( BOARD_SIZE )) )
				echo $possible_x,$possible_y
			fi
		fi
	fi
}

if (( ${PARAM_ACQUIRED[0]} == 0 )) && (( ${PARAM_ACQUIRED[1]} == 0 )); then
	P1_PARAM=$(generate_first_position)
	P1_X=${P1_PARAM%,*}
	P1_Y=${P1_PARAM#*,}
	P2_PARAM=$(generate_second_position P1_X P1_Y)
	P2_X=${P2_PARAM%,*}
	P2_Y=${P2_PARAM#*,}
elif (( ${PARAM_ACQUIRED[1]} == 0)); then
	validate_given_from_user_position $P1_X $P1_Y
	P2_PARAM=$(generate_second_position P1_X P1_Y)
	P2_X=${P2_PARAM%,*}
	P2_Y=${P2_PARAM#*,}
elif (( ${PARAM_ACQUIRED[0]} == 0)); then
	validate_given_from_user_position $P2_X $P2_Y
	P1_PARAM=$(generate_second_position P2_X P2_Y)
	P1_X=${P1_PARAM%,*}
	P1_Y=${P1_PARAM#*,}
elif (( $(abs $(( P1_X - P2_X )) ) < 8 )) &&
     (( $(abs $(( P1_Y - P2_Y )) ) < 8 )); then
	error_usage
fi

# now part of actual gameplay

function create_fifo_with_given_descriptor {
	PIPE=$(mktemp -u)
	mkfifo "$PIPE"
	eval "exec $1<>$PIPE"
	rm $PIPE
}

# probably a little overkill (pun intended) to create function
# able to receive any number of arguments - we need it for max
# two arguments - but hey, why not
function kill_all_if_some_pid_dies {
	local list_to_kill=()
	local was_someone_killed=0
	for pid in $@; do
		if ps -p $pid > /dev/null 2>&1; then
			list_to_kill=(${list_to_kill[@]} $pid)
		else
			was_someone_killed=1
		fi
	done
	
	if (( was_someone_killed == 1 )); then
		sleep 1
		for pid in $list_to_kill; do
			kill $pid > /dev/null 2>&1
		done
		echo 0
	else
		echo 1
	fi
}

for ((i = 3; i <= 8; i++)); do
	create_fifo_with_given_descriptor $i
done

GLOBAL_WHICH_PLAYER_TURN=1
function change_player_turn {
	if (( WHICH_PLAYER_TURN == 1 )); then
		GLOBAL_WHICH_PLAYER_TURN=2
	elif (( WHICH_PLAYER_TURN == 2 )); then
		GLOBAL_WHICH_PLAYER_TURN=1
	else
		echo "error"
	fi
}

function which_player_turn_is {
	if (( GLOBAL_WHICH_PLAYER_TURN == 1 )); then
		echo 1
	else
		echo 2
	fi
}

# human vs human
if (( PARAM_ACQUIRED[2] == 0 )) && (( PARAM_ACQUIRED[3] == 0 )); then
	./sredniowiecze_gui_with_libs.sh -human1 -human2 <&3 >&4 &
	gui_pid=$!
	
	echo "INIT $BOARD_SIZE $MAX_TURN 1 $P1_X $P1_Y $P2_X $P2_Y" >&3
	echo "INIT $BOARD_SIZE $MAX_TURN 2 $P1_X $P1_Y $P2_X $P2_Y" >&3

# human vs AI
elif (( PARAM_ACQUIRED[2] == 0 )) || (( PARAM_ACQUIRED[3] == 0 )); then
	# flag - 0 if gui is player two, 1 if player one
	is_gui_player_one=0
	
	function is_gui_turn {
		if (( WHICH_PLAYER_TURN == 1 )) && (( is_gui_player_one == 1 )); then
			echo 1
		elif (( WHICH_PLAYER_TURN == 2 )) && (( is_gui_player_one == 0 )); then
			echo 1
		else
			echo 0
		fi
	}
	
	if (( PARAM_ACQUIRED[2] == 1 )); then
		./sredniowiecze_gui_with_libs.sh -human2 <&3 >&4 &
		gui_pid=$!
		"$AI1" <&5 >&6 &
		ai_pid=$!
		echo "INIT $BOARD_SIZE $MAX_TURN 1 $P1_X $P1_Y $P2_X $P2_Y" >&5
	else
		./sredniowiecze_gui_with_libs.sh -human1 <&3 >&4 &
		gui_pid=$!
		"$AI2" <&5 >&6 &
		ai_pid=$!
		echo "INIT $BOARD_SIZE $MAX_TURN 2 $P1_X $P1_Y $P2_X $P2_Y" >&5
		is_gui_player_one=1
		
	fi

	echo "INIT $BOARD_SIZE $MAX_TURN 1 $P1_X $P1_Y $P2_X $P2_Y" >&3
	echo "INIT $BOARD_SIZE $MAX_TURN 2 $P1_X $P1_Y $P2_X $P2_Y" >&3

	{
		while true; do
			read line <&6
			echo $line >&3
		done
	} &
	pid1=$!
	disown $pid1

	{
		while true; do
			read line <&4
			echo $line >&5
		done
	} &
	pid2=$!
	disown $pid2

	{
		while ps -p $ai_pid > /dev/null 2>&1 && ps -p $gui_pid > /dev/null 2>&1; do
			do_nothing
		done
		if ! ps -p $gui_pid > /dev/null 2>&1; then
			kill $ai_pid > /dev/null 2>&1
		fi
	} &

	wait $! $gui_pid $ai_pid > /dev/null 2>&1
	kill $pid1 $pid2 > /dev/null 2>&1
	
	wait $ai_pid > /dev/null 2>&1
	tmp_error_code=$?
	case $tmp_error_code in
		[0]|[1]|[2]) do_nothing;;
		*) exit 1;;
	esac
# AI vs AI
else
	./sredniowiecze_gui_with_libs.sh <&3 >&4 &
	gui_pid=$!
	echo "INIT $BOARD_SIZE $MAX_TURN 1 $P1_X $P1_Y $P2_X $P2_Y" >&3
	echo "INIT $BOARD_SIZE $MAX_TURN 2 $P1_X $P1_Y $P2_X $P2_Y" >&3

	"$AI1" <&5 >&6 &
	ai1_pid=$!
	echo "INIT $BOARD_SIZE $MAX_TURN 1 $P1_X $P1_Y $P2_X $P2_Y" >&5

	"$AI2" <&7 >&8 &
	ai2_pid=$!
	echo "INIT $BOARD_SIZE $MAX_TURN 2 $P1_X $P1_Y $P2_X $P2_Y" >&7

	{
		WHICH_PLAYER_TURN=1
		while true; do
			if (( WHICH_PLAYER_TURN == 1 )); then
				read line <&6
				if [ "$line" = "END_TURN" ]; then
					WHICH_PLAYER_TURN=2
					sleep $WAIT_TIME
				else
					sleep $WAIT_TIME
				fi
				echo $line >&3
				echo $line >&7
			fi
			if (( WHICH_PLAYER_TURN == 2 )); then
				read line <&8
				if [ "$line" = "END_TURN" ]; then
					WHICH_PLAYER_TURN=1
					sleep $WAIT_TIME
				else
					sleep $WAIT_TIME
				fi
				echo $line >&3
				echo $line >&5
			fi
		done
	} &
	pid1=$!
	disown $pid1

	{
		while (( $(kill_all_if_some_pid_dies $ai1_pid $ai2_pid) == 1 )) && ps -p $gui_pid > /dev/null 2>&1; do
			do_nothing
		done
		if ! ps -p $gui_pid > /dev/null 2>&1; then
			kill $ai1_pid $ai2_pid > /dev/null 2>&1
		fi
	} &

	wait $! $gui_pid $ai1_pid $ai2_pid > /dev/null 2>&1
	kill $pid1 > /dev/null 2>&1
	
	wait $ai1_pid > /dev/null 2>&1
	tmp_error_code=$?
	if (( tmp_error_code == 42 )); then
		error_usage
	fi
	wait $ai2_pid > /dev/null 2>&1
	tmp_error_code=$?
	if (( tmp_error_code == 42 )); then
		error_usage
	fi
fi

wait $gui_pid > /dev/null 2>&1
tmp_error_code=$?
if ! (( tmp_error_code == 0 )); then
	error_usage
fi

exit 0