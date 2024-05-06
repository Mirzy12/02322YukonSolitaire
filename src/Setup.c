#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"


/****************** Load default deck unshuffled *****************/
// Function to create a new deck of cards by assigning suits and values
void LD_default(deck *newDeck) {
    newDeck->head = NULL;
    newDeck->tail = NULL;
    newDeck->size = 0;

    // Populate the deck with cards
    int i, j;
    for (i = 0; i < NUM_SUITS; i++) {
        for (j = 0; j < NUM_VALUES; j++) {
            // set the suits based on the current iteration of the loop
            char suit;
            if (i == 0) {
                suit = 'H'; //Hearts
            } else if (i == 1) {
                suit = 'D'; //Diamonds
            } else if (i == 2) {
                suit = 'C'; //Clubs
            } else {
                suit = 'S'; //Spades
            }
            //Assign the lettered cards
            char value;
            if (j == 0) {
                value = 'A'; //Ace
            } else if (j == 9) {
                value = 'T'; //Ten
            } else if (j == 10) {
                value = 'J'; //Jack
            } else if (j == 11) {
                value = 'Q'; //Queen
            } else if (j == 12) {
                value = 'K'; //King
            } else {
                value = (j + 1) + '0'; //numbered values
            }

            // Create a new card with the determined suit and value
            card *card = new_card(suit, value);
            // Attach new card to the deck
            attachCardToDeck(newDeck, card);

        }
    }
    printf("Finished creating deck of %d cards.\n", newDeck->size);
}


/********************** Split shuffle your deck **********************/

// Function to split shuffle a card
struct card *splitShuffle(struct deck *deck) {
    // Allocate memory for the shuffled card
    struct deck *shuffledDeck = malloc(sizeof(struct deck));
    shuffledDeck->head = NULL;
    shuffledDeck->tail = NULL;
    shuffledDeck->size = 0;

    // Split the card into two halves
    struct card *firstHalf = deck->head;
    struct card *secondHalf = NULL;
    struct card *current = deck->head;

    int i;
    for (i = 0; i < deck->size / 2; i++) {
        current = current->next;
    }
    secondHalf = current;

    // Interleave the cards from the two halves
    for (i = 0; i < deck->size / 2; i++) {
        // Take the i-th card from the first half
        struct card *firstCard = firstHalf;
        int j;
        for (j = 0; j < i; j++) {
            firstCard = firstCard->next;
        }

        // Take the i-th card from the second half
        struct card *secondCard = secondHalf;
        for (j = 0; j < i; j++) {
            secondCard = secondCard->next;
        }

        // Add the two cards to the shuffled card
        struct card *newCard1 = new_card(firstCard->suit, firstCard->value);
        attachCardToDeck(shuffledDeck, newCard1);

        struct card *newCard2 = new_card(secondCard->suit, secondCard->value);
        attachCardToDeck(shuffledDeck, newCard2);
    }

    return shuffledDeck;
}

/*************** Random shuffle your deck *******************/
// Function to randomly shuffle a deck
struct deck *randomshuffle_deck(struct deck *deck) {
    int i, j;

    // Set the seed for the random number generator
    srand(time(NULL));

    // Loop from size - 1 to 1 in reverse order
    for (i = deck->size - 1; i > 0; i--) {
        // Generate a random number j between 0 and i (inclusive)
        j = rand() % (i + 1);

        // Set current_i to point to the ith card in the deck
        struct card *current_i = deck->head;
        int k;
        for (k = 0; k < i; k++) {
            current_i = current_i->next;
        }
        struct card *card_i = current_i;

        // Set current_j to point to the jth card in the deck
        struct card *current_j = deck->head;
        for (k = 0; k < j; k++) {
            current_j = current_j->next;
        }
        struct card *card_j = current_j;

        // Swap the suits of card_i and card_j
        char temp_suit = card_i->suit;
        card_i->suit = card_j->suit;
        card_j->suit = temp_suit;

        // Swap the values of card_i and card_j
        char temp_value = card_i->value;
        card_i->value = card_j->value;
        card_j->value = temp_value;
    }

    // Return shuffled deck
    return deck;
}

