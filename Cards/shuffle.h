/*
 * shuffle.h
 *
 *  Created on: Mar 8, 2015
 *      Author: drajan
 */

#ifndef SHUFFLE_H_
#define SHUFFLE_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_CARDS 52

typedef struct node LLnode;

struct node {
	int data;
	LLnode* next;
};


unsigned int createCycle(LLnode** deck, LLnode** shuffle, unsigned int numCards );
void createDeck(LLnode** headref, LLnode** tailref, unsigned int N);
unsigned int computeLCM(unsigned int n1, unsigned int n2);
void findTail(LLnode* current, LLnode** tail);
unsigned int getGCD(unsigned int n1, unsigned int n2);
unsigned int getLCM(unsigned int x, unsigned int y);
void LLdelete(LLnode** headref, int data);
unsigned int LLfind(LLnode** headref, int index);
void LLInsertbeg(LLnode** headref, int data);
void LLInsertend(LLnode** headref, int data);
void LLremove(LLnode** headref, int data);
void permuteCards(LLnode** headOrig, LLnode** headMap, unsigned int numCards);


#endif /* SHUFFLE_H_ */
