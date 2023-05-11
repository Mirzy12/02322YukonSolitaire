#include "main.h"
#include "LD.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>



// Define number of suits, values and columns using preprocessor directives
#define NUM_SUITS 4
#define NUM_VALUES 13
#define LOADFILE "../cards.txt"
#define NUM_COLUMNS 7

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


// Function to free a deck of cards
void free_deck(struct deck *deck) {
    struct card *current = deck->head;
    while (current != NULL) {
        struct card *next = current->next;
        free(current);
        current = next;
    }
    free(deck);
}

struct deck* splitShuffle(struct deck* deck) {
    // Allocate memory for the shuffled deck
    struct deck* shuffledDeck = malloc(sizeof(struct deck));
    shuffledDeck->head = NULL;
    shuffledDeck->tail = NULL;
    shuffledDeck->size = 0;

    // Split the deck into two halves
    struct card* firstHalf = deck->head;
    struct card* secondHalf = NULL;
    struct card* current = deck->head;

    int i;
    for (i = 0; i < deck->size / 2; i++) {
        current = current->next;
    }
    secondHalf = current;

    // Interleave the cards from the two halves
    for (i = 0; i < deck->size / 2; i++) {
        // Take the i-th card from the first half
        struct card* firstCard = firstHalf;
        int j;
        for (j = 0; j < i; j++) {
            firstCard = firstCard->next;
        }

        // Take the i-th card from the second half
        struct card* secondCard = secondHalf;
        for (j = 0; j < i; j++) {
            secondCard = secondCard->next;
        }

        // Add the two cards to the shuffled deck
        struct card* newCard1 = new_card(firstCard->suit, firstCard->value);
        attachCardToDeck(shuffledDeck, newCard1);

        struct card* newCard2 = new_card(secondCard->suit, secondCard->value);
        attachCardToDeck(shuffledDeck, newCard2);
    }

    return shuffledDeck;
}


card *getCardAtIndex(deck *deck, int index) {
    if (index < 0 || index >= deck->size) {
        return NULL;
    }

    card *current = deck->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}

struct deck *randomshuffle_deck(struct deck *deck) {
    int i, j;
    srand(time(NULL));
    for (i = deck->size - 1; i > 0; i--) {
        j = rand() % (i + 1);

        //Using the "Fisher and Yates" method to randomize our deck
        struct card *current_i = deck->head;
        int k;
        for (k = 0; k < i; k++) {
            current_i = current_i->next;
        }
        struct card *card_i = current_i;

        struct card *current_j = deck->head;
        for (k = 0; k < j; k++) {
            current_j = current_j->next;
        }
        struct card *card_j = current_j;

        //Here we swap the cards
        char temp_suit = card_i->suit;
        card_i->suit = card_j->suit;
        card_j->suit = temp_suit;

        char temp_value = card_i->value;
        card_i->value = card_j->value;
        card_j->value = temp_value;
    }
    return deck;
}

card *moveCardToFoundation(pile *head, int source_column) {
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
    while (source_tail->next != NULL) {
        source_tail = source_tail->next;
    }

    // Remove the card from the source column
    if (source_tail->prev == NULL) {
        // The source column only had one card
        current->head = NULL;
        current->tail = NULL;
    } else {
        source_tail->prev->next = NULL;
        source_tail->prev = NULL;
    }

    return source_tail->assigned_card;
}

