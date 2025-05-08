
#include "main.h"
#include "LD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// To ensure that the LD_file is known
void LD_file(deck* deck, char* input);

deck* LD(char* input) {
    deck *deck = new_deck();

    // Use default, if there's no input
    if (input == NULL || strlen(input) == 0) {
        printf("🔍 Using default path: ../cards.txt\n");

        FILE *test = fopen("../cards.txt", "r");
        if (test == NULL) {
            perror("Failed to open ../cards.txt");
            free(deck);
            return NULL;
        }
        fclose(test);
        LD_file(deck, "../cards.txt");
    } else {
        FILE *test = fopen(input, "r");
        if (test == NULL) {
            perror("Failed to open provided file");
            free(deck);
            return NULL;
        }
        fclose(test);
        LD_file(deck, input);
    }

    return deck;
}

void LD_file(deck* deck, char* input) {
    FILE *fp = fopen(input, "r");
    if (fp == NULL) {
        perror("fopen failed");
        return;
    }

    char str[52];
    int line = 0;

    while (fgets(str, sizeof(str), fp)) {
        line++;
        char value = str[0];
        char suit = str[1];

        if (!((value >= '2' && value <= '9') || value == 'T' || value == 'A' || value == 'K' || value == 'Q' || value == 'J')) {
            printf("Invalid card value on line %d: %s\n", line, str);
            continue;
        }

        if (!(suit == 'C' || suit == 'D' || suit == 'S' || suit == 'H')) {
            printf("Invalid card suit on line %d: %s\n", line, str);
            continue;
        }

        card *new = new_card(suit, value);
        new->cardVisible = false;
        attachCardToDeck(deck, new);
    }

    fclose(fp);
    printf("Successfully loaded deck with %d cards.\n", deck->size);
}
