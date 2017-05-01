#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define UNITTEST "gainCard"


/*
conditions to test:
	each player
	putting a card into each of the destinations (deck, discard, hand)
	gaining card from fresh supply pile (normal)
	gaining card from supply with size = 1
	gaining card from empty supply
	gaining card from "not in this game" supply
*/

/*
bugs found:
	doesn't sanity-check that the supply pile is a real dominion card, but that's minor
*/

/*
sequence of actions:
	reset game
	p1 gain feast to hand
	p1 gain feast to deck
	p1 gain feast to discard
	repeat p2
	set smithy to size = 1
	p2 gain smithy (last in pile)
	p2 gain smithy (no more in pile)
	p2 gain steward (not used in game)
*/

/*
criteria to test:
	fails if given invalid index, or chosen card isn't in the supply
	correct card placed into correct zone
	other players' state does not change
*/

int main() {
	// seed must be static, for reproducibility
	int seed = 1910;
	// kingdom cards must be static, for reproducbility
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	int numPlayers = 2;
	int ret=0, thisPlayer=0, fail=0;

	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); // initialize a new game

	printf("----------------- Testing Function: %s ----------------\n", UNITTEST);
	
	
	
	// int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)
	for(thisPlayer=0; thisPlayer<numPlayers; thisPlayer++) {
		//TEST1: 
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		printf("Gaining feast to hand (normal)... ");
		ret = gainCard(feast,&G,2,thisPlayer);
		fail = 0;
		// do to testG what the function is supposed to do to G
		testG.supplyCount[feast]--;
		testG.hand[thisPlayer][ testG.handCount[thisPlayer] ] = feast;
		testG.handCount[thisPlayer]++;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		//TEST2: 
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		printf("Gaining feast to deck (normal)... ");
		ret = gainCard(feast,&G,1,thisPlayer);
		fail = 0;
		// do to testG what the function is supposed to do to G
		testG.supplyCount[feast]--;
		testG.deck[thisPlayer][ testG.deckCount[thisPlayer] ] = feast;
		testG.deckCount[thisPlayer]++;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		//TEST3: 
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		printf("Gaining feast to discard (normal)... ");
		ret = gainCard(feast,&G,0,thisPlayer);
		fail = 0;
		// do to testG what the function is supposed to do to G
		testG.supplyCount[feast]--;
		testG.discard[thisPlayer][ testG.discardCount[thisPlayer] ] = feast;
		testG.discardCount[thisPlayer]++;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		// endTurn(&G);

	}
	
	G.supplyCount[smithy] = 1;
	
	//TEST7: 
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	printf("Gaining smithy to discard (last card). ");
	ret = gainCard(smithy,&G,0,0);
	fail = 0;
	// do to testG what the function is supposed to do to G
	testG.supplyCount[smithy]--;
	testG.discard[0][ testG.discardCount[0] ] = smithy;
	testG.discardCount[0]++;
	// check that they match
	if(ret!=0) {fail++;}
	if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
	//TEST8: 
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	printf("Gaining smithy to discard (empty pile).");
	ret = gainCard(smithy,&G,0,0);
	fail = 0;
	// do to testG what the function is supposed to do to G
		// supposed to do nothing
	// check that they match
	if(ret!=-1) {fail++;}
	if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
	//TEST9: 
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	printf("Gaining steward to discard (unused)... ");
	ret = gainCard(steward,&G,0,0);
	fail = 0;
	// do to testG what the function is supposed to do to G
		// supposed to do nothing
	// check that they match
	if(ret!=-1) {fail++;}
	if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	return 0;
}


/*
// TEST4: discard position 0 (final card)
memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
printf("discard position 0 (final card)... ");
ret = discardPlayedCard(0,thisPlayer,&G,0);
fail = 0;
// do to testG what the function is supposed to do to G
...
// check that they match
if(ret!=0) {fail++;}
if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
// print out result
if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
*/


