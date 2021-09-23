/*
 *Program: Musical chairs game with n players and m intervals.
 *Authors:  Bandi Chiranjeevi Sai Ganesh, Navudu Jyothi Swaroop
 *Roll# :  CS18BTECH11004,CS18BTECH11033
 */
#include <stdlib.h>  /*for exit, atoi */
#include <iostream>  /*for fprintf */
#include <errno.h>   /*for error code eg. E2BIG */
#include <getopt.h>  /*for getopt */
#include <assert.h>  /*for assert */
#include <chrono>    /*for timers */
#include <vector>   /*for vectors*/
#include <thread>   /*for threads*/
#include <string.h> /*for string comparsion*/
#include <mutex>    /*for mutex*/
#include <bits/stdc++.h> /*for any other functions*/

/*
 *Forward declarations
 */
using namespace std;
mutex mtx;
condition_variable cv;	// conditional variable for player threads
mutex ump;
condition_variable xyz;	// conditional variable for umpire thread
int check = 0;
int id, amount;

int winner;	// stores the id of player who wins the game
int count_lap;	// stores the remaining no. of laps 
mutex lock_m;	// lock used to ensure that player threads starts execution only when umpire says to do it. 
vector<thread> vec_threads;
int no_of_chairs;

int index_A;	// stores no. of values in A

void usage(int argc, char *argv[]);
unsigned long long musical_chairs(int nplayers);
void player_main(int plid, int *A, int *slep);

int
main(int argc, char *argv[])
{
	int c;
	int nplayers = 0;

	/*Loop through each option (and its's arguments) and populate variables */
	while (1)
	{
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
		{ 	"help", no_argument, 0, 'h' },
			{ 	"nplayers", required_argument, 0, '1' },
			{ 0, 0, 0, 0 }
		};

		c = getopt_long(argc, argv, "h1:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
		{
			case 0:
				cerr << "option " << long_options[option_index].name;
				if (optarg)
					cerr << " with arg " << optarg << endl;
				break;

			case '1':
				nplayers = atoi(optarg);
				break;

			case 'h':
			case '?':
				usage(argc, argv);

			default:
				cerr << "?? getopt returned character code 0%o ??n" << c << endl;
				usage(argc, argv);
		}
	}

	if (optind != argc)
	{
		cerr << "Unexpected arguments.\n";
		usage(argc, argv);
	}

	if (nplayers == 0)
	{
		cerr << "Invalid nplayers argument." << endl;
		return EXIT_FAILURE;
	}

	unsigned long long game_time;
	game_time = musical_chairs(nplayers);

	cout << "Time taken for the game: " << game_time << " us" << endl;

	exit(EXIT_SUCCESS);
}

/*
 *Show usage of the program
 */
void
usage(int argc, char *argv[])
{
	cerr << "Usage:\n";
	cerr << argv[0] << "--nplayers<n>" << endl;
	exit(EXIT_FAILURE);
}

/*
 *umpire thread function, which creates player threads and conducts game for player threads
 */
void
umpire_main(int nplayers, int *slep, int *A)
{
	no_of_chairs = nplayers - 1;

	count_lap = nplayers - 1;

	lock_m.lock();	// acquires lock 
	for (int i = 0; i < nplayers; i++)
	{

		thread th1(player_main, i, A, slep);	//creates n player threads

		vec_threads.push_back(move(th1));	//adds thread to vector
	}

	for (int i = 0; i < nplayers - 1; i++)
	{
		char s[16];

		do { 	cin >> s;	//takes input from file
			if (!strcmp(s, "lap_start")) {}
			else if (!strcmp(s, "player_sleep"))
			{
				cin >> id >> amount;
				slep[id] = amount;
			}
			else if (!strcmp(s, "music_start")) {}
			else if (!strcmp(s, "umpire_sleep"))
			{
				cin >> amount;
				this_thread::sleep_for(chrono::microseconds(amount));	// makes umpire thread to sleep
			}
			else if (!strcmp(s, "music_stop"))
			{

				if (check)
				{
					cv.notify_all();
				}	//all player threads have to wake up
				else
				{
					lock_m.unlock();	// executes only for the first time
				}

				unique_lock<mutex> lck_u(ump);
				xyz.wait(lck_u);	// umpire threads waits on conditional variable

				check = 1;
			}
			else if (!strcmp(s, "lap_stop"))
			{

				no_of_chairs = nplayers - 2 - i;	//updates no. of chairs after every lap

				for (int g = 0; g < nplayers; g++)
					slep[g] = 0;
			}
		} while (strcmp(s, "lap_stop"));	// breaks when lap_stop is read, this implies that one lap is finished 
	}

	for (std::thread &thread: vec_threads)
	{
		thread.join();	// makes umpire thread to wait for all other player threads to finish their job
	}

	return;
}
/*
 *player thread function
 */
void
player_main(int plid, int *A, int *slep)
{
	lock_m.lock();

	lock_m.unlock();

	while (1)
	{
		if (slep[plid] != 0)
		{
			this_thread::sleep_for(chrono::microseconds(slep[plid]));	// makes player thread to sleep
		}
		unique_lock<mutex> lck(mtx);	// acquires lock so as to ensure that no other player threads enters the critical section

		if (no_of_chairs == 0)
		{
			A[index_A++] = plid;

			count_lap--;
			xyz.notify_all();	// wake up the empire thread;
			break;
		}
		else
		{
			no_of_chairs--;	// player thread occupies a chair

			if (count_lap == 1)	// executed in last lap by player thread who wins the game
			{
				winner = plid;
				break;
			}
			cv.wait(lck);	// player thread waits on conditional variable after occuping a chair
		}
	}

	return;
}
/*
 *funtion which creates umpire thread and print the result of the game
 */
unsigned long long
musical_chairs(int nplayers)
{
	auto t1 = chrono::steady_clock::now();
	int n = nplayers;

	int A[n - 1];	//to store threads losing order
	int slep[n] = { 0 };	// to store sleep quantum of player threads

	thread umpire(umpire_main, nplayers, &slep[0], &A[0]);	// creates umpire thread

	/*Add your code here */

	umpire.join();	// makes thread to wait for umpire thread to finish its execution.

	cout << "Musical Chairs: " << nplayers << " player game with " << nplayers - 1 << " laps." << endl;
	for (int i = 0; i < n - 1; i++)
	{
		cout << "======= lap# " << i + 1 << " =======" << endl;
		cout << A[i] << " could not get chair" << endl;
		cout << "**********************" << endl;
	}
	cout << "Winner is " << winner << endl;

	auto t2 = chrono::steady_clock::now();

	auto d1 = chrono::duration_cast<chrono::microseconds > (t2 - t1);

	return d1.count();
}
