#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define UNITTEST "scoreFor"

// NOTE: simple function to test, doesn't modify gameState at all, just returns a value

/*
conditions to test:
	hand/deck/discard have sum of 0 (gamestart)
	some normal situation
	hand/deck/discard are empty
	hand/deck/discard have negative sum of points (curses)
	all gardens
	different players
*/

/*
bugs found:
	gardens are improperly implemented; doesn't count full deck size
	iterates over deck until it reaches size of discard pile
	fullDeckCount function seems to be misnamed
*/

/*
sequence of actions:
	reset game
	hand/deck/discard have sum of 0 (gamestart)
	set hand to curse/estate/duchy/province/great_hall/gardens
	repeat for deck/discard
	hand/deck/discard are empty (no cards)
	reset game
	hand is all curses, deck exists but has no value
	hand is all gardens
	repeat for player2
*/

/*
criteria to test:
	doesn't modify gamestate
	gives correct result
*/


void showCards (struct gameState *G, int thisPlayer) {
	int i=0, ret=0;
	ret = scoreFor(thisPlayer,G);
	printf("Score=%i\n*",ret);
	for (i = 0; i < G->handCount[thisPlayer]; i++) {
		printf("Hand%i = %i, ",i,G->hand[thisPlayer][i]);
	}
	printf("\n*");
	for (i = 0; i < G->deckCount[thisPlayer]; i++) {
		printf("Deck%i = %i, ",i,G->deck[thisPlayer][i]);
	}
	printf("\n*");
	for (i = 0; i < G->discardCount[thisPlayer]; i++) {
		printf("Discard%i = %i, ",i,G->discard[thisPlayer][i]);
	}
	printf("\n\n");
}








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
	
	
	for(thisPlayer=0; thisPlayer<numPlayers; thisPlayer++) {
		
		//TEST1: 
		printf("Gamestart: hand/deck/discard have score = 3.. ");
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		ret = scoreFor(thisPlayer,&G);
		fail = 0;
		// check that they match
		if(ret!=3) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		showCards(&G, thisPlayer);
		
		// use a more interesting hand
		G.hand[thisPlayer][0] = curse;
		G.hand[thisPlayer][1] = estate;
		G.hand[thisPlayer][2] = duchy;
		G.hand[thisPlayer][3] = province;
		G.hand[thisPlayer][4] = great_hall;
		G.hand[thisPlayer][5] = gardens;
		G.handCount[thisPlayer] = 6;
		G.deckCount[thisPlayer] = 0;
		G.discardCount[thisPlayer] = 0;
		
		//TEST2: 
		printf("Hand has value of -1+1+3+6+1+(6/10)=10.... ");
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		ret = scoreFor(thisPlayer,&G);
		fail = 0;
		// check that they match
		if(ret!=10) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		showCards(&G, thisPlayer);
		
		
		// use a more interesting deck
		G.deck[thisPlayer][0] = curse;
		G.deck[thisPlayer][1] = estate;
		G.deck[thisPlayer][2] = duchy;
		G.deck[thisPlayer][3] = province;
		G.deck[thisPlayer][4] = great_hall;
		G.deck[thisPlayer][5] = gardens;
		G.handCount[thisPlayer] = 0;
		G.deckCount[thisPlayer] = 6;
		G.discardCount[thisPlayer] = 0;
		
		//TEST3: 
		printf("Deck has value of -1+1+3+6+1+(6/10)=10.... ");
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		ret = scoreFor(thisPlayer,&G);
		fail = 0;
		// check that they match
		if(ret!=10) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		showCards(&G, thisPlayer);
		
		
		// use a more interesting discard
		G.discard[thisPlayer][0] = curse;
		G.discard[thisPlayer][1] = estate;
		G.discard[thisPlayer][2] = duchy;
		G.discard[thisPlayer][3] = province;
		G.discard[thisPlayer][4] = great_hall;
		G.discard[thisPlayer][5] = gardens;
		G.handCount[thisPlayer] = 0;
		G.deckCount[thisPlayer] = 0;
		G.discardCount[thisPlayer] = 6;
		
		//TEST4: 
		printf("Discard has value of -1+1+3+6+1+(6/10)=10.. ");
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		ret = scoreFor(thisPlayer,&G);
		fail = 0;
		// check that they match
		if(ret!=10) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		showCards(&G, thisPlayer);
		
		
		G.handCount[thisPlayer] = 0;
		G.deckCount[thisPlayer] = 0;
		G.discardCount[thisPlayer] = 0;
		
		//TEST5: 
		printf("Hand/deck/discard are totally empty......... ");
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		ret = scoreFor(thisPlayer,&G);
		fail = 0;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		showCards(&G, thisPlayer);
		

		// reset the game
		memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
		initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
		printf("gamerestart\n");
		// use a more interesting hand
		G.hand[thisPlayer][0] = curse;
		G.hand[thisPlayer][1] = curse;
		G.hand[thisPlayer][2] = curse;
		G.hand[thisPlayer][3] = curse;
		G.hand[thisPlayer][4] = curse;
		G.handCount[thisPlayer] = 5;
		G.deckCount[thisPlayer] = 0;
		G.discardCount[thisPlayer] = 0;

		//TEST6: 
		printf("Hand is nothing but curses, score = -5...... ");
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		ret = scoreFor(thisPlayer,&G);
		fail = 0;
		// check that they match
		if(ret!=-5) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		showCards(&G, thisPlayer);
		
		// use a more interesting hand
		G.hand[thisPlayer][0] = gardens;
		G.hand[thisPlayer][1] = gardens;
		G.hand[thisPlayer][2] = gardens;
		G.hand[thisPlayer][3] = gardens;
		G.hand[thisPlayer][4] = gardens;
		G.hand[thisPlayer][5] = gardens;
		G.hand[thisPlayer][6] = gardens;
		G.hand[thisPlayer][7] = gardens;
		G.hand[thisPlayer][8] = gardens;
		G.hand[thisPlayer][9] = gardens;
		G.hand[thisPlayer][10] = gardens;
		G.handCount[thisPlayer] = 11;
		G.deckCount[thisPlayer] = 0;
		G.discardCount[thisPlayer] = 0;

		//TEST7
		printf("Hand is nothing but 11 gardens, score = 11.. ");
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		ret = scoreFor(thisPlayer,&G);
		fail = 0;
		// check that they match
		if(ret!=5) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		showCards(&G, thisPlayer);
		
		// endTurn(&G);
		
	}
	
	return 0;
}




