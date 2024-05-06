#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "main.h"
#include "Setup.h"


void LD_file(deck*, char*);

//LD a Deck

deck* LD(char* input) {
    deck *deck = new_deck();
    //deck *deck = malloc(sizeof (deck));

    if(strlen(input) > 0)
        LD_file(deck, input);
    else
        LD_default(deck);

    return deck;
}


void LD_file(deck* deck, char* input) {
    FILE *fp;
    char str[52];
    int count = 0;
    int line = 0;

    if (input[0] == '\0') {
        fp = fopen("DefaultDeck", "r");
        printf("OK!\n");
    }
        // opening file for reading
    else {
        fp = fopen(input, "r");
    }

    if (fp == NULL) {
        perror("File does not exist");
    }

    while (fgets(str, 200, fp)) {
        //puts(str);
        line++;
        count++;

        char value = str[0];
        char suit = str[1];

        if (!((value >= '2' && value <= '9') || value == 'T' || value == 'A' || value == 'K' || value == 'Q' ||
              value == 'J')) {
            printf("Error Message: Incorrect card value on line %d: %s\n", line, str);
            fclose(fp);

            // writing content to terminal
            //puts(str);
        }

        if (suit != 'C' && suit != 'D' && suit != 'S' && suit != 'H') {
            printf("Error Message: card suit incorrect on line %d: %s\n", line, str);
            fclose(fp);

        }

        card *card = new_card(suit, value);
        card->cardVisible = false;
        attachCardToDeck(deck, card);
    }
}


// This function saves the cards in a deck to a file in a specific format
void SaveDeckCards(pile *head_of_pile) {
    pile *current_pile = head_of_pile; // pointer to the current pile being processed

    // Initialize some variables
    int longest_pile = 0; // length of the longest pile of cards
    int id_of_longest_pile = 0; // id of the pile with the most cards
    int current_id = 0; // current pile id
    int index = 0; // index used to keep track of the current card in the pile
    node *current_card_in_pile[10]; // array of pointers to nodes, used to keep track of the current card in each pile


    // Loop through all piles to find the longest one and populate the array of current cards in each pile
    while (current_pile != NULL) {
        current_card_in_pile[index] = current_pile->head; // store a pointer to the head node of the current pile
        if (longest_pile <= current_pile->length) { // check if the current pile is longer than the longest pile so far
            longest_pile = current_pile->length; // if it is, update the longest_pile variable
            id_of_longest_pile = current_pile->id; // and store the id of the current pile as the id of the pile with the most cards
        }
        index += 1; // increment the index
        current_pile = current_pile->next; // move to the next pile
    }

    // If the longest pile has less than 8 cards, set its length to 8
    if (longest_pile < 8) {
        longest_pile = 8;
    }

    // Open the file for writing
    card *current_card;
    FILE *stream;
    stream = fopen(LOADFILE, "w");
    fclose(stream);

    // Re-open the file for appending
    stream = fopen(LOADFILE, "a");
    char newData[4];
    bool notFirstLine = false;

    // Loop through the cards in the deck
    while (longest_pile != 0) {
        // Get the card at the current position
        if (current_card_in_pile[current_id] == NULL) { // if the current node is NULL, set the current card to NULL
            current_card = NULL;
        } else {
            current_card = current_card_in_pile[current_id]->assigned_card; // otherwise, get the card assigned to the current node
        }

        // If there is a current card, save it to the file
        if (current_card != NULL) {
            newData[0] = current_card->value; // copy the card value to newData
            newData[1] = current_card->suit; // copy the card suit to newData
            if (notFirstLine) { // if this is not the first line of the file, add a newline character before writing the data
                fprintf(stream, "\n");
            }
            notFirstLine = true; // set the notFirstLine flag to true
            fwrite(newData, sizeof(char), strlen(newData), stream); // write the data to the file
        }

        // If the current pile is the longest pile, decrement longest_pile
        if (id_of_longest_pile == current_id) {
            longest_pile--;
        }

        // Move to the next card in the current pile
        if (current_card_in_pile[current_id] == NULL) {
            current_card_in_pile[current_id] = NULL;
        } else {
            current_card_in_pile[current_id] = current_card_in_pile[current_id]->next;
        }
        //The code then checks whether the current_id is equal to 6, which is the maximum index for the current_card_in_pile array
        if (current_id == 6) {
            //Index is reset to 0 to start at the beginning of the array again.
            current_id = 0;
            //Index is incremented by 1 to move to the next pile in the array
        } else {
            current_id += 1;
        }
    }
    //stream is closed
    fclose(stream);
}