#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

#define UNITTEST "remodel"

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
	total # players = 1,4
	supply counts = -5,20
	whoseTurn = 0,3
	handcount = 0,500
	deckcount = 0,500
	discardcount = 0,500
	playedcardcount = 0,500
	cards in the actual deck have values = -5,50
	choice1 discard = -20,handsize+20 (user-provided value, so may be illogical)
	choice2 supply = -10,treasure_map+10 (user-provided value, so may be illogical)
	set some sort of flag (for output printing) if an "illogical" user input is used
*/


int main() {
	// seed must be static, for reproducibility
	int seed = 1910;
	struct gameState G, testG;
	int ret=0, thisPlayer=0, fail=0;
	int bonus=0;
	int choice1=0, choice2=0;
	int i=0; // size iterator
	int k=0; // num_loops iterator
	int NUM_LOOPS = 200;
	int p=0; // player iterator
	int z=0; // number of succeeding tests
	char failcode[20], temp[5];
	int j=0; // the code for the discarded card
	int bad1 = 0, bad2 = 0; // flag for determining if inputs are bad
	int countjbefore=0, countjafter=0;
	int countgbefore=0, countgafter=0;
	
	
	SelectStream(2);
	PutSeed(seed);
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~randomtestcard2.c:   remodel~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	
	
	// begin loop
	for(k=0; k<NUM_LOOPS; k++) {
		memset(failcode, '\0', 20*sizeof(char)); // reset the error string
		//////////////// fill the whole struct with utter garbage
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&G)[i] = floor(Random() * 256);
		}
		
		//////////////// fill the relevant areas with more-reasonable garbage (overwrite most of the above statement)
		G.numPlayers = 1 + floor(Random() * MAX_PLAYERS);
		fillArrayRand(G.supplyCount, treasure_map+1, -5, 20);
		G.whoseTurn = floor(Random() * MAX_PLAYERS);
		fillArrayRand((int *)G.hand, MAX_HAND*MAX_PLAYERS, -5, 50); 	// fill hand with -5,50 all in one command
		fillArrayRand((int *)G.deck, MAX_DECK*MAX_PLAYERS, -5, 50); 	// fill deck with -5,50 all in one command
		fillArrayRand((int *)G.discard, MAX_DECK*MAX_PLAYERS, -5, 50); 	// fill disard with -5,50 all in one command
		fillArrayRand(G.playedCards, MAX_DECK, -5, 50); 				// fill playedcards with -5,50 all in one command
		fillArrayRand(G.handCount, MAX_PLAYERS, 0, MAX_HAND); 
		fillArrayRand(G.deckCount, MAX_PLAYERS, 0, MAX_DECK);
		fillArrayRand(G.discardCount, MAX_PLAYERS, 0, MAX_DECK);
		G.playedCardCount = floor(Random() * MAX_DECK);
		G.hand[G.whoseTurn][1] = remodel;				// add the remodel card to the hand
		memcpy(&testG, &G, sizeof(struct gameState)); 	// create a copy of gamestate object
		thisPlayer = G.whoseTurn;
		bonus = floor(Random() * 256);
		choice1 = -20 + floor(Random() * (G.handCount[thisPlayer]+40));  // set discard choice to -20,handsize+20 (user-provided value, so may be illogical)
		choice2 = -10 + floor(Random() * (treasure_map+20)); // set supply choice to -10,treasure_map+10 (user-provided value, so may be illogical)
		j = testG.hand[thisPlayer][choice1]; // store the discarded card here 
		
		//////////////// do the thing
		// cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus)
		// if(k==2 || k==58 || k==151) {showCards(&G,thisPlayer);}
		ret = cardEffect(remodel,choice1,choice2,0,&G,1,&bonus);
		// if(k==2 || k==58 || k==151) {printf("p1=%i, h[p1]=%i, p2=%i, s[p2]=%i, v[h[p1]]=%i, v[p2]=%i, r=%i\n",choice1,j,choice2,supplyCount(choice2, &testG),getCost(j), getCost(choice2),ret);}
		fail = 0; bad1 = 0; bad2 = 0;
		// if(k==2 || k==58 || k==151) {showCards(&G,thisPlayer);}
		
		
		
		//////////////// verify it worked right (HARD)
		// illogical user inputs: guaranteed failure
		if((choice1 < 0) || (choice1 >= testG.handCount[thisPlayer])) {bad1++;} // invalid discard index
		if((choice2 < 0) || (choice2 > treasure_map)) {bad1++;} // invalid supply pile
		if(bad1) {
sprintf(temp, "X,");strcat(failcode,temp); // X signifies bad user input ranges, may see some sort of pattern
		}else{
sprintf(temp, "-,");strcat(failcode,temp);
		}
		// other sources of failure: does it fail when it should?
		if(j==remodel) { // if wanting to discard remodel from playing remodel, need to make sure there are 2 in hand
			p = 0;
			for (i = 0; i < testG.handCount[thisPlayer]; i++) {
				if (testG.hand[thisPlayer][i] == j) {p++;}
			}
			if(p<2) {bad2++;} // should fail
		}
		if ((getCost(j) + 2) < getCost(choice2)) {bad2++;} // should fail, supply > discard+2
		if ( supplyCount(choice2, &testG) < 1 ) {bad2++;} // should fail, supply is empty
		if(bad2) {
sprintf(temp, "X,");strcat(failcode,temp); // Y signifies bad input for some other reason
		}else{
sprintf(temp, "-,");strcat(failcode,temp);
		}
		
		if((bad1 || bad2) && (ret != -1)) {fail++;} // does it return -1 when its supposed to?
		if(!(bad1 || bad2) && (ret != 0)) {fail++;}  // does it return 0 when its supposed to?
