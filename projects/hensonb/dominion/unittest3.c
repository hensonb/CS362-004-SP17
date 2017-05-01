#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define UNITTEST "isGameOver"


/*
conditions to test:
	all piles are empty
	all piles are full
	provinces are empty
	some random 3 are empty
	final 3 piles (of enum) are empty

*/

/*
bugs found:
	doesn't check last 2 entries (sea hag, treasure map)
*/

/*
sequence of actions:
	reset
	all piles are fresh
	provinces are empty
	all piles are empty
	reset
	some random 4 are empty
	final 3 piles of enum are empty
*/

/*
criteria to test:
	does it correctly answer the question given the gamestate I construct?
	doesn't change game state
*/

int main() {
	// seed must be static, for reproducibility
	int seed = 1910;
	// kingdom cards must be static, for reproducbility
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	int numPlayers = 3;
	int ret=0, fail=0, i=0;
	
	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	
	printf("----------------- Testing Function: %s ----------------\n", UNITTEST);
	
	
	//TEST1: 
	printf("All piles are fresh (normal).. ");
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	ret = isGameOver(&G);
	fail = 0;
	// check that they match
	if(ret!=0) {fail++;}
	if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	//TEST2: 
	printf("Provinces are empty........... ");
	G.supplyCount[province] = 0;
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	ret = isGameOver(&G);
	fail = 0;
	// check that they match
	if(ret!=1) {fail++;}
	if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	//TEST3: 
	printf("All piles are empty........... ");
	for (i = 0; i <= treasure_map; i++) {
		G.supplyCount[i] = 0;
	}
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	ret = isGameOver(&G);
	fail = 0;
	// check that they match
	if(ret!=1) {fail++;}
	if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	//TEST4: 
	printf("4 arbitrary piles are empty... ");
	G.supplyCount[adventurer] = 0;
	G.supplyCount[smithy] = 0;
	G.supplyCount[copper] = 0;
	G.supplyCount[estate] = 0;
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	ret = isGameOver(&G);
	fail = 0;
	// check that they match
	if(ret!=1) {fail++;}
	if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	

	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	//TEST5: 
	printf("Final 3 piles are empty... ");
	G.supplyCount[salvager] = 0;
	G.supplyCount[sea_hag] = 0;
	G.supplyCount[treasure_map] = 0;
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	ret = isGameOver(&G);
	fail = 0;
	// check that they match
	if(ret!=1) {fail++;}
	if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	

	
	
	return 0;
}




