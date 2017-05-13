#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

#define UNITTEST "adventurer"

void showCards (struct gameState *G, int thisPlayer) {
	int i=0;
	printf("*Hand=%i:\t",G->handCount[thisPlayer]);
	for (i = 0; i < G->handCount[thisPlayer]; i++) {
		printf("%i, ",G->hand[thisPlayer][i]);
	}
	printf("\n*Deck=%i:\t",G->deckCount[thisPlayer]);
	for (i = 0; i < G->deckCount[thisPlayer]; i++) {
		printf("%i, ",G->deck[thisPlayer][i]);
	}
	printf("\n*Discard=%i:\t",G->discardCount[thisPlayer]);
	for (i = 0; i < G->discardCount[thisPlayer]; i++) {
		printf("%i, ",G->discard[thisPlayer][i]);
	}
	// printf("\n*Played=%i:\t",G->playedCardCount);
	// for (i = 0; i < G->playedCardCount; i++) {
		// printf("%i, ",G->playedCards[i]);
	// }
	printf("\n");
}

// basically memset but for ints
// memset(G.hand[thisPlayer], 1, G.handCount[thisPlayer]*sizeof(int));
void fillArray (int A[], int length, int val) {
	int i = 0;
	for(i = 0; i < length; i++) {
		A[i] = val;
	}
}
// fills the array randomly, rather than evenly
// memset(&G, Random()%256, sizeof(struct gameState));
void fillArrayRand (int A[], int length, int lower, int upper) {
	int i = 0;
	for(i = 0; i < length; i++) {
		A[i] = lower + floor(Random() * (upper-lower));
	}
}


/*
reasonable constraints for the random values:
do I want to ensure it doesnt read outside the bounds of the struct? or really, really random testing?
	total # players = [0,5] 
	whoseTurn = [0,6]
	handcount = -10,600
	deckcount = -10,600
	discardcount = -10,600
	playedcardcount = -10,600
	cards in the actual deck have values = -5,50
	
	
*/


int main() {
	// seed must be static, for reproducibility
	int seed = 1910;
	// kingdom cards must be static, for reproducbility
	// int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	// int numPlayers = 2;
	int ret=0, thisPlayer=0, fail=0;
	int bonus=0;
	int i=0; // size iterator
	int k=0; // num_loops iterator
	int NUM_LOOPS = 200;
	int p=0; // player iterator
	int z=0; // number of succeeding tests
	char failcode[20], temp[5];
	
	
	SelectStream(2);
	PutSeed(seed);
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~randomtestadventurer.c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("note that all tests are expected to fail\n");
	
	// begin loop
	for(k=0; k<NUM_LOOPS; k++) {
		memset(failcode, '\0', 20*sizeof(char)); // reset the error string
		//////////////// fill the whole struct with utter garbage
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&G)[i] = floor(Random() * 256);
		}

		//////////////// fill the relevant areas with more-reasonable garbage (overwrite most of the above statement)
		G.numPlayers = 1 + floor(Random() * MAX_PLAYERS); 			// unsure, may cause out-of-memory access
		fillArrayRand(G.supplyCount, treasure_map+1, -5, 20);
		G.whoseTurn = floor(Random() * MAX_PLAYERS);  				// unsure, may cause out-of-memory access
		fillArrayRand((int *)G.hand, MAX_HAND*MAX_PLAYERS, -5, 50); 	// fill hand with -5,50 all in one command
		fillArrayRand((int *)G.deck, MAX_DECK*MAX_PLAYERS, -5, 50); 	// fill deck with -5,50 all in one command
		fillArrayRand((int *)G.discard, MAX_DECK*MAX_PLAYERS, -5, 50); 	// fill disard with -5,50 all in one command
		fillArrayRand(G.playedCards, MAX_DECK, -5, 50); 				// fill playedcards with -5,50 all in one command
		fillArrayRand(G.handCount, MAX_PLAYERS, 0, MAX_HAND); 			// unsure, may cause out-of-memory access
		fillArrayRand(G.deckCount, MAX_PLAYERS, 0, MAX_DECK); 			// unsure, may cause out-of-memory access
		fillArrayRand(G.discardCount, MAX_PLAYERS, 0, MAX_DECK); 		// unsure, may cause out-of-memory access
		G.playedCardCount = floor(Random() * MAX_DECK); 	// unsure, may cause out-of-memory access
		G.hand[G.whoseTurn][1] = adventurer;				// add the adventurer card to the hand
		memcpy(&testG, &G, sizeof(struct gameState)); 		// create a copy of gamestate object
		thisPlayer = G.whoseTurn;
		bonus = floor(Random() * 256);
		
		//////////////// do the thing
		// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
		ret = cardEffect(adventurer,0,0,0,&G,1,&bonus);
		fail = 0;
		
		//////////////// verify it worked right (HARD)
