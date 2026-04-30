#include <algorithm>
#include <conio.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <windows.h>
#include "CardGame.h"
using namespace std;

// --- GLOBAL VARIABLES ---
vector<Card> deck;
vector<Card> hand;

bool selected[8] = { false };
bool discarded[8] = { false };

int position = 0;
int discardsRemaining = 2;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

constexpr int PLAY_AREA_LEFT = 2;
constexpr int PLAY_AREA_TOP = 4;
constexpr int PLAY_AREA_WIDTH = 86;
constexpr int PLAY_AREA_HEIGHT = 12;
constexpr int CARD_BASE_Y = 8;

// --- UTILITY FUNCTIONS ---
void moveCursor(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOut, pos);
}

// Returns string name for card value
string rankName(int v)
{
    switch (v)
    {
    case 1: return "A";
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    default: return to_string(v);
    }
}

// Hand name
string handName(HandType h)
{
    switch (h)
    {
    case STRAIGHT_FLUSH: return "Straight Flush";
    case FOUR_KIND: return "Four of a Kind";
    case FULL_HOUSE: return "Full House";
    case FLUSH: return "Flush";
    case STRAIGHT: return "Straight";
    case THREE_KIND: return "Three of a Kind";
    case TWO_PAIR: return "Two Pair";
    case PAIR: return "Pair";
    default: return "High Card";
    }
}

// --- DECK MANAGEMENT ---
void createDeck()
{
    deck.clear();
    char suits[] = { '^', '&', 'o', 'v' };
    for (char s : suits)
        for (int v = 1; v <= 13; v++)
            deck.push_back({ v, s });
}

