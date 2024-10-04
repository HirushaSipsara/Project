#include <stdio.h>

int main() {
    const char *scoringOptions[] = {
        "0 = Ones",
        "1 = Twos",
        "2 = Threes",
        "3 = Fours",
        "4 = Fives",
        "5 = Sixes",
        "6 = Three of a Kind",
        "7 = Four of a Kind",
        "8 = Full House",
        "9 = Small Straight",
        "10 = Large Straight",
        "11 = Yahtzee",
        "12 = Chance"
    };

    // Example: print the scoring options
    for (int i = 0; i < sizeof(scoringOptions) / sizeof(scoringOptions[0]); i++) {
        printf("%s\n", scoringOptions[i]);
    }

    return 0;
}