sprintf(temp, "%i,",fail);strcat(failcode,temp);
		if(ret == 0) {				// found 2 treasure
			if(G.handCount[thisPlayer] != testG.handCount[thisPlayer] + 1)  {fail++;} 	
			if((G.hand[thisPlayer][1] != gold) && (G.hand[thisPlayer][1] != silver) && (G.hand[thisPlayer][1] != copper))  {fail++;}  // treasure goes here when adventurer is discarded
			if((G.hand[thisPlayer][G.handCount[thisPlayer]-1] != gold) && (G.hand[thisPlayer][G.handCount[thisPlayer]-1] != silver) && (G.hand[thisPlayer][G.handCount[thisPlayer]-1] != copper))  {fail++;} 			
		} else {					// deck empty, no treasure remaining
			if(G.deckCount[thisPlayer] != 0)  {fail++;}
			for (i = 0; i < G.discardCount[thisPlayer]; i++) {
				if ((G.discard[thisPlayer][i] == gold) || (G.discard[thisPlayer][i] == silver) || (G.discard[thisPlayer][i] == copper)) { fail++; break; }
			}
		}
sprintf(temp, "%i,",fail);strcat(failcode,temp);
		// no cards created or destroyed
		if(fullDeckCount(thisPlayer, curse, &G) != fullDeckCount(thisPlayer, curse, &testG)) {fail++;} 
		if(fullDeckCount(thisPlayer, estate, &G) != fullDeckCount(thisPlayer, estate, &testG)) {fail++;} 
		if(fullDeckCount(thisPlayer, copper, &G) != fullDeckCount(thisPlayer, copper, &testG)) {fail++;} 
		if(fullDeckCount(thisPlayer, gold, &G) != fullDeckCount(thisPlayer, gold, &testG)) {fail++;} 
		if(fullDeckCount(thisPlayer, silver, &G) != fullDeckCount(thisPlayer, silver, &testG)) {fail++;} 
		if(G.handCount[thisPlayer] + G.deckCount[thisPlayer] + G.discardCount[thisPlayer] + G.playedCardCount \
				!= testG.handCount[thisPlayer] + testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.playedCardCount)  {fail++;} 
sprintf(temp, "%i,",fail);strcat(failcode,temp);
		// other players unaffected
		for(p=0; p<G.numPlayers; p++) { 
			if(p==thisPlayer) continue;
			if(G.handCount[p] != testG.handCount[p]) {fail++;} 
			if(G.deckCount[p] != testG.deckCount[p]) {fail++;} 
			if(G.discardCount[p] != testG.discardCount[p]) {fail++;} 
			if(memcmp(G.hand[p], testG.hand[p], G.handCount[p]*sizeof(int))) {fail++;} 
			if(memcmp(G.deck[p], testG.deck[p], G.deckCount[p]*sizeof(int))) {fail++;}
			if(memcmp(G.discard[p], testG.discard[p], G.discardCount[p]*sizeof(int))) {fail++;}
		}
sprintf(temp, "%i,",fail);strcat(failcode,temp);
		// properly discarded
		if(G.playedCards[G.playedCardCount-1] != adventurer) {fail++;} 
		if(testG.playedCardCount + 1 != G.playedCardCount) {fail++;}
sprintf(temp, "%i,",fail);strcat(failcode,temp);
		
		//////////////// print the result
		if(fail) {
			// printf("iter%i FAIL! code=%i\n",k,fail);
			printf("iter%i FAIL! code=%s\n",k,failcode);
		} else {
			z++;
			// printf("iter%i pass\n",k);
		}
		
	}// repeat
	
	printf("total success rate: %i/%i, %i%%\n", z, NUM_LOOPS, (z*100) / NUM_LOOPS);
	
	return 0;
}