void moveCard(pile *head, int source_column, int target_column) {
    // Check that source_column and target_column are valid indices
    if (source_column < 0 || source_column >= NUM_COLUMNS || target_column < 0 || target_column >= NUM_COLUMNS) {
        printf("Invalid column indices.\n");
        return;
    }

    // Check that the source column is not empty
    if (head == NULL) {
        printf("Source column is empty.\n");
        return;
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
    while (source_card->next != NULL) {
        source_card = source_card->next;
    }

    // Remove the card from the source column
    if (source_card->prev == NULL) {
        // The source column only had one card
        current->head = NULL;
        current->tail = NULL;
    } else {
        source_card->prev->next = NULL;
        source_card->prev = NULL;
    }

    current = head;
    while(current->id != target_column) {
        current = current->next;
    }

    target_pile = current;

    // Add the card to the end of the target column
    if (target_pile->head == NULL && target_pile->tail == NULL) {
        // The target column is empty
        target_pile->head = source_card;
        target_pile->tail = source_card;
        source_card->prev = NULL;
    } else {
        // Find the last card in the target column
        node *target_tail = target_pile->tail;
        while (target_tail->next != NULL) {
            target_tail = target_tail->next;
        }

        // Add the card to the end of the target column
        target_tail->next = source_card;
        source_card->prev = target_tail;
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
}

void moveSpecificCard(pile *head, int source_column, int target_column, char value, char suit) {
    // Check that source_column and target_column are valid indices
    if (source_column < 0 || source_column >= NUM_COLUMNS || target_column < 0 || target_column >= NUM_COLUMNS) {
        printf("Invalid column indices.\n");
        return;
    }

    // Check that the source column is not empty
    if (head == NULL) {
        printf("Source column is empty.\n");
        return;
    }

    pile *current = head;
    pile *source_pile;
    pile *target_pile;

    while(current->id != source_column) {
        current = current->next;
    }
    source_pile = current;

    // Find the last card in the source column
    node *source_card = current->head;
    while(source_card != NULL) {
        if(source_card->assigned_card->value == value && source_card->assigned_card->suit == suit) {
            break;
        }
        source_card = source_card->next;
    }

    if(source_card == NULL) {
        printf("ERROR\n");
        return;
    }

    // Remove the card from the source column
    if (source_card->prev == NULL) {
        // The source column only had one card
        current->head = NULL;
        current->tail = NULL;
    } else {
        source_card->prev->next = NULL;
        source_card->prev = NULL;
    }

    current = head;
    while(current->id != target_column) {
        current = current->next;
    }

    target_pile = current;

    // Add the card to the end of the target column
    if (target_pile->head == NULL && target_pile->tail == NULL) {
        // The target column is empty
        target_pile->head = source_card;
        target_pile->tail = source_card;
        source_card->prev = NULL;
    } else {
        // Find the last card in the target column
        node *target_tail = target_pile->tail;
        while (target_tail->next != NULL) {
            target_tail = target_tail->next;
        }

        // Add the card to the end of the target column
        target_tail->next = source_card;
        source_card->prev = target_tail;
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
}

//This function initializes the game piles with cards from the deck
pile *initializePiles(deck *Deck) {
    int temp_length;
    pile *current_pile = new_pile(); // create a new pile
    pile *head_of_pile = current_pile; // initialize the head of the pile with the current pile
    node *current_node;

    card *current_card = Deck->head; // initialize the current card to the first card of the deck

    //Loop through NUM_COLUMNS (7) times to fill in each pile with cards
    for(int i = 0; i < NUM_COLUMNS; i++) {
        current_pile->id = i; // assign a unique id to each pile
        current_pile->head = new_node(current_card); // create a new node for each card
        if(i != 0) {
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
    while(current_card != NULL) {
        // If the current pile is the first pile and has only 1 card or if
        // it has reached the maximum number of cards allowed, move to next pile
        if(current_pile->id == 0) {
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

        // Add the current card to the current pile
        current_node = new_node(current_card); // create a new node for the current card
        current_node->prev = current_pile->tail; // set the previous pointer of the new node to the tail of the current pile
        current_pile->tail->next = current_node; // set the next pointer of the tail of the current pile to the new node
        current_pile->tail = current_node; // set the tail of the current pile to the new node
        temp_length = current_pile->length; // increment the length of the current pile
        current_pile->length = ++temp_length;

        // Set visibility of card based on game rules
        if((current_pile->id == 1 && current_pile->length <= 1) ||
           (current_pile->id == 2 && current_pile->length <= 2) ||
           (current_pile->id == 3 && current_pile->length <= 3) ||
           (current_pile->id == 4 && current_pile->length <= 4) ||
           (current_pile->id == 5 && current_pile->length <= 5) ||
           (current_pile->id == 6 && current_pile->length <= 6)){
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
void displayCardPiles(pile *head_of_pile, card *Foundations[]) {
    pile *current_pile = head_of_pile;

    int longest_pile = 0;
    int id_of_longest_pile = 0;
    int current_id = 0;
    int index = 0;
    int foundations = 0;
    node *current_card_in_pile[10];


    while(current_pile != NULL) {
        current_card_in_pile[index] = current_pile->head;
        if(longest_pile <= current_pile->length) {
            longest_pile = current_pile->length;
            id_of_longest_pile = current_pile->id;
        }
        index += 1;
        current_pile = current_pile->next;
    }

    if(longest_pile < 8) {
        longest_pile = 8;
    }

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    index = 0;

    card *current_card;

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

        if (current_id == 6) {
            if (foundations % 2 == 0 && index < 4) {
                if(Foundations[index] != NULL) {
                    printf("\t\t%c%c\tF%d", Foundations[index]->value, Foundations[index]->suit, index+1);
                } else {
                    printf("\t\t[]\tF%d", index+1);
                }
                index++;
            }
            foundations++;
            puts("");
        }

        if(id_of_longest_pile == current_id) {
            longest_pile--;
        }

        if(current_card_in_pile[current_id] == NULL){
            current_card_in_pile[current_id] = NULL;
        } else {
            current_card_in_pile[current_id] = current_card_in_pile[current_id]->next;
        }
        if(current_id == 6) {
            current_id = 0;
        } else {
            current_id += 1;
        }
    }
    puts("\n");
}

void SaveDeckCards(pile *head_of_pile) {
    pile *current_pile = head_of_pile;

    int longest_pile = 0;
    int id_of_longest_pile = 0;
    int current_id = 0;
    int index = 0;
    node *current_card_in_pile[10];


    while(current_pile != NULL) {
        current_card_in_pile[index] = current_pile->head;
        if(longest_pile <= current_pile->length) {
            longest_pile = current_pile->length;
            id_of_longest_pile = current_pile->id;
        }
        index += 1;
        current_pile = current_pile->next;
    }

    if(longest_pile < 8) {
        longest_pile = 8;
    }

    card *current_card;
    FILE *stream;
    stream = fopen(LOADFILE, "w");
    fclose(stream);
    stream = fopen(LOADFILE, "a");//a
    char newData[4];
    bool notFirstLine = false;

    while(longest_pile != 0) {
        if(current_card_in_pile[current_id] == NULL) {
            current_card = NULL;
        } else {
            current_card = current_card_in_pile[current_id]->assigned_card;
        }

        if(current_card != NULL) {
            newData[0] = current_card->value;
            newData[1] = current_card->suit;
            if(notFirstLine) {
                fprintf(stream, "\n");
            }
            notFirstLine = true;
            fwrite(newData, sizeof(char), strlen(newData), stream);
        }

        if(id_of_longest_pile == current_id) {
            longest_pile--;
        }

        if(current_card_in_pile[current_id] == NULL){
            current_card_in_pile[current_id] = NULL;
        } else {
            current_card_in_pile[current_id] = current_card_in_pile[current_id]->next;
        }
        if(current_id == 6) {
            current_id = 0;
        } else {
            current_id += 1;
        }
    }
    fclose(stream);
}

bool checkWinState(pile *head) {
    pile *current = head;
    while(current != NULL) {
        if(current->head != NULL) {
            return false;
        }
        current = current->next;
    }
    return true;
}

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
    char Message[100] = "";
    int sourceColumn;
    int targetColumn;
    char cardValue;
    char cardSuit;

    //While loop of the main game, commands, messages and last commands saved
    while (1) {
        char input[50];
        printf("LAST Command: %s\n", LastCommand);
        printf("Message: %s\n", Message);
        printf("INPUT> ");
        scanf("%s", input);

        strcpy(LastCommand, input);

        if (strcmp(input, " ") == 0) {

        } else if (strcmp(input, "P") == 0) {
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
        } else if(strstr(input, ":") != 0){
            cardValue = input[0];
            cardSuit = input[1];
            sourceColumn = input[4] - '0';
            targetColumn = input[8] - '0';
            sourceColumn = sourceColumn - 1;
            targetColumn = targetColumn - 1;
            moveSpecificCard(head_of_pile, sourceColumn, targetColumn, cardValue, cardSuit);
        } else if(strstr(input, "->") != 0) {
            sourceColumn = input[1] - '0';
            targetColumn = input[5] - '0';

            sourceColumn = sourceColumn - 1;
            targetColumn = targetColumn - 1;

            if(strstr(input, "F1") != 0){
                Foundation[0] = moveCardToFoundation(head_of_pile, sourceColumn);
            } else if(strstr(input, "F2") != 0) {
                Foundation[1] = moveCardToFoundation(head_of_pile, sourceColumn);
            } else if(strstr(input, "F3") != 0) {
                Foundation[2] = moveCardToFoundation(head_of_pile, sourceColumn);
            } else if(strstr(input, "F4") != 0) {
                Foundation[3] = moveCardToFoundation(head_of_pile, sourceColumn);
            } else {
                moveCard(head_of_pile, sourceColumn, targetColumn);
            }

            if(checkWinState(head_of_pile)) {
                printf("Winner Winner, Chicken Dinner!\n");
                break;
            }
        } else if (strcmp(input, "QQ") == 0) {
            printf("BYE BYE BYE!\n");
            break;
        } else if(strcmp(input, "W") == 0) {
            printf("Winner Winner, Chicken Dinner!\n");
            break;
        }

    }


    return 0;
}
