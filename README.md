Go to catalogue src and execute cmake .. to create working files.
Execute make doc afterwards to create documentation.

How to start the game :
Execute game.sh.

game.sh [-n n] [-k k] [-s s] [-p1 x1,y1] [-p2 x2,y2] [-ai1 ai1] [-ai2 ai2]
n - board size (default 10)
k - number of turns (default 100)
s - delay between ending a turn of one player and beginning the other one turn (deafult 1)
x1, y1 - first player's king position (if not specified it will be randomized)
x2, y2 - second player's king position (if not specified it will be randomized)
ai1 - path to first player's AI (if not specified it is assumed that human is first player)
ai2 - path to second player's AI (if not specified it is assumed that human is second player)