sprintf(temp, "%i,",fail);strcat(failcode,temp);
		
		// if ret=0, did it function right?
		if(ret == 0) {				// trashed one, played one, gained one, so total deck stays the same size
			if(G.handCount[thisPlayer] + G.deckCount[thisPlayer] + G.discardCount[thisPlayer] + G.playedCardCount \
					!= testG.handCount[thisPlayer] + testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] + testG.playedCardCount)  {fail++;} 
			countjafter = fullDeckCount(thisPlayer, j, &G); countjbefore = fullDeckCount(thisPlayer, j, &testG);
			countgafter = fullDeckCount(thisPlayer, choice2, &G); countgbefore = fullDeckCount(thisPlayer, choice2, &testG);
			if((j==choice2) && (j!=remodel) && (countjafter != countjbefore)) {fail++;} // gain the same you trash: total count unchanged
			if((j==choice2) && (j==remodel) && (countjafter != countjbefore-1)) {fail++;} // gain remodel, trash remodel, play remodel: goes into playedCards (not covered by fullDeckCount)
			if((j!=choice2) && (j!=remodel) && (countjafter != countjbefore-1)) {fail++;} // trash card j
			if((j!=choice2) && (j==remodel) && (countjafter != countjbefore-2)) {fail++;} // trash remodel card, also remodel goes into playedCards (not covered by fullDeckCount)
			if((j!=choice2) && (choice2!=remodel) && (countgafter != countgbefore+1)) {fail++;} // gain card choice2
			if((j!=choice2) && (choice2==remodel) && (countgafter != countgbefore)) {fail++;} // gain card remodel, play card remodel
		} else {	// might fail because supply > discard+2, or supply is empty, or invalid supply, or invalid discard(index or don't have)
			// if it fails, NOTHING CHANGES
			if(memcmp(&G, &testG, sizeof(struct gameState))) {fail++;}
		}
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
		// properly discarded, unless it failed
		if(!(bad1 || bad2) && (G.playedCards[G.playedCardCount-1] != remodel)) {fail++;} 
		if(!(bad1 || bad2) && (testG.playedCardCount + 1 != G.playedCardCount)) {fail++;}
		if((bad1 || bad2) && (G.hand[thisPlayer][1] != remodel)) {fail++;} // if it fails, NOTHING CHANGES
sprintf(temp, "%i,",fail);strcat(failcode,temp);
		
		//////////////// print the result
		if(fail) {
			// printf("iter%i FAIL! code=%i\n",k,fail);
			printf("iter%i FAIL! code=%s\n",k,failcode);
		} else {
			z++;
			// printf("iter%i pass, code=%s\n",k,failcode);
		}
		// if(k==2 || k==58 || k==151) {printf("%i~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",k);}
	}// repeat
	
	printf("total success rate: %i/%i, %i%%\n", z, NUM_LOOPS, (z*100) / NUM_LOOPS);
	
	return 0;
}
