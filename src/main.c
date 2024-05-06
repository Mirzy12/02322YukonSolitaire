#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Setup.h"
#include "Display.h"
#include "gamePlay.h"
#include "LoadGame.h"

#define NUM_SUITS 4
#define NUM_VALUES 13
#define LOADFILE "../cards.txt"
#define NUM_COLUMNS 7


/***
 * create your cards and card deck here
 * ***/

// Struct for creating a single card
card *new_card(char suit, char value) {
    struct card *newCard = malloc(sizeof(struct card));
    newCard->suit = suit;
    newCard->value = value;
    newCard->next = NULL;
    newCard->prev = NULL;
    newCard->cardVisible = true;

    return newCard;
}


// Struct for creating a deck of cards
deck *new_deck() {
    deck *deck = malloc(sizeof(deck));
    deck->head = NULL;
    deck->tail = NULL;
    deck->size = 0;
    return deck;
}


// Struct that represents the card on the board display
node *new_node(card *someCard) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->assigned_card = someCard;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

// Struct that represents the columns of cards in the game display,
// and used for manipulating the piles of cards on the board
pile *new_pile() {
    pile *pile1 = malloc(sizeof(pile));
    pile1->head = NULL;
    pile1->tail = NULL;
    pile1->next = NULL;
    pile1->prev = NULL;
    pile1->length = 0;
    return pile1;
}


//function that attaches a card to the end of the deck
void attachCardToDeck(deck *deck, card *new_card) {

    if (deck == NULL || new_card == NULL) {
        fprintf(stderr, "Invalid deck or card provided\n");
        return;  // It's a good practice to handle possible NULL inputs
    }

    if (deck->tail == NULL) {
        deck->head = new_card;
        deck->tail = new_card;
    } else {
        // otherwise, add the new card to the end of the deck
        deck->tail->next = new_card;
        deck->tail = new_card;
    }

    // increment the size of the deck and return the new card
    deck->size += 1;
}

void updateMessage(char *Message, const char *newMessage) {
    strncpy(Message, newMessage, 99);  // Copy the new message into the Message buffer, ensuring not to overflow
    Message[99] = '\0'; // Ensure null termination in edge cases
}

char Message[100] = "";


int main(int argc, char *argv[]) {
    // Create a deck of cards
    card *Foundation[5];

    for (int i = 0; i <= 4; i++) {
        Foundation[i] = NULL;
    }

    deck *Deck = new_deck();

    LD_default(Deck);

    printInitalSetup();
    pile *head_of_pile = initializePiles(Deck);
    char LastCommand[100] = "";

    int sourceColumn;
    int targetColumn;
    char cardValue;
    char cardSuit;
    int displayInitialized = 0;

    //While loop of the main game, commands and last commands that are saved
    while (1) {
        char input[50];
        printf("LAST Command: %s\n", LastCommand);
        printf("Message: %s\n", Message);
        printf("INPUT> ");
        scanf("%s", input);


        strcpy(LastCommand, input);

        if (strcmp(input, " ") == 0) {
        } else if (strcmp(input, "P") == 0) {
            displayInitialized = 1;
            displayCardPiles(head_of_pile, Foundation);
        } else if (strcmp(input, "Q") == 0) {
            deck *Deck = LD(LOADFILE);
            print_cards_in_deck(Deck);
        } else if (strcmp(input, "LD") == 0) {
            deck *Deck = LD(LOADFILE);
            print_cards_in_deck(Deck);
            head_of_pile = initializePiles(Deck);
        } else if (strcmp(input, "SW") == 0) {
            showAllCards(Deck);
        } else if (strcmp(input, "SI") == 0) {
            Deck = splitShuffle(Deck);
            print_cards_in_deck(Deck);
        } else if (strcmp(input, "SR") == 0) {
            Deck = randomshuffle_deck(Deck);
            showAllCards(Deck);
        } else if (strcmp(input, "SD") == 0) {
            SaveDeckCards(head_of_pile);
            printf("Save\n");
        } else if (strstr(input, ":") && strstr(input, "->")) {
            int parseCount = sscanf(input, "C%d:%c%c->C%d", &sourceColumn, &cardValue, &cardSuit, &targetColumn);
            if (parseCount != 4) {
                updateMessage(Message,
                              "Error: Invalid command format. Please use 'C<sourceColumn>:ValueSuit->C<targetColumn>'.\n");
            } else {
                sourceColumn--;
                targetColumn--;
                if (sourceColumn >= 0 && sourceColumn < NUM_COLUMNS && targetColumn >= 0 &&
                    targetColumn < NUM_COLUMNS) {
                    moveSpecificCard(head_of_pile, sourceColumn, targetColumn, cardValue, cardSuit);
                } else {
                    printf("Error: Column numbers must be between 1 and %d.\n", NUM_COLUMNS);
                }
            }
        } else if (strstr(input, "->") != 0) {
            sourceColumn = input[1] - '0';
            targetColumn = input[5] - '0';

            sourceColumn = sourceColumn - 1;
            targetColumn = targetColumn - 1;

            // Foundation movement commands
            if (strstr(input, "F1") != 0) {
                Foundation[0] = moveCardToFoundation(head_of_pile, sourceColumn);
            } else if (strstr(input, "F2") != 0) {
                Foundation[1] = moveCardToFoundation(head_of_pile, sourceColumn);
            } else if (strstr(input, "F3") != 0) {
                Foundation[2] = moveCardToFoundation(head_of_pile, sourceColumn);
            } else if (strstr(input, "F4") != 0) {
                Foundation[3] = moveCardToFoundation(head_of_pile, sourceColumn);
            } else {
                moveCard(head_of_pile, sourceColumn, targetColumn);
            }

            if (checkWinState(head_of_pile)) {
                printf("Winner Winner, Chicken Dinner!\n");
                break;
            }
        } else if (strcmp(input, "QQ") == 0) {
            printf("BYE BYE BYE!\n");
            break;
        } else if (strcmp(input, "W") == 0) {
            printf("Winner Winner, Chicken Dinner!\n");
            break;
        }
        if (displayInitialized) {
            displayCardPiles(head_of_pile, Foundation);
        }

    }
    return 0;
}

