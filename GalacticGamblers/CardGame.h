#pragma once
#include <string>
#include <vector>

//  .-------.
// /  cards  \
// '-------'
struct Card {
    int value;      // 1-13 (Ace-King)
    char suit;      // '^', '&', 'o', 'v'
};

enum HandType {
    HIGH_CARD, PAIR, TWO_PAIR, THREE_KIND, STRAIGHT,
    FLUSH, FULL_HOUSE, FOUR_KIND, STRAIGHT_FLUSH
};

void createDeck();
void shuffleDeck();
void dealHand();
void drawCards();
void showHandGuide();
void animatePlayHand();
void playAsciiAnimation();
HandType evaluateHand(const std::vector<Card>& hand);
std::string handName(HandType type);
void performDiscard();

// Shared poker state from CardGame.cpp
extern bool selected[8];
extern bool discarded[8];
extern int position;
extern int discardsRemaining;
extern std::vector<Card> hand;
