//
// Created by HP on 09-05-2023.
//

#ifndef SOLITAIRE_MAIN_H
#define SOLITAIRE_MAIN_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define a struct to represent a card
typedef struct card {
    char suit;
    char value;
    struct card *next;
    struct card *prev;
    bool cardVisible;
} card;

// Define a struct to represent a deck of cards
typedef struct deck {
    struct card *head;
    struct card *tail;
    int size;
    bool deckVisible;
} deck;

// Define a struct to represent a column pile
typedef struct pile {
    struct node *head;
    struct node *tail;
    struct pile *next;
    struct pile *prev;
    int length;
    int id;
} pile;

//Define a struct that contains a pointer to a "card" struct, and pointers to the next and previous nodes in a deck
typedef struct node {
    struct card *assigned_card;
    struct node *next;
    struct node *prev;
} node;

card *new_card(char suit, char value);
void attachCardToDeck(deck *deck, card *new_card);
void LD_default(deck*);
deck *new_deck();
void save_deck(deck *deck, char *filename);
#endif //SOLITAIRE_MAIN_H
