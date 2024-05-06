#include "main.h"
#include <stdio.h>
#include <string.h>


/******** Move card to foundation *********/
//Function to move the correct pile of cards into the foundation pile
card *moveCardToFoundation(pile *head, int source_column) {
    // Check that source_column and target_column are valid indices

    if (source_column < 0 || source_column >= NUM_COLUMNS) {
        updateMessage(Message,"Invalid column indices.");
        return NULL;
    }

    // Check that the source column is not empty
    if (head == NULL) {
        updateMessage(Message,"Source column is empty.");
        return NULL;
    }

    pile *current = head;

    while (current->id != source_column) {
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

/*****Move a card from one column to another*****/
void moveCard(pile *head, int source_column, int target_column) {
    // Check that source_column and target_column are valid indices
    if (source_column < 0 || source_column >= NUM_COLUMNS || target_column < 0 || target_column >= NUM_COLUMNS) {
        updateMessage(Message, "Invalid column indices.");
        return;
    }

    // Finding source and target piles
    pile *source_pile = NULL;
    pile *target_pile = NULL;
    for (pile *current = head; current != NULL; current = current->next) {
        if (current->id == source_column) {
            source_pile = current;
        }
        if (current->id == target_column) {
            target_pile = current;
        }
        if (source_pile && target_pile) {
            break; // Found both piles, no need to continue
        }
    }

    // Check if the source pile was found and is not empty
    if (!source_pile || !source_pile->head) {
        updateMessage(Message, "Source column is empty or does not exist.");
        return;
    }

    // Check if the target pile was found (even if it's empty, it should exist)
    if (!target_pile) {
        updateMessage(Message, "Target column does not exist.");
        return;
    }

    // If checks are passed, proceed with moving the card
    node *source_card = source_pile->tail; // Assuming tail is the last card
    if (!source_card) {
        updateMessage(Message, "No cards in source column to move.");
        return;
    }

    // Remove the card from the source column
    if (source_card->prev) {
        source_card->prev->next = NULL;
        source_pile->tail = source_card->prev;
    } else {
        source_pile->head = NULL;
        source_pile->tail = NULL;
    }

    // Add the card to the target column
    if (target_pile->head == NULL) {
        target_pile->head = source_card;
        target_pile->tail = source_card;
        source_card->prev = NULL;
    } else {
        target_pile->tail->next = source_card;
        source_card->prev = target_pile->tail;
        target_pile->tail = source_card;
    }
    source_card->next = NULL;  // Make sure to nullify the next pointer of the moved card

    updateMessage(Message, "OK!");
}


/***Move a specific card from one column to another***/

void moveSpecificCard(pile *head, int source_column, int target_column, char value, char suit) {
    if (source_column < 0 || source_column >= NUM_COLUMNS || target_column < 0 || target_column >= NUM_COLUMNS) {
        updateMessage(Message,"Invalid column indices.");
        return;
    }

    //Find the source and target piles
    pile *source_pile = NULL, *target_pile = NULL;
    for (pile *current = head; current != NULL; current = current->next) {
        if (current->id == source_column) {
            source_pile = current;
        }
        if (current->id == target_column) {
            target_pile = current;
        }
        if (source_pile && target_pile) {
            break; // Exit the loop early if both piles are found
        }
    }

    if (!source_pile || !source_pile->head) {
        updateMessage(Message,"Source column is empty or not found.");
        return;
    }

    if (!target_pile) {
        updateMessage(Message,"Target column not found.");
        return;
    }

    node *source_card = source_pile->head;
    node *prev_card = NULL;
    while (source_card && !(source_card->assigned_card->value == value && source_card->assigned_card->suit == suit)) {
        prev_card = source_card;
        source_card = source_card->next;
    }

    if (!source_card) {
        updateMessage(Message,"Card not found in source column.");
        return;
    }


    // Detach source card from source column
    if (source_card->prev) {
        source_card->prev->next = NULL; //previous card points to null
        source_card->prev = source_pile->tail; //update tail
    } else {
        source_pile->head = NULL;
        source_pile->tail = NULL;
    }

    // Attach source card and following cards to target column
    if (!target_pile->head) {
        target_pile->head = source_card;
        source_card->prev = NULL; // set prev of the new head to NULL
    } else {
        target_pile->tail->next = source_card;
        source_card->prev = target_pile->tail;
    }

    // Find the last card in the target pile
    node *last_card_target = source_card;
    while (last_card_target->next != NULL) {
        last_card_target = last_card_target->next;
    }

// Update tail pointer of the target pile to point to the last card
    target_pile->tail = last_card_target;


    updateMessage(Message,"OK!");
}



/****checks the if player has won the game***/
// This function checks if all piles in the linked list have an empty head node, indicating that the game has been won.
bool checkWinState(pile *head) {
// Start at the beginning of the linked list.
    pile *current = head;

// Iterate through each pile in the linked list.
    while (current != NULL) {
        // If the head node of the current pile is not empty,the game has not been won, so return false.
        if (current->head != NULL) {
            return false;
        }
        // Move to the next pile in the linked list.
        current = current->next;
    }

// If we reach the end of the linked list without finding a non-empty head node, the game has been won, so return true.
    return true;
}