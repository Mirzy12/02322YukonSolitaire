//
// Created by aljwa on 07-04-2024.
//

#ifndef SOLITAIRE_GAMEPLAY_H
#define SOLITAIRE_GAMEPLAY_H
void moveCard(pile *head, int source_column, int target_column);
void moveSpecificCard(pile *head, int source_column, int target_column, char value, char suit);
card *moveCardToFoundation(pile *head, int source_column);
bool checkWinState(pile *head);
#endif //SOLITAIRE_GAMEPLAY_H
