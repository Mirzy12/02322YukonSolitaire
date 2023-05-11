
#include "main.h"


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