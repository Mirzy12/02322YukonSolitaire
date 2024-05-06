//
// Created by aljwa on 07-04-2024.
//

#ifndef SOLITAIRE_LOADGAME_H
#define SOLITAIRE_LOADGAME_H
#include "main.h"
void LD_file(deck*, char*);
deck* LD(char* input);
void LD_file(deck* deck, char* input);
void SaveDeckCards(pile *head_of_pile);
#endif //SOLITAIRE_LOADGAME_H
