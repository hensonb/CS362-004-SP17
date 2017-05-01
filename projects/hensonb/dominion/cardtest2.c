#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define UNITTEST "smithy"


/*
conditions to test:
	deck is empty
	deck and discard are empty
	normal operation
	hand is empty
	each player
	smithy is discarded to 'playedCards'
*/

/*
bugs found:
	only draws for player 0, regardless of who plays it
	
*/

/*
sequence of actions:
*/

/*
criteria to test:
	no cards created or destroyed
	other player's state hasnt changed
	exactly 3 cards drawn, unless deck/discard are empty afterwards
*/

int main() {
	// seed must be static, for reproducibility
	int seed = 1910;
	// kingdom cards must be static, for reproducbility
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	int numPlayers = 2;
	int ret=0, thisPlayer=0, fail=0;
	int bonus=0;

	memset(&G, 0, sizeof(struct gameState));    // clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); // initialize a new game


	printf("----------------- Testing Card: %s ----------------\n", UNITTEST);
	
	
	
	
	for(thisPlayer=0; thisPlayer<numPlayers; thisPlayer++) {
		
		// use a more interesting hand
		G.handCount[thisPlayer] = 3;
		memset(G.hand[thisPlayer], 13, G.handCount[thisPlayer]); // fill the hand with smithy
		G.deckCount[thisPlayer] = 4;
		memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
		G.discardCount[thisPlayer] = 3;
		memset(G.discard[thisPlayer], 1, G.discardCount[thisPlayer]); // fill the discard with estates
		
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
		printf("First draw 3......... ");
		ret = cardEffect(smithy,0,0,0,&G,0,&bonus);
		fail = 0;
		// do the thing
		drawCard(thisPlayer, &testG);
		drawCard(thisPlayer, &testG);
		drawCard(thisPlayer, &testG);
		testG.playedCards[testG.playedCardCount] = testG.hand[thisPlayer][0]; 
		testG.playedCardCount++;
		testG.hand[thisPlayer][0]=testG.hand[thisPlayer][testG.handCount[thisPlayer]-1];
		testG.hand[thisPlayer][testG.handCount[thisPlayer]-1] = -1;
		testG.handCount[thisPlayer]--;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		
		
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
		printf("Second draw 3......... ");
		ret = cardEffect(smithy,0,0,0,&G,1,&bonus);
		fail = 0;
		// do the thing
		drawCard(thisPlayer, &testG);
		drawCard(thisPlayer, &testG);
		drawCard(thisPlayer, &testG);
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
		
		
		memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
		// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
		printf("Second draw 3......... ");
		ret = cardEffect(smithy,0,0,0,&G,2,&bonus);
		fail = 0;
		// do the thing
		drawCard(thisPlayer, &testG);
		drawCard(thisPlayer, &testG);
		drawCard(thisPlayer, &testG);
		testG.playedCards[testG.playedCardCount] = testG.hand[thisPlayer][2]; 
		testG.playedCardCount++;
		testG.hand[thisPlayer][2]=testG.hand[thisPlayer][testG.handCount[thisPlayer]-1];
		testG.hand[thisPlayer][testG.handCount[thisPlayer]-1] = -1;
		testG.handCount[thisPlayer]--;
		// check that they match
		if(ret!=0) {fail++;}
		if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		// print out result
		if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
		
		
		
		
		endTurn(&G);
		
	}
	
	return 0;
}




