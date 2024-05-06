#include "main.h"
#include <stdio.h>

/**show current deck**/
// Function for command SW to print a deck of cards
void showAllCards(struct deck *deck) {
    // Display the array of linked lists
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    int count = 1;
    int foundations = 0;
    int index = 1;
    struct card *current = deck->head;
    while (current != NULL) {
        printf("%c%c\t", current->value, current->suit);
        if (count % 7 == 0) {
            if (foundations % 2 == 0) {
                printf("\t\t[]\tF%d", index++);
            }
            foundations++;
            puts("");
        }
        current = current->next;
        count++;
    }
    puts("\n");
}

/**Print cards on display**/
// Function to print a deck of cards
void print_cards_in_deck(struct deck *deck) {
    // Display the array of linked lists
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    int count = 1;
    int foundations = 0;
    int index = 1;
    struct card *current = deck->head;
    while (current != NULL) {
        if (current->cardVisible) {
            printf("%c%c\t", current->value, current->suit);
        } else {
            printf("[]\t");
        }
        if (count % 7 == 0) {
            if (foundations % 2 == 0) {
                printf("\t\t[]\tF%d", index++);
            }
            foundations++;
            puts("");
        }
        current = current->next;
        count++;
    }
    puts("\n");
}


void printInitalSetup() {
    // Display the array of linked lists
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");
    int index = 1;
    for (int i = 0; i < 8; i++) {
        printf("\t\t\t\t\t\t\t");
        if (i % 2 != 0) {
            printf("\t\t[]\tF%d", index++);
        }
        puts("");
    }
    puts("\n");
}



//Function that initializes board's piles with cards from the deck
pile *initializePiles(deck *Deck) {
    int temp_length;
    pile *current_pile = new_pile();
    pile *head_of_pile = current_pile;
    node *current_node;

    card *current_card = Deck->head; // initialize the current card to the first card of the deck

    //Loop through NUM_COLUMNS 7 times to fill in each pile with cards
    for (int i = 0; i < NUM_COLUMNS; i++) {
        current_pile->id = i; // assign a unique id to each pile
        current_pile->head = new_node(current_card); // create a new node for each card
        if (i != 0) {
            current_card->cardVisible = false; // hide the card on the first pile
        } else {
            current_card->cardVisible = true;
        }
        current_pile->length = 1; // set the initial length of each pile to 1
        current_pile->tail = current_pile->head; // set the tail to the head of each pile
        current_pile->next = new_pile(); // create a new pile for the next iteration
        current_pile->next->prev = current_pile; // set the previous pointer of the next pile to the current pile
        current_pile = current_pile->next; // set the current pile to the next pile
        current_card = current_card->next; // set the current card to the next card
    }

    current_pile = head_of_pile;

    // Loop through remaining cards and add to the piles based on game rules
    while (current_card != NULL) {
        // If the current pile is the first pile and has only 1 card or if
        // it has reached the maximum number of cards allowed, move to next pile
        if (current_pile->id == 0) {
            current_pile = head_of_pile;
        }
        if ((current_pile->id == 0 && current_pile->length == 1) ||
            (current_pile->id == 1 && current_pile->length == 6) ||
            (current_pile->id == 2 && current_pile->length == 7) ||
            (current_pile->id == 3 && current_pile->length == 8) ||
            (current_pile->id == 4 && current_pile->length == 9) ||
            (current_pile->id == 5 && current_pile->length == 10) ||
            (current_pile->id == 6 && current_pile->length == 11)
                ) {
            // Skip this pile and move to next pile
            current_pile = current_pile->next;
            continue;
        }

        // Move the card to the target pile in display
        current_node = new_node(current_card); // current card
        current_node->prev = current_pile->tail; // set the previous pointer of the new node to the tail of the new target pile
        current_pile->tail->next = current_node; // set the new card at the end of the pile
        current_pile->tail = current_node; // set the tail of the current pile to the new node
        temp_length = current_pile->length; // increment the length of the current pile
        current_pile->length = ++temp_length;

        // Set visibility of card based on game rules
        if ((current_pile->id == 1 && current_pile->length <= 1) ||
            (current_pile->id == 2 && current_pile->length <= 2) ||
            (current_pile->id == 3 && current_pile->length <= 3) ||
            (current_pile->id == 4 && current_pile->length <= 4) ||
            (current_pile->id == 5 && current_pile->length <= 5) ||
            (current_pile->id == 6 && current_pile->length <= 6)) {
            current_card->cardVisible = false;
        } else {
            current_card->cardVisible = true;
        }

        // Move to the next card and the next pile
        current_card = current_card->next;
        current_pile = current_pile->next;
    }

    // Return the head of the pile linked list
    return head_of_pile;
}

// Function to display the current state of the card piles and foundations
void displayCardPiles(pile *head_of_pile, card *Foundations[]) {
// Initialize variables
    pile *current_pile = head_of_pile;
    int longest_pile = 0;
    int id_of_longest_pile = 0;
    int current_id = 0;
    int index = 0;
    int foundations = 0;
    node *current_card_in_pile[10];

// Iterate through each pile to determine the longest pile and its ID
    while (current_pile != NULL) {
        current_card_in_pile[index] = current_pile->head;
        if (longest_pile <= current_pile->length) {
            longest_pile = current_pile->length;
            id_of_longest_pile = current_pile->id;
        }
        index += 1;
        current_pile = current_pile->next;
    }

// If the longest pile is less than 8, set it to 8
    if (longest_pile < 8) {
        longest_pile = 8;
    }

// Display the column headers for the card piles
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    index = 0;

    card *current_card;

// Loop through each row of cards and display their values, or an empty space if there is no card
    while (longest_pile != 0) {
        if (current_card_in_pile[current_id] == NULL) {
            current_card = NULL;
        } else {
            current_card = current_card_in_pile[current_id]->assigned_card;
        }

        if (current_card != NULL && current_card->cardVisible) {
            printf("%c%c\t", current_card->value, current_card->suit);
        } else if (current_card != NULL && !current_card->cardVisible) {
            printf("[]\t");
        } else {
            printf("\t");
        }

        // If the current card is the last card in a pile, and it is a foundation pile, display the foundation
        if (current_id == 6) {
            if (foundations % 2 == 0 && index < 4) {
                if (Foundations[index] != NULL) {
                    printf("\t\t%c%c\tF%d", Foundations[index]->value, Foundations[index]->suit, index + 1);
                } else {
                    printf("\t\t[]\tF%d", index + 1);
                }
                index++;
            }
            foundations++;
            puts("");
        }

        // Decrement the longest pile variable if the current card is the last card in the longest pile
        if (id_of_longest_pile == current_id) {
            longest_pile--;
        }

        // Move to the next card in the current pile
        if (current_card_in_pile[current_id] == NULL) {
            current_card_in_pile[current_id] = NULL;
        } else {
            current_card_in_pile[current_id] = current_card_in_pile[current_id]->next;
        }
        // Move to the next pile if at the end of the current row, otherwise move to the next card in the row
        if (current_id == 6) {
            current_id = 0;
        } else {
            current_id += 1;
        }
    }
// Print a newline character to separate the output from other text
    puts("\n");
}