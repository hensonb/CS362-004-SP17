#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define UNITTEST "discardPlayedCard"

//////////////////////////////////////////////////////////////
// NOTE: renamed discardCard to discardPlayedCard because that's what it is actually for
//////////////////////////////////////////////////////////////

/*
conditions to test:
	discarding from different hand positions
		invalid hand positions (negative, too large, hand is empty)
	discarding from different hand sizes
	" to different piles (discard(played), trash)
	" when dest. piles are empty, non-empty
*/

/*
bugs found:
	doesnt sanity-check inputs
	DOESN'T ACTUALLY PUT CARDS INTO DISCARD PILE, instead puts them into "played cards" pile which is never used again... but that's not a bug in THIS function, it's a bug in the endTurn function
	end-turn logic doesnt match game rules
*/

/*
criteria to test:
	card gone from hand, handsize decreased
	card copied to playedCards or deleted entirely
	fails if given an invalid index
	other players' state does not change
*/

/*
sequence of actions:
	reset piles and hands
	"Interesting hand", handsize = 4
	player 1: trash position 3 (right end)
				discard position 1 (middle)
				trash position 0 (left end)
				discard position 0 (final card)
	end turn
	repeat for player 2
	reset piles and hands
	discard position -10
	discard position 15
	discard position 1234
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
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game

	printf("----------------- Testing Function: %s ----------------\n", UNITTEST);
	
	
	
	// int discardPlayedCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag)
	for(thisPlayer=0; thisPlayer<numPlayers; thisPlayer++) {
		// use a more interesting hand
		G.hand[thisPlayer][0] = steward;
		G.hand[thisPlayer][1] = copper;
		G.hand[thisPlayer][2] = duchy;
		G.hand[thisPlayer][3] = estate;
		G.hand[thisPlayer][4] = -1;
		G.handCount[thisPlayer] = 4;
		
		// TEST1: trashing position 3 (right end)
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		printf("Trashing position 3 (right end)... ");
		ret = discardPlayedCard(3,thisPlayer,&G,1);
		fail = 0;
		// do to testG what the function is supposed to do to G
		// erase the card and reduce hand size to hide it
		testG.hand[thisPlayer][testG.handCount[thisPlayer]-1] = -1;
		testG.handCount[thisPlayer]--;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		// TEST2: discard position 1 (middle)
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		printf("Discard position 1 (middle)....... ");
		ret = discardPlayedCard(1,thisPlayer,&G,0);
		fail = 0;
		// do to testG what the function is supposed to do to G
		// copy card to playedCards, copy the end card into the handPos, erase the end card, and reduce hand size to hide it
		testG.playedCards[testG.playedCardCount] = testG.hand[thisPlayer][1]; 
		testG.playedCardCount++;
		testG.hand[thisPlayer][1]=testG.hand[thisPlayer][testG.handCount[thisPlayer]-1];
		testG.hand[thisPlayer][testG.handCount[thisPlayer]-1] = -1;
		testG.handCount[thisPlayer]--;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		// TEST3: trash position 0 (left end)
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		printf("Trash position 0 (left end)....... ");
		ret = discardPlayedCard(0,thisPlayer,&G,1);
		fail = 0;
		// do to testG what the function is supposed to do to G
		testG.hand[thisPlayer][0]=testG.hand[thisPlayer][testG.handCount[thisPlayer]-1];
		testG.hand[thisPlayer][testG.handCount[thisPlayer]-1] = -1;
		testG.handCount[thisPlayer]--;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		// TEST4: discard position 0 (final card)
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		printf("discard position 0 (final card)... ");
		ret = discardPlayedCard(0,thisPlayer,&G,0);
		fail = 0;
		// do to testG what the function is supposed to do to G
		testG.playedCards[testG.playedCardCount] = testG.hand[thisPlayer][0]; 
		testG.playedCardCount++;
		testG.hand[thisPlayer][testG.handCount[thisPlayer]-1] = -1;
		testG.handCount[thisPlayer]--;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		
		endTurn(&G);
	}
	
	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	// TEST9: discard illegal position -10
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	printf("Discard illegal position -10... ");
	ret = 0;
	fail = 0;
	// try { 
		ret = discardPlayedCard(-10,0,&G,0);
	// } catch (...) {
		// printf("\tFAIL! EXCEPTION THROWN\n");
		// fail = -100;
	// }
	// do to testG what the function is supposed to do to G
		// supposed to do nothing, return -1
	// check that they match
	if(fail==0) {
		if(ret!=-1) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail>0) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	}

	
	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	// TEST10: discard illegal position 15
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	printf("Discard illegal position 15.... ");
	ret = 0;
	fail = 0;
	// try { 
		ret = discardPlayedCard(15,0,&G,0); 
	// } catch (...) {
		// printf("\tFAIL! EXCEPTION THROWN\n");
		// fail = -100;
	// }
	// do to testG what the function is supposed to do to G
		// supposed to do nothing, return -1
	// check that they match
	if(fail==0) {
		if(ret!=-1) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail>0) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	}

	
	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	// TEST11: discard illegal position 1234
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	printf("Discard illegal position 1234... ");
	ret = 0;
	fail = 0;
	// try { 
		ret = discardPlayedCard(1234,0,&G,0); 
	// } catch (...) {
		// printf("\tFAIL! EXCEPTION THROWN\n");
		// fail = -100;
	// }
	// do to testG what the function is supposed to do to G
		// supposed to do nothing, return -1
	// check that they match
	if(fail==0) {
		if(ret!=-1) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail>0) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	}

	
	
	return 0;
}

// if(G.deckCount[thisPlayer]!=testG.deckCount[thisPlayer]) {fail++;}
// if(memcmp(G.deck[thisPlayer], testG.deck[thisPlayer], G.deckCount[thisPlayer])) {fail++;}

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


