I have submitted three files(including this file),One file is report file which tells about the
program and the last file is c++ source code with extension .cpp.
If the cpp file name is musical.cpp, then we have to compile this file using command "g++
musical.cpp -pthread".This command will create a executable file (a.out).
We have to run this executable file by using this command "./a.out --np no. <
input4fastrand.txt".where no. is number of players and input4fastrand.txt contains inputs for
a.out file.
If we want to use compiler optimisation we can add -O1,-O2 to command "g++ musical.cpp
-pthread",at the end while compiling it.
Sample outputs:
1)
$ cat input4randfast.txt
lap_start
music_start
music_stop
lap_stop
lap_start
music_start
music_stop
lap_stop
lap_start
music_start
music_stop
lap_stop

$ ./a.out --np 4 < input4randfast.txt
Musical Chairs: 4 player game with 3 laps.
======= lap# 1 =======
3 could not get chair
**********************
======= lap# 2 =======
0 could not get chair
**********************
======= lap# 3 =======
1 could not get chair
**********************
Winner is 2
Time taken for the game: 1818 us


2)

$ cat input4rand.txt
lap_start
music_start
umpire_sleep 200
music_stop
lap_stop
lap_start
music_start
umpire_sleep 200000
music_stop
lap_stop
lap_start
music_start
umpire_sleep 800000
music_stop
lap_stop

$ ./a.out --np 4 < input4rand.txt
Musical Chairs: 4 player game with 3 laps.
======= lap# 1 =======
3 could not get chair
**********************
======= lap# 2 =======
1 could not get chair
**********************
======= lap# 3 =======
0 could not get chair
**********************
Winner is 2
Time taken for the game: 1003015 us

3)


$ cat input4deterministic.txt
lap_start
player_sleep 0 1000player_sleep 1 2000
player_sleep 2 3000
player_sleep 3 4000
music_start
umpire_sleep 200
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
player_sleep 2 3000
music_start
umpire_sleep 200000
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
music_start
umpire_sleep 800000
music_stop
lap_stop

$ ./a.out --np 4 < input4deterministic.txt
Musical Chairs: 4 player game with 3 laps.
======= lap# 1 =======
3 could not get chair
**********************
======= lap# 2 =======
2 could not get chair
**********************
======= lap# 3 =======
1 could not get chair
**********************
Winner is 0
Time taken for the game: 1012461 us

4)


$ cat input4deterministic.txt
lap_start
player_sleep 0 1000
player_sleep 1 2000
player_sleep 2 3000
player_sleep 3 4000
music_start
umpire_sleep 200
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
player_sleep 2 3000
music_start
umpire_sleep 200000
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
music_start
umpire_sleep 800000
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
player_sleep 2 3000
player_sleep 3 4000
music_start
umpire_sleep 200
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
player_sleep 2 3000
music_startumpire_sleep 200000
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
music_start
umpire_sleep 800000
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
music_start
umpire_sleep 800000
music_stop
lap_stop

$ ./a.out --np 8 < input4deterministic.txt
Musical Chairs: 8 player game with 7 laps.
======= lap# 1 =======
3 could not get chair
**********************
======= lap# 2 =======
2 could not get chair
**********************
======= lap# 3 =======
1 could not get chair
**********************
======= lap# 4 =======
0 could not get chair
**********************
======= lap# 5 =======
6 could not get chair
**********************
======= lap# 6 =======
4 could not get chair
**********************
======= lap# 7 =======
7 could not get chair
**********************
Winner is 5
Time taken for the game: 2615042 us


