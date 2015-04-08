/*
 * shuffle.c
 *
 *  Created on: Mar 7, 2015
 *      Author: drajan
 */
#include "shuffle.h"


/* --------------------------------------------------------------------------------------------------------------*/
// Program to shuffle a deck of cards and compute the number of shuffles required to restore deck to original order.
// Shuffle Procedure:
// 1. Take the top card off the deck and set it on the table.
// 2. Take the next card off the top and put it on the bottom of the deck in your hand.
// 3. Continue steps 1 and 2 until all cards are on the table.  This is a round.
/* --------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	unsigned int N = 0; /* Number of cards */
	printf("Please enter number of cards to be shuffled: ");
	scanf("%d", &N);

	if((N > 2) && (N <= MAX_CARDS)) /* Number of cards is restricted to 52 */
	{
		/* Initialize card decks as nodes of a linked list */
		LLnode *deckOrigHead = NULL;
		LLnode *deckShuffled = NULL;
		LLnode *deckOrigTail = NULL;

		unsigned int rounds = 0; /* Initialize result */

		createDeck(&deckOrigHead, &deckOrigTail, N); /* Create a deck of N cards */
		permuteCards(&deckOrigHead, &deckShuffled, N); /* Complete one round of shuffling as described in the problem statement */
		rounds = createCycle(&deckOrigHead, &deckShuffled, N); /* Create a cycle mapping between original and shuffled cards to compute result */

		printf("\nNumber of rounds required to shuffle to restore original order is:  %d \n", rounds);
	}
	else
	{
		printf("\n Error: Please enter a valid number of cards. \n");
	}
	return 0;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Creating a deck of N cards.
