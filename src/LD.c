
#include "main.h"
#include "LD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// To ensure that the LD_file is known
void LD_file(deck* deck, char* input);

void free_deck(deck* deck) {
    card *current = deck->head;
    while (current != NULL) {
        card *next = current->next;
        free(current);
        current = next;
    }
    free(deck);
}

deck* LD(char* input) {
    deck *deck = new_deck();

    // Use default, if there's no input
    if (input == NULL || strlen(input) == 0) {
        printf("🔍 Using default path: %s\n", LOADFILE);

        FILE *test = fopen(LOADFILE, "r");
        if (test == NULL) {
            perror("Failed to open default file");
            free_deck(deck);
            return NULL;
        }
        fclose(test);

        LD_file(deck, LOADFILE);
    } else {
        FILE *test = fopen(input, "r");
        if (test == NULL) {
            perror("Failed to open provided file");
            free_deck(deck);
            return NULL;
        }
        fclose(test);
        LD_file(deck, input);
    }
    //check that deck is equal to 52 cards
    if (deck->size !=52) {
        printf("Error: deck must contain exactly 52 valid cards.\n");
        free_deck(deck);
        return NULL;
    }

    return deck;
}

void LD_file(deck* deck, char* input) {
    FILE *fp = fopen(input, "r");
    if (fp == NULL) {
        perror("fopen failed");
        return;
    }

    char str[16];
    int line = 0;

    //track duplicates
    bool seen[13][4] = {false};

    while (fgets(str, sizeof(str), fp)) {
        line++;
        char value = str[0];
        char suit = str[1];

        //check for value
        if (!((value >= '2' && value <= '9') || value == 'T' || value == 'A' || value == 'K' || value == 'Q' || value == 'J')) {
            printf("Invalid card value on line %d: %s\n", line, str);
            fclose(fp);
            return;
        }

        //check for suit
        if (!(suit == 'C' || suit == 'D' || suit == 'S' || suit == 'H')) {
            printf("Invalid card suit on line %d: %s\n", line, str);
            fclose(fp);
            return;
        }

        //find index
        int v= -1, s = -1;

        for (int i = 0; i < 13; i++) {
            if (order[i] == value) v = i;
        }

        if (suit == 'C') s = 0;
        else if (suit == 'D') s = 1;
        else if (suit == 'H') s = 2;
        else if (suit == 'S') s = 3;

        if (v == -1 || s == -1) {
            printf("Invalid card on line %d: %s\n", line, str);
            fclose(fp);
            return;
        }
        // check for duplicates
        if (seen[v][s]) {
            printf("Duplicate card found: %c%c\n", value, suit);
            fclose(fp);
            return;
        }

        seen[v][s] = true;
        // Add card
        card *new = new_card(suit, value);
        new->cardVisible = false;
        attachCardToDeck(deck, new);
    }

    fclose(fp);
    printf("Successfully loaded deck with %d cards.\n", deck->size);
}