void shuffleDeck()
{
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

void dealHand()
{
    hand.clear();
    for (int i = 0; i < 8; i++)
        hand.push_back(deck.back()), deck.pop_back();
}

// --- DRAWING ---
void drawCard(int x, int y, Card c)
{
    moveCursor(x, y);
    cout << " _____ ";

    moveCursor(x, y + 1);
    cout << "|";

    string r = rankName(c.value);
    if (r.length() == 1)
        cout << r << "    |";
    else
        cout << r << "   |";

    moveCursor(x, y + 2);
    cout << "|  " << c.suit << "  |";

    moveCursor(x, y + 3);
    cout << "|_____|";
}

void drawPlayAreaBorder()
{
    moveCursor(PLAY_AREA_LEFT, PLAY_AREA_TOP);
    cout << "+" << string(PLAY_AREA_WIDTH - 2, '-') << "+";

    for (int y = PLAY_AREA_TOP + 1; y < PLAY_AREA_TOP + PLAY_AREA_HEIGHT - 1; y++)
    {
        moveCursor(PLAY_AREA_LEFT, y);
        cout << "|";

        moveCursor(PLAY_AREA_LEFT + PLAY_AREA_WIDTH - 1, y);
        cout << "|";
    }

    moveCursor(PLAY_AREA_LEFT, PLAY_AREA_TOP + PLAY_AREA_HEIGHT - 1);
    cout << "+" << string(PLAY_AREA_WIDTH - 2, '-') << "+";
}

void drawCards()
{
    system("cls");

    // Keep controls slightly below top and pinned each frame.
    moveCursor(0, 1);
    cout << "Left/Right = move | Up = select (max 5) | Down = mark discard | D = redraw | H = hand guide | Enter = play";

    moveCursor(0, 2);
    cout << "Discards remaining: " << discardsRemaining;

    drawPlayAreaBorder();

    for (int i = 0; i < 8; i++)
    {
        int x = 6 + i * 10;
        int y = CARD_BASE_Y;

        if (selected[i])
            y -= 2;
        else if (discarded[i])
            y += 2;
        else if (position == i)
            y -= 1;

        drawCard(x, y, hand[i]);
    }

    // Keep following output below the border.
    moveCursor(0, PLAY_AREA_TOP + PLAY_AREA_HEIGHT + 1);
}

void showHandGuide()
{
    system("cls");
    cout << "========== POKER HAND GUIDE ==========\n";
    cout << "[A^][9&][7o][4v][2^]  High Card\n";
    cout << "[K^][K&][9o][6v][3^]  Pair\n";
    cout << "[Q^][Q&][8o][8v][2^]  Two Pair\n";
    cout << "[J^][J&][Jv][7o][2^]  Three of a Kind\n";
    cout << "[5^][6&][7o][8v][9^]  Straight\n";
    cout << "[2^][5^][8^][J^][K^]  Flush\n";
    cout << "[10^][10&][10o][4v][4^] Full House\n";
    cout << "[9^][9&][9o][9v][2^]  Four of a Kind\n";
    cout << "[6^][7^][8^][9^][10^] Straight Flush\n";
    cout << "======================================\n";
    cout << "Press any key to return...\n";
    _getch();
}

void drawAnimationFrame(const vector<int>& yOffsets)
{
    system("cls");
    cout << "Playing hand...\n\n";

    drawPlayAreaBorder();

    for (int i = 0; i < 8; i++)
    {
        int x = 6 + i * 10;
        int y = CARD_BASE_Y + yOffsets[i];
        drawCard(x, y, hand[i]);
    }

    moveCursor(0, PLAY_AREA_TOP + PLAY_AREA_HEIGHT + 2);
}

void animatePlayHand()
{
    vector<int> offsets(8, 0);

    for (int i = 0; i < 8; i++)
    {
        if (selected[i])
            offsets[i] = -3;
    }

    drawAnimationFrame(offsets);
    Sleep(120);

    for (int step = 0; step < 3; step++)
    {
        for (int i = 0; i < 8; i++)
        {
            if (selected[i])
                offsets[i] += 1;
        }
        drawAnimationFrame(offsets);
        Sleep(70);
    }

    for (int i = 0; i < 8; i++)
    {
        if (selected[i])
            offsets[i] = 1;
    }
    drawAnimationFrame(offsets);
    Sleep(50);

    for (int i = 0; i < 8; i++)
    {
        if (selected[i])
            offsets[i] = 0;
    }
    drawAnimationFrame(offsets);
    Sleep(90);
}

// --- DISCARD ---
void performDiscard()
{
    if (discardsRemaining <= 0)
        return;

    for (int i = 0; i < 8; i++)
    {
        if (discarded[i])
        {
            hand[i] = deck.back();
            deck.pop_back();
            discarded[i] = false;
        }
    }

    discardsRemaining--;
}

// --- HAND EVALUATION ---
HandType evaluateHand(const vector<Card>& cards)
{
    vector<int> ranks;
    vector<char> suits;

    for (auto& c : cards)
    {
        ranks.push_back(c.value);
        suits.push_back(c.suit);
    }

    sort(ranks.begin(), ranks.end());

    bool flush = all_of(suits.begin(), suits.end(), [&](char s) { return s == suits[0]; });
    bool straight = true;
    for (int i = 1; i < ranks.size(); i++)
        if (ranks[i] != ranks[i - 1] + 1)
            straight = false;

    int counts[15] = { 0 };
    for (int r : ranks) counts[r]++;

    int pairs = 0;
    bool three = false;
    bool four = false;

    for (int i = 1; i <= 13; i++)
    {
        if (counts[i] == 2) pairs++;
        if (counts[i] == 3) three = true;
        if (counts[i] == 4) four = true;
    }

    if (straight && flush) return STRAIGHT_FLUSH;
    if (four) return FOUR_KIND;
    if (three && pairs) return FULL_HOUSE;
    if (flush) return FLUSH;
    if (straight) return STRAIGHT;
    if (three) return THREE_KIND;
    if (pairs == 2) return TWO_PAIR;
    if (pairs == 1) return PAIR;

    return HIGH_CARD;
}

void playAsciiAnimation() {
    vector<string> cardArt = {
        "  _____",
        " |\\ ~ /|",
        " |}}:{{|",
        " |}}:{{|  _____",
        " |}}:{{| |Joker|",
        " |/_~_\\||==%, |",
        "         | \\/>\\|",
        "         | _>^^|                 _____",
        "     ____|/_\\/|          _____  |6    |",
        "    |2    | _____        |5    || ^ ^ |",
        "    |  ^  ||3    | _____ | ^ ^ || ^ ^ | _____",
        "    |     || ^ ^ ||4    ||  ^  || ^ ^ ||7    |",
        "    |  ^  ||     || ^ ^ || ^ ^ ||____9|| ^ ^ | _____",
        "    |____Z||  ^  ||     ||____S|       |^ ^ ^||8    | _____",
        "           |____E|| ^ ^ |              | ^ ^ ||^ ^ ^||9    |",
        "                  |____h|              |____L|| ^ ^ ||^ ^ ^|",
        "                              _____           |^ ^ ^||^ ^ ^|",
        "                      _____  |K  WW|          |____8||^ ^ ^|",
        "              _____  |Q  ww| | ^ {)|                 |____6|",
        "       _____ |J  ww| | ^ {(| |(.)%%| _____",
        "      |10 ^ || ^ {)| |(.)%%| | |%%%||A .  |",
        "      |^ ^ ^||(.)% | | |%%%| |_%%%>|| /.\\ |",
        "      |^ ^ ^|| | % | |_%%%O|        |(_._)|",
        "      |^ ^ ^||__%%[|                |  |  |",
        "      |___0I|                       |____V|"
    };

    int consoleWidth = 120;
    int xPos = 0;
    int yOffset = 0;
    int yDir = 1;

    for (int frame = 0; frame < 50; frame++) {
        system("cls");

        yOffset += yDir;
        if (yOffset > 5 || yOffset < 0) yDir *= -1;

        for (int i = 0; i < cardArt.size(); i++) {
            for (int v = 0; v < yOffset; v++)
                cout << "\n";

            int spaceCount = xPos % (consoleWidth - 20);
            for (int s = 0; s < spaceCount; s++)
                cout << " ";

            cout << cardArt[i] << "\n";
        }

        Sleep(150);
        xPos += 3;
    }
}

