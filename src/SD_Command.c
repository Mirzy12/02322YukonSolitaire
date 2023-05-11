#include "main.h"
void save_deck(deck *deck, char *filename) {
    FILE *fp;
    fp = fopen(filename, "w");

    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    card *current = deck->head;

    while (current != NULL) {
        fprintf(fp, "%c%c\n", current->value, current->suit);
        current = current->next;
    }

    fclose(fp);
}
