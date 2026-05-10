#include "main.h"
#include "LD.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


// Define number of suits, values and columns using directives
#define NUM_SUITS 4
#define NUM_VALUES 13
// If running on another machine, do update LOADFILE to the correct path.
#define NUM_COLUMNS 7
const char order[] = "A23456789TJQK";
typedef enum { StartUp, Play } Phase;
// Function to create a new card and insert into linked list
card *new_card(char suit, char value) {
    // Allocate memory for a new card and initialize its properties
    struct card *newCard = malloc(sizeof(struct card));
    newCard->suit = suit;
    newCard->value = value;
    newCard->next = NULL;
    newCard->prev = NULL;
    newCard->cardVisible = true;


    return newCard;
}

node *new_node(card *someCard) {
    struct node *newNode = malloc(sizeof (struct node));
    newNode->assigned_card = someCard;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

//if deck is empty, set the new card as the head and tail of the deck

deck *new_deck() {
    deck *deck = malloc(sizeof(deck));
    deck->head = NULL;
    deck->tail = NULL;
    deck->size = 0;
    return deck;
}

pile *new_pile() {
    pile *pile1 = malloc(sizeof (pile));
    pile1->head = NULL;
    pile1->tail = NULL;
    pile1->next = NULL;
    pile1->prev = NULL;
    pile1->length = 0;
    return pile1;
}

void attachCardToDeck(deck *deck, card *new_card) {
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
bool isRedSuit(char suit) {
    return (suit == 'H' || suit == 'D');
}

bool isValidMove(card *moving, card *target) {

    // if target is NULL (empty column)
    if (target == NULL) {
        // only king is allowed on empty column
        return (moving->value == 'K');

    }
    // checks for color of suit
    if (isRedSuit(moving->suit) == isRedSuit(target->suit)) {
        return false;
    }

    // Check for ranking of cards

    int moving_index = -1;
    int target_index = -1;

    for (int i = 0; i < 13; i++) {
        if (order[i] == moving->value) moving_index = i;
        if (order[i] == target->value) target_index = i;
    }
    // card moved has to be less than the targeted card
    return (moving_index == target_index - 1);
}

// Function to create a new deck of cards
void LD_default(deck *newDeck) {
    newDeck->head = NULL;
    newDeck->tail = NULL;
    newDeck->size = 0;

    // Populate the deck with cards
    int i, j;
    for (i = 0; i < NUM_SUITS; i++) {
        for (j = 0; j < NUM_VALUES; j++) {
            // set the suit based on the current iteration of the loop
            char suit;
            if (i == 0) {
                suit = 'H';
            } else if (i == 1) {
                suit = 'D';
            } else if (i == 2) {
                suit = 'C';
            } else {
                suit = 'S';
            }

            char value;
            if (j == 0) {
                value = 'A';
            } else if (j == 9) {
                value = 'T';
            } else if (j == 10) {
                value = 'J';
            } else if (j == 11) {
                value = 'Q';
            } else if (j == 12) {
                value = 'K';
            } else {
                value = (j + 1) + '0';
            }

            card *card = new_card(suit, value);
            attachCardToDeck(newDeck, card);

        }
    }
    printf("Finished creating deck of %d cards.\n", newDeck->size);
}

// Function for SW to print a deck of cards
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

void printInitialSetup() {
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

struct deck* splitShuffle(struct deck* deck, int split) {
    // Allocate memory for the shuffled deck
    struct deck* shuffledDeck = malloc(sizeof(struct deck));
    shuffledDeck->head = NULL;
    shuffledDeck->tail = NULL;
    shuffledDeck->size = 0;

    // Split the deck into two halves
    struct card* firstHalf = deck->head;
    struct card* secondHalf = NULL;
    struct card* current = deck->head;
    struct card* prev = NULL;

    int i;
    // split point
    for (i = 0; i < split; i++) {
        prev = current;
        current = current->next;
    }
    secondHalf = current;

    // splitting the linked list
    if (prev != NULL) {
        prev->next = NULL;
    }

    // sizes of the two halves
    int firstSize = split;
    int secondSize = deck->size - split;

    // smallest half
    int minSize = firstSize < secondSize ? firstSize : secondSize;

    // Interleaving the card
    for (i = 0; i < minSize; i++) {
        // Take the i-th card from the first half
        struct card* firstCard = firstHalf;
        // Take the i-th card from the second half
        struct card* secondCard = secondHalf;

        int j;
        for (j = 0; j < i; j++) {
            firstCard = firstCard->next;
        }

        for (j = 0; j < i; j++) {
            secondCard = secondCard->next;
        }

        // Add the two cards to the shuffled deck
        struct card* newCard1 = new_card(firstCard->suit, firstCard->value);

        attachCardToDeck(shuffledDeck, newCard1);

        struct card* newCard2 = new_card(secondCard->suit, secondCard->value);

        attachCardToDeck(shuffledDeck, newCard2);
    }
    // adding the remaining cards from the first half
    struct card* remaining = firstHalf;

    for (i = 0; i < minSize; i++) {
        remaining = remaining->next;
    }

    while (remaining != NULL && firstSize> secondSize) {
        struct card* newCard = new_card(remaining->suit, remaining->value);

        attachCardToDeck(shuffledDeck, newCard);
        remaining = remaining->next;
    }

    // Add remaining cards from second half
    remaining = secondHalf;
    for (i = 0; i < minSize; i++) {
        remaining = remaining->next;
    }

    while (remaining != NULL && secondSize > firstSize) {
        struct card* newCard = new_card(remaining->suit, remaining->value);

        attachCardToDeck(shuffledDeck, newCard);
        remaining = remaining->next;
    }

    return shuffledDeck;
}



// Define a function that takes in a pointer to a deck struct and returns a pointer to a deck struct
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

// Return the pointer to the shuffled deck
    return deck;
}

bool canMoveCardToFoundation(card *moving, card *top) {
    if (top == NULL) {
        return (moving->value == 'A');
    }

    if (moving->suit != top->suit) return false;

    int m = -1, t = -1;
    for (int i = 0; i < 13; i++) {
        if (order[i] == moving->value) m=i;
        if (order[i] == top->value) t=i;
    }
    if (m == -1 || t == -1) return false;

    return (m == t +1);
}
card *moveCardToFoundation(pile *head, int source_column,int target_foundation, pile *Foundation[]) {
    // Check that source_column and target_column are valid indices
    if (source_column < 0 || source_column >= NUM_COLUMNS) {
        printf("Invalid column indices.\n");
        return NULL;
    }

    // Check that the source column is not empty
    if (head == NULL) {
        printf("Source column is empty.\n");
        return NULL;
    }

    pile *current = head;

    while(current->id != source_column) {
        current = current->next;
    }

    // Find the last card in the source column
    node *source_tail = current->tail;

    if (source_tail == NULL) {
        printf("Source column is empty.\n");
        return NULL;
    }

    card *moving =source_tail->assigned_card;

    // Find the foundation index
    int f = target_foundation;
    if (f < 0 || f>=4) {
        printf("Invalid foundation.\n");
        return NULL;
    }
    card *top = NULL;
    if (Foundation[f]->tail != NULL) {
        top = Foundation[f]->tail->assigned_card;
    }

    //Check if it's a valid move to foundation
    if (!canMoveCardToFoundation(moving, top)) {
        printf("Invalid move.\n");
        return NULL;
    }

    //Remove card from source column
    if (source_tail->prev == NULL) {
        current->head = NULL;
        current->tail = NULL;
    } else {
        current->tail = source_tail->prev;
        current->tail->next = NULL;
        source_tail->prev = NULL;
    }
    current->length--;


    //Flip the next card
    if (current->tail != NULL && !current->tail->assigned_card->cardVisible) {
        current->tail->assigned_card->cardVisible = true;
    }
    //Foundation[f] = moving;
    node *newNode = new_node(moving);

    if (Foundation[f]->head == NULL) {

        Foundation[f]->head = newNode;
        Foundation[f]->tail = newNode;
    } else {
        Foundation[f]->tail->next = newNode;
        newNode->prev = Foundation[f]->tail;
        Foundation[f]->tail = newNode;
    }
    Foundation[f]->length++;

    return moving;
}

bool moveCard(pile *head, int source_column, int target_column) {
    // Check that source_column and target_column are valid indices
    if (source_column < 0 || source_column >= NUM_COLUMNS || target_column < 0 || target_column >= NUM_COLUMNS) {
        printf("Invalid column indices.\n");
        return false;
    }

    // Check that the source column is not empty
    if (head == NULL) {
        printf("Source column is empty.\n");
        return false;
    }

    pile *current = head;
    pile *source_pile;
    pile *target_pile;

    while(current->id != source_column) {
        current = current->next;
    }
    source_pile = current;

    // Find the last card in the source column
    node *source_card = current->tail;
    if (source_card == NULL) {
        printf("Source column is empty.\n");
        return false;
    }
   // while (source_card->next != NULL) {
     //   source_card = source_card->next;
    //}

    // Find the target pile (before we remove a card)
    pile *temp=head;
    while (temp->id != target_column) {
        temp=temp->next;
    }
    target_pile = temp;

    // Get last card in target pile
    card *target_card = target_pile->tail ? target_pile->tail->assigned_card : NULL;

    // Validate the move
    if (!isValidMove(source_card->assigned_card, target_card)) {
        printf("Invalid move.\n");
        return false;
    }

    // Remove the card from the source column
    if (source_card->prev == NULL) {
        // The source column only had one card
        current->head = NULL;
        current->tail = NULL;
    } else {
        source_pile->tail = source_card->prev;
        source_pile->tail->next = NULL;
        source_card->prev = NULL;
    }
    //Flip the new top card in the source pile
    if (source_pile->tail != NULL &&
        source_pile->tail->assigned_card->cardVisible == false) {
        source_pile->tail->assigned_card->cardVisible = true;
    }
    // Add the card to the end of the target column
    //if (target_pile->head == NULL && target_pile->tail == NULL)
    if (target_pile->tail == NULL){
        // The target column is empty
        target_pile->head = source_card;
        target_pile->tail = source_card;
        source_card->prev = NULL;
    } else {
        // Find the last card in the target column
        node *target_tail = target_pile->tail;

        // Add the card to the end of the target column
        target_tail->next = source_card;
        source_card->prev = target_tail;
        target_pile->tail = source_card;
    }

    node *current_node = source_pile->head;
    int temp_length = 0;
    while(current_node != NULL) {
        current_node = current_node->next;
        temp_length++;
    }
    source_pile->length = temp_length;

    temp_length = 0;
    current_node = target_pile->head;
    while(current_node != NULL) {
        current_node = current_node->next;
        temp_length++;
    }
    target_pile->length = temp_length;

    return true;
}

bool moveSpecificCard(pile *head, int source_column, int target_column, char value, char suit) {
    if (source_column < 0 || source_column >= NUM_COLUMNS || target_column < 0 || target_column >= NUM_COLUMNS) {
        printf("Invalid column indices.\n");
        return false;
    }

    pile *current = head;
    pile *source_pile = NULL;
    pile *target_pile = NULL;

    // find source pile
    while (current != NULL) {
        if (current->id == source_column) {
            source_pile = current;
            break;
        }
        current = current->next;
    }

    if (source_pile == NULL || source_pile->head == NULL) {
        printf("ERROR: Source empty\n");
        return false;
    }

    // find target pile
    current = head;
    while (current != NULL) {
        if (current->id == target_column) {
            target_pile = current;
            break;
        }
        current = current->next;
    }

    if (target_pile == NULL) {
        printf("ERROR: Target not found\n");
        return false;
    }

    // Prevent from moving to same pile
    if (source_pile == target_pile) {
        printf("Cannot move within same pile\n");
        return false;
    }

    // find the card in source
    node *source_card = source_pile->head;
    while (source_card != NULL) {
        if (source_card->assigned_card->value == value &&
            source_card->assigned_card->suit == suit) {
            break;
        }
        source_card = source_card->next;
    }

    if (source_card == NULL) {
        printf("ERROR: Card not found\n");
        return false;
    }

    // Prevent from moving hidden cards
    if (!source_card->assigned_card->cardVisible) {
        printf("Cannot move hidden card.\n");
        return false;
    }

    // Validate moving specific cards
    card *target_card = target_pile->tail ? target_pile->tail->assigned_card : NULL;

    if (!isValidMove(source_card->assigned_card, target_card)) {
        printf("Invalid move.\n");
        return false;
    }
    //Check that cards underneath are visible before moving
    node *temp = source_card;
    while (temp != NULL) {
        if (!temp->assigned_card->cardVisible) {
            printf("Cannot move hidden stack. \n");
            return false;
        }
        temp = temp->next;
    }

    //  SPLIT LIST
    node *before = source_card->prev;

    if (before != NULL) {
        before->next = NULL;
        source_pile->tail = before;
    } else {
        // moving entire pile
        source_pile->head = NULL;
        source_pile->tail = NULL;
    }

    source_card->prev = NULL;

    // APPEND TO TARGET
    if (target_pile->head == NULL) {
        // empty pile
        target_pile->head = source_card;
        target_pile->tail = source_card;
    } else {
        node *target_tail = target_pile->tail;

        target_tail->next = source_card;
        source_card->prev = target_tail;
    }

        // find new tail
        node *new_tail = source_card;
        while (new_tail->next != NULL) {
            new_tail = new_tail->next;
        }
        target_pile->tail = new_tail;

    // FLIP CARD
    if (source_pile->tail != NULL &&
        source_pile->tail->assigned_card->cardVisible == false) {
        source_pile->tail->assigned_card->cardVisible = true;
    }

    // UPDATE LENGTHS
    int count = 0;
    node *tmp = source_pile->head;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    source_pile->length = count;

    count = 0;
    tmp = target_pile->head;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    target_pile->length = count;

    return true;
}

// This function initializes the game piles with cards from the deck
pile *initializePiles(deck *Deck) {

    // Number of cards in each column (Yukon rules)
    int sizes[7] = {1, 6, 7, 8, 9, 10, 11};

    pile *head = NULL;          // pointer to first pile
    pile *prev_pile = NULL;     // pointer to previous pile

    card *current_card = Deck->head; // start from first card in deck

    // Loop through all columns (C1 to C7)
    for (int i = 0; i < NUM_COLUMNS; i++) {

        // Create a new pile
        pile *p = new_pile();
        p->id = i; // assign column id

        // Set head of pile list
        if (head == NULL) {
            head = p;
        }

        // Link piles together (double linked list)
        if (prev_pile != NULL) {
            prev_pile->next = p;
            p->prev = prev_pile;
        }

        prev_pile = p;

        // Add cards to this pile
        for (int j = 0; j < sizes[i]; j++) {

            // Create a node for current card
            node *n = new_node(current_card);

            // If pile is empty, set head and tail
            if (p->head == NULL) {
                p->head = n;
                p->tail = n;
            } else {
                // Otherwise, append to end
                p->tail->next = n;
                n->prev = p->tail;
                p->tail = n;
            }

            // Set visibility:
            // All cards except the last one are hidden
            if (j < sizes[i] - 1) {
                current_card->cardVisible = false;
            } else {
                current_card->cardVisible = true;
            }

            // Move to next card in deck
            current_card = current_card->next;

            // Increase pile length
            p->length++;
        }
    }

    return head; // return first pile
}

// Function to display the current state of the card piles and foundations
void displayCardPiles(pile *head_of_pile, pile *Foundations[]) {
// Initialize variables
    pile *current_pile = head_of_pile;
    int longest_pile = 0;
    int id_of_longest_pile = 0;
    int current_id = 0;
    int index = 0;
    int foundations = 0;
    node *current_card_in_pile[10];

// Iterate through each pile to determine the longest pile and its ID
    while(current_pile != NULL) {
        current_card_in_pile[index] = current_pile->head;
        if(longest_pile <= current_pile->length) {
            longest_pile = current_pile->length;
            id_of_longest_pile = current_pile->id;
        }
        index += 1;
        current_pile = current_pile->next;
    }

// If the longest pile is less than 8, set it to 8
    if(longest_pile < 8) {
        longest_pile = 8;
    }

// Display the column headers for the card piles
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    index = 0;

    card *current_card;

// Loop through each row of cards and display their values, or an empty space if there is no card
    while(longest_pile != 0) {
        if(current_card_in_pile[current_id] == NULL) {
            current_card = NULL;
        } else {
            current_card = current_card_in_pile[current_id]->assigned_card;
        }

        if(current_card != NULL && current_card->cardVisible) {
            printf("%c%c\t", current_card->value, current_card->suit);
        } else if(current_card != NULL && !current_card->cardVisible){
            printf("[]\t");
        } else {
            printf("\t");
        }

        // If the current card is the last card in a pile and it is a foundation pile, display the foundation
        if (current_id == 6) {
            if (foundations % 2 == 0 && index < 4) {
                if(Foundations[index]->tail != NULL) {
                    card *top = Foundations[index]->tail->assigned_card;
                    printf("\t\t%c%c\tF%d", top->value, top->suit, index+1);
                } else {
                    printf("\t\t[]\tF%d", index+1);
                }
                index++;
            }
            foundations++;
            puts("");
        }

        // Decrement the longest pile variable if the current card is the last card in the longest pile
        if(id_of_longest_pile == current_id) {
            longest_pile--;
        }

        // Move to the next card in the current pile
        if(current_card_in_pile[current_id] == NULL){
            current_card_in_pile[current_id] = NULL;
        } else {
            current_card_in_pile[current_id] = current_card_in_pile[current_id]->next;
        }
        // Move to the next pile if at the end of the current row, otherwise move to the next card in the row
        if(current_id == 6) {
            current_id = 0;
        } else {
            current_id += 1;
        }
    }
// Print a newline character to separate the output from other text
    puts("\n");
}
// This function saves the cards in a deck
void SaveDeckCards(deck *Deck) {

    FILE *stream = fopen(LOADFILE, "w");
    if (stream == NULL) {
        printf("Could not save deck.\n");
        return;
    }
    card *current = Deck->head;

    while (current != NULL) {
    fprintf(stream, "%c%c\n", current->value, current->suit);
    current = current->next;
    }
    fclose(stream);
}

// This function checks if all piles in the linked list have an empty head node, indicating that the game has been won.

bool checkWinState(pile *Foundation[]) {

    for (int i = 0; i < 4; i++) {
        if (Foundation[i]->tail == NULL|| Foundation[i]->tail->assigned_card->value != 'K')
            return false;
    }
    // All piles are empty, player has won
    return true;
}

//This our main that run our Game.
int main(int argc, char *argv[]) {
    pile *Foundation[4];
    for (int i = 0; i < 4; i++) {
        Foundation[i] = new_pile();
        Foundation[i]->id = i;
    }

    deck *Deck = new_deck();
    LD_default(Deck);

    printInitialSetup();
    pile *head_of_pile = NULL;

    char LastCommand[100] = "";
    char Message[100] = "";
    int sourceColumn, targetColumn;
    char cardValue, cardSuit;

    Phase current_phase = StartUp;

    while (1) {

        char input[50];

        printf("LAST Command: %s\n", LastCommand);
        printf("Message: %s\n", Message);
        printf("INPUT> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;


        strcpy(LastCommand, input);

        //  PLAY PHASE COMMANDS

        if (strcmp(input, "P") == 0) {

            if (current_phase == Play) {
                strcpy(Message, "Already in PLAY phase.");
            } else {
                head_of_pile = initializePiles(Deck);
                current_phase = Play;
                strcpy(Message, "OK");
            }

        } else if (strcmp(input, "Q") == 0) {

            current_phase = StartUp;

            // reset the foundations
            for (int i = 0; i < 4; i++) {
                Foundation[i]->head = NULL;
                Foundation[i]->tail = NULL;
                Foundation[i]->length = 0;
            }

            // reset the table
            head_of_pile = NULL;
            strcpy(Message, "OK");

        } else if (strcmp(input, "QQ") == 0) {

            printf("OK");
            break;

        }

        //  DECK COMMANDS

        else if (strcmp(input, "LD") == 0 || strncmp(input, "LD ", 3) == 0){

            if (current_phase == Play) {
                strcpy(Message, "Command not allowed in PLAY.");
            } else {
                char filename[100];
                deck *tempDeck;

                if (sscanf(input, "LD %s", filename) == 1) {
                    tempDeck = LD(filename); // loads file
                } else {
                    tempDeck = LD(NULL);
                }
                if (tempDeck == NULL) {
                    strcpy(Message, "Invalid file.");
                }else {
                    free_deck(Deck);
                    Deck = tempDeck;
                    strcpy(Message, "OK");
                }
            }
        }

        else if (strcmp(input, "SW") == 0) {

            if (current_phase == Play) {
                strcpy(Message, "Not allowed in PLAY.");
            } else {
                showAllCards(Deck);
                strcpy(Message, "OK");
            }

        } else if (strcmp(input, "SI") == 0 || strncmp(input, "SI ", 3) == 0) {

            if (current_phase == Play) {
                strcpy(Message, "Not allowed in PLAY.");
            } else {
                int split;

                // random split
                if (sscanf(input, "SI %d", &split) != 1) {
                    split = rand() % 51 + 1;
                }
                // validate split
                if (split < 1 || split > 51) {
                    strcpy(Message, "Invalid split.");
                    continue;
                }
                deck *oldDeck = Deck;
                Deck = splitShuffle(Deck, split);
                free_deck(oldDeck);

                print_cards_in_deck(Deck);
                strcpy(Message, "OK");
            }

        } else if (strcmp(input, "SR") == 0) {

            if (current_phase == Play) {
                strcpy(Message, "Not allowed in PLAY.");
            } else {
                Deck = randomshuffle_deck(Deck);
                showAllCards(Deck);
                strcpy(Message, "OK");
            }

        } else if (strcmp(input, "SD") == 0) {

            if (current_phase == StartUp) {
                SaveDeckCards(Deck);
                strcpy(Message, "OK");
            } else {
                strcpy(Message, "Not allowed.");
            }
        }

        // MOVE COMMANDS

        else if (strstr(input, "->")) {

            if (current_phase != Play) {
                strcpy(Message, "Not allowed in STARTUP.");
                continue;
            }

            int s, t, f;
            char v, suit;

            // move specific card
            if (sscanf(input, "C%d:%c%c->C%d", &s, &v, &suit, &t) ==4) {

                sourceColumn = s - 1;
                targetColumn = t - 1;

                if (sourceColumn < 0 || sourceColumn >= NUM_COLUMNS|| targetColumn < 0 || targetColumn >= NUM_COLUMNS) {
                    strcpy(Message, "Invalid column.");
                    continue;
                }

                if (moveSpecificCard(head_of_pile, sourceColumn, targetColumn, v, suit)) {
                    printf("Moved from %c%c from C%d to C%d\n", v, suit, s, t);
                    strcpy(Message, "OK");
                }else {
                    strcpy(Message, "Invalid card.");
                }
            }

            //  card move from column to column
            else if (sscanf(input, "C%d->C%d", &s, &t) == 2) {
                sourceColumn = s - 1;
                targetColumn = t - 1;
                if (sourceColumn < 0 || sourceColumn >= NUM_COLUMNS || targetColumn < 0 || targetColumn >= NUM_COLUMNS) {
                    strcpy(Message, "Invalid column.");
                    continue;
                }

                if (moveCard(head_of_pile, sourceColumn, targetColumn)) {
                    printf("Moved from C%d to C%d\n", s, t);
                        strcpy(Message, "OK");
                } else {
                    strcpy(Message, "Invalid move.");
                }
            }
            // card move from column to foundation
            else if (sscanf(input, "C%d->F%d", &s, &f) == 2) {
                sourceColumn = s - 1;
                int foundationIndex = f - 1;

                if (sourceColumn < 0 || sourceColumn >= NUM_COLUMNS || foundationIndex < 0 || foundationIndex >= 4) {
                    strcpy(Message, "Invalid move.");
                    continue;
                }

                if (moveCardToFoundation(head_of_pile, sourceColumn, foundationIndex, Foundation)) {
                    printf("Moved from C%d to F%d\n", s, f);
                    strcpy(Message, "OK");
                } else {
                    strcpy(Message, "Invalid move.");
                }
            }

            // card move from foundation to column
            else if (sscanf(input, "F%d->C%d", &f, &t) == 2) {

                int foundationIndex = f - 1;
                targetColumn = t - 1;
                if (foundationIndex < 0 || foundationIndex >= 4 || targetColumn < 0 || targetColumn >= NUM_COLUMNS) {
                    strcpy(Message, "Invalid move.");
                    continue;
                }
                if (Foundation[foundationIndex] != NULL) {

                    if (Foundation[foundationIndex]->tail == NULL) {
                        strcpy(Message, "Foundation empty.");
                        continue;
                    }

                    node *foundationTop = Foundation[foundationIndex]->tail;
                    card *movingcard = foundationTop->assigned_card;

                    pile *targetPile = head_of_pile;
                    for (int i = 0; i < targetColumn; i++) {
                        targetPile = targetPile->next;
                    }
                    card *targetCard = targetPile->tail ? targetPile->tail->assigned_card : NULL;
                    if (!isValidMove(movingcard, targetCard)) {
                        strcpy(Message, "Invalid move.");
                    }else {

                        if (foundationTop->prev == NULL) {
                            Foundation[foundationIndex]->head = NULL;
                            Foundation[foundationIndex]->tail = NULL;
                        }else {
                            Foundation[foundationIndex]->tail=foundationTop->prev;
                            Foundation[foundationIndex]->tail->next=NULL;
                        }
                        Foundation[foundationIndex]->length--;

                        foundationTop->prev = NULL;
                        foundationTop->next = NULL;

                        if (targetPile->tail != NULL) {
                            targetPile->tail->next = foundationTop;
                            foundationTop->prev = targetPile->tail;
                            targetPile->tail = foundationTop;
                        }else {

                            targetPile->head = foundationTop;
                            targetPile->tail = foundationTop;
                        }
                        targetPile->length++;
                        printf("Moved from F%d to C%d\n", f, t);
                        strcpy(Message, "OK");
                    }
                }else {
                    strcpy(Message, "Foundation empty.");
                }
            }
            else {
                strcpy(Message, "Invalid move format");
            }
        }

        else {
            strcpy(Message, "Invalid command.");
        }
        //Check for win
        if (current_phase == Play && checkWinState(Foundation)) {
            strcpy(Message, "You have won the game!");
            printf("Winner!\n");
            break;
        }

        // Display of the game
        if (current_phase == Play) {
            displayCardPiles(head_of_pile, Foundation);
        }
    }
    free_deck(Deck);
    return 0;
}