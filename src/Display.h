//
// Created by aljwa on 07-04-2024.
//

#ifndef SOLITAIRE_DISPLAY_H
#define SOLITAIRE_DISPLAY_H
void showAllCards(struct deck *deck);
void print_cards_in_deck(struct deck *deck);
void printInitalSetup();
pile *initializePiles(deck *Deck);
void displayCardPiles(pile *head_of_pile, card *Foundations[]);

#endif //SOLITAIRE_DISPLAY_H
