// File: prg1_2.cpp
// the program simulates playing the game of Craps.
// if die is a randomNumber object, simulate tossing
// the dice with the expression

//		(1 + die.random(6)) + (1+ die.random(6))

// make an initial toss and determine if the player
// or house wins. otherwise, execute a loop that
// continues until a toss matches the initial toss
// or totals 7

#include <iostream>

#include "d_random.h"		// randomNumber class

using namespace std;

int main()
{
	randomNumber die;
	int initToss, nextToss;

	// make the initial toss of 2 dice
	initToss = (1 + die.random(6)) + (1 + die.random(6));

	// check for immediate win. house wins with 2, 3, or 12
	// player wins with 7 or 11
	if (initToss == 2 || initToss == 3 || initToss == 12)
		cout << "Initial toss is " << initToss << "  House wins" << endl;	
	else if (initToss == 7 || initToss == 11)
		cout << "Initial toss is " << initToss << "  Player wins" << endl;
	else
	// initial toss becomes the target; continue tossing until the
	// target occurs (player wins) or craps = 7 occurs (house wins)
	{
		cout << "Target is " << initToss << "  Play on" << endl;
		do
		{
			nextToss = (1 + die.random(6)) + (1 + die.random(6));
			cout << "Next toss " << nextToss << endl;
		}	while (nextToss != 7 && nextToss != initToss);
		if (nextToss == 7)
			cout << "    Craps - House wins" << endl;
		else
			cout << "    Match - Player wins" << endl;
	}

	return 0;
}

/*
Run 1:
Initial toss is 12  House wins

Run 2:
Initial toss is 7  Player wins

Run 3:
Target is 6  Play on
Next toss 5
Next toss 2
Next toss 11
Next toss 6
    Match - Player wins

Run 4:
Target is 6  Play on
Next toss 9
Next toss 9
Next toss 4
Next toss 8
Next toss 11
Next toss 11
Next toss 7
    Craps - House wins
*/