/* --------------------------------------------------------------------------------------------------------------*/
void createDeck(LLnode **headref, LLnode **tailref, unsigned int N){
	LLnode *current = *headref;
	unsigned int i;
	/* populate a linked list to be used as a deck of cards */
	for(i = N; i >= 1; i--){
		LLInsertbeg(&current, i);
		if(i == N){
			*tailref = current;
		}
	}

	*headref = current;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Performing the shuffling procedure described in the problem statement.
/* --------------------------------------------------------------------------------------------------------------*/
void permuteCards(LLnode **headOrig, LLnode **headMap, unsigned int numCards) {
	LLnode *current = *headOrig; /* analogous to deck in hand */
	LLnode *map = *headMap; /* analogous to deck on table */
	LLnode *tail = NULL;

	findTail(current, &tail);

	LLnode *tailOrig = NULL;
	tailOrig = tail;

	char flag = 1; /* controls the alternate movement of cards between original and map */

	while(current->next->next != NULL){ /* loop until only 2 cards remain in hand */
		flag = !flag;
		if(map!=NULL){
			/* reset the flag after reaching the last card in hand on first pass */
			if(current->data < map->data)
				flag = 0;
		}
		if (flag == 0){
			LLInsertbeg(&map, current->data); /* move odd numbered cards to deck on table */
			current = current->next;
		}
		else
		{
			LLInsertend(&tail->next, current->data); /* move even numbered cards to the bottom of deck in hand */
			tail = tail->next;
			current = current->next;
		}

	}
	/* Add the last two cards into the map and empty the deck in hand */
	if (current->data < map->data)
		flag = 1;

	if(flag == 0){
		LLInsertbeg(&map, current->next->data);
		LLInsertbeg(&map, current->data);
	}
	else
	{
		LLInsertbeg(&map, current->data);
		LLInsertbeg(&map, current->next->data);
	}
	/* used to ensure original deck is intact */
	tailOrig->next = NULL;
	*headMap = map;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Algorithm to compute the number of rounds required to restore shuffled deck.
// 1. Create a permutation mapping between original deck and the one round of shuffled deck.
// 2. The above mapping becomes the RULE that is followed by the given shuffling.
// 3. Repeated application of the RULE is bound to ensure the deck is restored, with each card tracing a specific path.
// 4. Such a path is called a cycle. The order of a cycle is the number of elements in it.
// 5. Thus, the number of times a RULE is applied must be a factor of all the cycles that describe the RULE.
// 6. The least common multiplier (LCM) of the all the cycle orders will be the result.
/* --------------------------------------------------------------------------------------------------------------*/
unsigned int createCycle(LLnode **deck, LLnode **shuffle, unsigned int numCards ){
	LLnode *orig = *deck;
	LLnode *map = *shuffle;
	unsigned int start = 1;
	int n = 1;
	char order = 0;
	unsigned int index;
	unsigned int lcm = 1;
	while(numCards > 0){
		do{
			index = LLfind(&map, n);

			if(index) /* if valid mapping exists */
			{
				if(numCards > 1)
					LLremove(&orig,n);
				order++;
				n = index; /* to find next map in the cycle.*/
				numCards--;
			}
		}while(start != index && numCards > 0);

		lcm = getLCM(lcm, order);
		order = 0; /* reset for next cycle */
		start = orig->data;
		n = start;
	}
	return lcm;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Find the mapping between the original and shuffled decks.
/* --------------------------------------------------------------------------------------------------------------*/
unsigned int LLfind(LLnode **headref, int index) {
	LLnode *current = *headref;
	int count = 1;
	while (current!= NULL) {
		if (count == index) {
			return(current->data);
		}
		count++;
		current = current->next;
	}

	printf("Invalid: Index not found.");
	return 0;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Remove a specific entry from the linked list.
/* --------------------------------------------------------------------------------------------------------------*/
void LLremove(LLnode **headref, int data){
  LLnode *current = *headref;

  // check if head is the element to be deleted
  if(current->data == data){
    current = (*headref)->next;
    *headref = current;
  }

  while(current->next!= NULL){
    if(current->next->data == data)
    {
      current->next = current->next->next;
      return;
    }
    current = current->next;
  }
}
/* --------------------------------------------------------------------------------------------------------------*/
// Very similar to LLdelete, but used only to delete the head of the linked list.
// (Note: This function can be combined with LLremove.)
/* --------------------------------------------------------------------------------------------------------------*/
void LLdelete(LLnode **headref, int data) {
	LLnode *current = *headref;

	/* check if head is the element to be deleted */
	if (current->data == data) {
		if((*headref)->next != NULL)
			current = (*headref)->next;
		else
			current = NULL;
	}
	*headref = current;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Insert a node to the beginning of a linked list.
/* --------------------------------------------------------------------------------------------------------------*/
void LLInsertbeg(LLnode **headref, int data) {
	LLnode *newnode = (LLnode*) malloc(sizeof(LLnode));
	newnode->data = data;
	newnode->next = *headref;

	*headref = newnode;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Insert a node at the end of a linked list.
/* --------------------------------------------------------------------------------------------------------------*/
void LLInsertend(LLnode **tailref, int data){
	LLnode *newnode = (LLnode*) malloc(sizeof(LLnode));
	newnode->data = data;
	newnode->next= 0;
	*tailref = newnode;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Traverse a linked list to reach the last node (tail).
/* --------------------------------------------------------------------------------------------------------------*/
void findTail(LLnode *head, LLnode **tail) {
	LLnode* current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	*tail = current;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Algorithm to compute the Least Common Multiplier (LCM) of two numbers.
/* --------------------------------------------------------------------------------------------------------------*/
unsigned int computeLCM(unsigned int n1, unsigned int n2) {
	/* initialize variables */
	unsigned int x = n2;
	unsigned int y = n1;
	unsigned int temp = 0;
	unsigned int lcm = 0;

	if ((n1 != 0) && (n2 != 0)) /* ensures inputs are valid */
	{
		if (n2 > n1) {
			/* swap numbers */
			x = n1;
			y = n2;
		}
		/* compute the greatest common divisor */
		while (x != 0) {
			temp = x;
			x = y % x;
			y = temp;
		}

		lcm = (n1 * n2) / y;

		return lcm;
	} else {
		printf("Error: Invalid Inputs to computeLCM()");

		return 0;
	}
}
/* --------------------------------------------------------------------------------------------------------------*/
// Alternate approach to compute LCM using Euler's method with a recursive function.
/* --------------------------------------------------------------------------------------------------------------*/
unsigned int getLCM(unsigned int x, unsigned int y) {
	unsigned int lcm = 0;
	if ((x != 0) && (y != 0)) /* ensures inputs are valid */
		{
	lcm = (x * y) / getGCD(x, y);
		}
	else
	{
		printf("Error: Invalid Inputs to getLCM");
	}

	return lcm;
}
/* --------------------------------------------------------------------------------------------------------------*/
// Recursive function to compute the Greatest Common Divisor (GCD) between two numbers.
/* --------------------------------------------------------------------------------------------------------------*/
unsigned int getGCD(unsigned int n1, unsigned int n2) {
	if ((n1 != 0) && (n2 != 0)) /* ensures inputs are valid */
	{
		/* base case */
		if (n1 == n2) {
			return n1;
		}

		/* recursive case */
		if (n1 > n2) {
			return getGCD(n1 - n2, n2);
		} else {
			return getGCD(n2 - n1, n1);
		}
	}
	else{

		printf("Error: Invalid Inputs to getGCD");
		return 1;
	}

}

