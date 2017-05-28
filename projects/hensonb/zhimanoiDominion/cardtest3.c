#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define UNITTEST "???"


/*
conditions to test:
*/

/*
bugs found:
*/

/*
sequence of actions:
*/

/*
criteria to test:
*/

int main() {
	// seed must be static, for reproducibility
	int seed = 1910;
	// kingdom cards must be static, for reproducbility
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	int numPlayers = 2;

	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); // initialize a new game

	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object

	printf("----------------- Testing Card: %s ----------------\n", UNITTEST);
	
	
	
		
	
	
	
	
	
	
	return 0;
}




