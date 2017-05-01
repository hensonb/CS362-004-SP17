#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define UNITTEST "adventurer"


/*
conditions to test:
	treasure in deck/discard
	no treasure in deck/discard
	deck is empty, treasure in discard
	hand is otherwise empty
	multiple players
*/

/*
bugs found:
	doesn't draw silvers
	doesn't discard itself to playedCards pile
	infinite loop if there are less than 2 treasure cards to find
*/

/*
sequence of actions:
	reset game, add card to hand
	set up specific situation :(
	compare against specific result :(
*/

/*
criteria to test:
	other players' state hasn't changed
	doesn't discard any treasures
	leaves 2 new treasures in-hand, or if fewer, no treasures left in the deck
	no cards created or destroyed (use fulldeckCount[thisPlayer] function)
	adventurer is discarded to 'playedCards'
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

	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	
	printf("----------------- Testing Card: %s ----------------\n", UNITTEST);
	
	
		
	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	
	// use a more interesting hand
	G.handCount[thisPlayer] = 5;
	memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]); // fill the hand with estates
	G.deckCount[thisPlayer] = 5;
	memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
	G.discardCount[thisPlayer] = 5;
	memset(G.discard[thisPlayer], 1, G.discardCount[thisPlayer]); // fill the discard with estates
	G.hand[thisPlayer][1] = adventurer;
	
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
	printf("Deck/discard is all estates......... ");
	ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
	fail = 0;
	// check that they match
	if(ret!=-1) {fail++;}
	if(fullDeckCount(thisPlayer, estate, &G) != 14) {fail++;} // no cards created or destroyed
	if(G.handCount[thisPlayer] != 4)  {fail++;} 
	if(G.deckCount[thisPlayer] != 0)  {fail++;} 
	if(G.discardCount[thisPlayer] != 10)  {fail++;} 
	if(G.playedCardCount != 1)  {fail++;} // properly discarded
	if(G.playedCards[0] != adventurer)  {fail++;} // properly discarded
	if(memcmp(G.hand[!thisPlayer], testG.hand[!thisPlayer], G.handCount[!thisPlayer])) {fail++;} // other players unaffected
	if(memcmp(G.deck[!thisPlayer], testG.deck[!thisPlayer], G.deckCount[!thisPlayer])) {fail++;}
	if(memcmp(G.discard[!thisPlayer], testG.discard[!thisPlayer], G.discardCount[!thisPlayer])) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	
	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	
	// use a more interesting hand
	G.handCount[thisPlayer] = 5;
	memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]); // fill the hand with estates
	G.deckCount[thisPlayer] = 5;
	memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
	G.discardCount[thisPlayer] = 5;
	memset(G.discard[thisPlayer], 4, G.discardCount[thisPlayer]); // fill the discard with coppers
	G.hand[thisPlayer][1] = adventurer;
	
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
	printf("Deck is estates, discard is coppers... ");
	ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
	fail = 0;
	// check that they match
	if(ret!=0) {fail++;}
	if(fullDeckCount(thisPlayer, estate, &G) != 9) {fail++;} // no cards created or destroyed
	if(fullDeckCount(thisPlayer, copper, &G) != 5) {fail++;} // no cards created or destroyed
	if(G.handCount[thisPlayer] != 6)  {fail++;} 
	if(G.deckCount[thisPlayer] != 3)  {fail++;} 
	if(G.discardCount[thisPlayer] != 5)  {fail++;} 
	if(G.playedCardCount != 1)  {fail++;} // properly discarded
	if(G.playedCards[0] != adventurer)  {fail++;} // properly discarded
	if(memcmp(G.hand[!thisPlayer], testG.hand[!thisPlayer], G.handCount[!thisPlayer])) {fail++;} // other players unaffected
	if(memcmp(G.deck[!thisPlayer], testG.deck[!thisPlayer], G.deckCount[!thisPlayer])) {fail++;}
	if(memcmp(G.discard[!thisPlayer], testG.discard[!thisPlayer], G.discardCount[!thisPlayer])) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	
	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	
	// use a more interesting hand
	G.handCount[thisPlayer] = 5;
	memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]); // fill the hand with estates
	G.deckCount[thisPlayer] = 5;
	memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
	G.discardCount[thisPlayer] = 5;
	memset(G.discard[thisPlayer], 5, G.discardCount[thisPlayer]); // fill the discard with coppers
	G.hand[thisPlayer][1] = adventurer;
	
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
	printf("Deck is estates, discard is silvers... ");
	ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
	fail = 0;
	// check that they match
	if(ret!=0) {fail++;}
	if(fullDeckCount(thisPlayer, estate, &G) != 9) {fail++;} // no cards created or destroyed
	if(fullDeckCount(thisPlayer, silver, &G) != 5) {fail++;} // no cards created or destroyed
	if(G.handCount[thisPlayer] != 6)  {fail++;} 
	if(G.deckCount[thisPlayer] != 3)  {fail++;} 
	if(G.discardCount[thisPlayer] != 5)  {fail++;} 
	if(G.playedCardCount != 1)  {fail++;} // properly discarded
	if(G.playedCards[0] != adventurer)  {fail++;} // properly discarded
	if(memcmp(G.hand[!thisPlayer], testG.hand[!thisPlayer], G.handCount[!thisPlayer])) {fail++;} // other players unaffected
	if(memcmp(G.deck[!thisPlayer], testG.deck[!thisPlayer], G.deckCount[!thisPlayer])) {fail++;}
	if(memcmp(G.discard[!thisPlayer], testG.discard[!thisPlayer], G.discardCount[!thisPlayer])) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	
	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	
	// use a more interesting hand
	G.handCount[thisPlayer] = 5;
	memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]); // fill the hand with estates
	G.deckCount[thisPlayer] = 5;
	memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
	G.discardCount[thisPlayer] = 5;
	memset(G.discard[thisPlayer], 6, G.discardCount[thisPlayer]); // fill the discard with coppers
	G.hand[thisPlayer][1] = adventurer;
	
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
	printf("Deck is estates, discard is golds... ");
	ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
	fail = 0;
	// check that they match
	if(ret!=0) {fail++;}
	if(fullDeckCount(thisPlayer, estate, &G) != 9) {fail++;} // no cards created or destroyed
	if(fullDeckCount(thisPlayer, gold, &G) != 5) {fail++;} // no cards created or destroyed
	if(G.handCount[thisPlayer] != 6)  {fail++;} 
	if(G.deckCount[thisPlayer] != 3)  {fail++;} 
	if(G.discardCount[thisPlayer] != 5)  {fail++;} 
	if(G.playedCardCount != 1)  {fail++;} // properly discarded
	if(G.playedCards[0] != adventurer)  {fail++;} // properly discarded
	if(memcmp(G.hand[!thisPlayer], testG.hand[!thisPlayer], G.handCount[!thisPlayer])) {fail++;} // other players unaffected
	if(memcmp(G.deck[!thisPlayer], testG.deck[!thisPlayer], G.deckCount[!thisPlayer])) {fail++;}
	if(memcmp(G.discard[!thisPlayer], testG.discard[!thisPlayer], G.discardCount[!thisPlayer])) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	
	thisPlayer = 1;
	
	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	endTurn(&G);
	
	// use a more interesting hand
	G.handCount[thisPlayer] = 5;
	memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]); // fill the hand with estates
	G.deckCount[thisPlayer] = 5;
	memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
	G.discardCount[thisPlayer] = 5;
	memset(G.discard[thisPlayer], 1, G.discardCount[thisPlayer]); // fill the discard with estates
	G.hand[thisPlayer][1] = adventurer;
	
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
	printf("Deck/discard is all estates......... ");
	ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
	fail = 0;
	// check that they match
	if(ret!=-1) {fail++;}
	if(fullDeckCount(thisPlayer, estate, &G) != 14) {fail++;} // no cards created or destroyed
	if(G.handCount[thisPlayer] != 4)  {fail++;} 
	if(G.deckCount[thisPlayer] != 0)  {fail++;} 
	if(G.discardCount[thisPlayer] != 10)  {fail++;} 
	if(G.playedCardCount != 1)  {fail++;} // properly discarded
	if(G.playedCards[0] != adventurer)  {fail++;} // properly discarded
	if(memcmp(G.hand[!thisPlayer], testG.hand[!thisPlayer], G.handCount[!thisPlayer])) {fail++;} // other players unaffected
	if(memcmp(G.deck[!thisPlayer], testG.deck[!thisPlayer], G.deckCount[!thisPlayer])) {fail++;}
	if(memcmp(G.discard[!thisPlayer], testG.discard[!thisPlayer], G.discardCount[!thisPlayer])) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	
	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	endTurn(&G);
	// use a more interesting hand
	G.handCount[thisPlayer] = 5;
	memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]); // fill the hand with estates
	G.deckCount[thisPlayer] = 5;
	memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
	G.discardCount[thisPlayer] = 5;
	memset(G.discard[thisPlayer], 4, G.discardCount[thisPlayer]); // fill the discard with coppers
	G.hand[thisPlayer][1] = adventurer;
	
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
	printf("Deck is estates, discard is coppers... ");
	ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
	fail = 0;
	// check that they match
	if(ret!=0) {fail++;}
	if(fullDeckCount(thisPlayer, estate, &G) != 9) {fail++;} // no cards created or destroyed
	if(fullDeckCount(thisPlayer, copper, &G) != 5) {fail++;} // no cards created or destroyed
	if(G.handCount[thisPlayer] != 6)  {fail++;} 
	if(G.deckCount[thisPlayer] != 3)  {fail++;} 
	if(G.discardCount[thisPlayer] != 5)  {fail++;} 
	if(G.playedCardCount != 1)  {fail++;} // properly discarded
	if(G.playedCards[0] != adventurer)  {fail++;} // properly discarded
	if(memcmp(G.hand[!thisPlayer], testG.hand[!thisPlayer], G.handCount[!thisPlayer])) {fail++;} // other players unaffected
	if(memcmp(G.deck[!thisPlayer], testG.deck[!thisPlayer], G.deckCount[!thisPlayer])) {fail++;}
	if(memcmp(G.discard[!thisPlayer], testG.discard[!thisPlayer], G.discardCount[!thisPlayer])) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	
	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	endTurn(&G);
	// use a more interesting hand
	G.handCount[thisPlayer] = 5;
	memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]); // fill the hand with estates
	G.deckCount[thisPlayer] = 5;
	memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
	G.discardCount[thisPlayer] = 5;
	memset(G.discard[thisPlayer], 5, G.discardCount[thisPlayer]); // fill the discard with coppers
	G.hand[thisPlayer][1] = adventurer;
	
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
	printf("Deck is estates, discard is silvers... ");
	ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
	fail = 0;
	// check that they match
	if(ret!=0) {fail++;}
	if(fullDeckCount(thisPlayer, estate, &G) != 9) {fail++;} // no cards created or destroyed
	if(fullDeckCount(thisPlayer, silver, &G) != 5) {fail++;} // no cards created or destroyed
	if(G.handCount[thisPlayer] != 6)  {fail++;} 
	if(G.deckCount[thisPlayer] != 3)  {fail++;} 
	if(G.discardCount[thisPlayer] != 5)  {fail++;} 
	if(G.playedCardCount != 1)  {fail++;} // properly discarded
	if(G.playedCards[0] != adventurer)  {fail++;} // properly discarded
	if(memcmp(G.hand[!thisPlayer], testG.hand[!thisPlayer], G.handCount[!thisPlayer])) {fail++;} // other players unaffected
	if(memcmp(G.deck[!thisPlayer], testG.deck[!thisPlayer], G.deckCount[!thisPlayer])) {fail++;}
	if(memcmp(G.discard[!thisPlayer], testG.discard[!thisPlayer], G.discardCount[!thisPlayer])) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	
	memset(&G, 0, sizeof(struct gameState));	// clear the game state, just to be safe
	initializeGame(numPlayers, k, seed, &G); 	// initialize a new game
	
	// use a more interesting hand
	G.handCount[thisPlayer] = 5;
	memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]); // fill the hand with estates
	G.deckCount[thisPlayer] = 5;
	memset(G.deck[thisPlayer], 1, G.deckCount[thisPlayer]); // fill the deck with estates
	G.discardCount[thisPlayer] = 5;
	memset(G.discard[thisPlayer], 6, G.discardCount[thisPlayer]); // fill the discard with coppers
	G.hand[thisPlayer][1] = adventurer;
	
	memcpy(&testG, &G, sizeof(struct gameState)); // create a copy of gamestate object
	// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
	printf("Deck is estates, discard is golds... ");
	ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
	fail = 0;
	// check that they match
	if(ret!=0) {fail++;}
	if(fullDeckCount(thisPlayer, estate, &G) != 9) {fail++;} // no cards created or destroyed
	if(fullDeckCount(thisPlayer, gold, &G) != 5) {fail++;} // no cards created or destroyed
	if(G.handCount[thisPlayer] != 6)  {fail++;} 
	if(G.deckCount[thisPlayer] != 3)  {fail++;} 
	if(G.discardCount[thisPlayer] != 5)  {fail++;} 
	if(G.playedCardCount != 1)  {fail++;} // properly discarded
	if(G.playedCards[0] != adventurer)  {fail++;} // properly discarded
	if(memcmp(G.hand[!thisPlayer], testG.hand[!thisPlayer], G.handCount[!thisPlayer])) {fail++;} // other players unaffected
	if(memcmp(G.deck[!thisPlayer], testG.deck[!thisPlayer], G.deckCount[!thisPlayer])) {fail++;}
	if(memcmp(G.discard[!thisPlayer], testG.discard[!thisPlayer], G.discardCount[!thisPlayer])) {fail++;}
	// print out result
	if(fail) {printf("\tFAIL!\n");} else {printf("\tpass\n");}
	
	
	printf("Note: expect that every test will fail\n");
	
	// probably could use more
	

	
	
	return 0;
}




