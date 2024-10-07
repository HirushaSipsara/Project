#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_DICE 5
#define NUM_ROLLS 3
#define NUM_ROUNDS 13

// Function declarations
void user_turn(int dice[], int user_score[], int scorecard[]);
void select_scoring(int dice[], int scorecard[], int user_score[]);
int calculate_score(int dice[], int category, int scorecard[], int user_score[]);
int has_category_been_chosen(int category, int user_score[]);
void roll_dice(int dice[], int keep_dices[]);
void print_dice(int dice[]);
void print_score(int score[]);
void computer_turn(int dice[], int computer_score[], int scorecard[]);
void evaluate_dice(int dice[], int keep[]);
int choose_best_category(int dice[], int computer_score[], int scorecard[]);

// User's Turn
void user_turn(int dice[], int user_score[], int scorecard[]) {
    int roll_count = 0;
    int keep_dices[NUM_DICE] = {0};
    char choice;

    while (roll_count < NUM_ROLLS) {
        printf("%d Roll :\n", roll_count + 1);
        roll_dice(dice, keep_dices);
        print_dice(dice);

        if (roll_count < NUM_ROLLS -1) {
            printf("Would you like to keep any dice? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                for (int i = 0; i < NUM_DICE; i++) {
                    printf("Keep dice %d (value: %d)? (1 = yes, 0 = no): ", i + 1, dice[i]);
                    scanf("%d", &keep_dices[i]);
                }
            }
        }
        roll_count++;
    }
    
    // After rolling, select a scoring category
    select_scoring(dice, scorecard, user_score);
}

// Select scoring category
void select_scoring(int dice[], int scorecard[], int user_score[]) {
    int category;
    char *scoringOptions[] = {
        "0 = Ones", "1 = Twos", "2 = Threes", "3 = Fours", "4 = Fives", "5 = Sixes",
        "6 = Three of a Kind", "7 = Four of a Kind", "8 = Full House", "9 = Small Straight",
        "10 = Large Straight", "11 = Yahtzee", "12 = Chance"
    };

    do {
        printf("Available scoring categories:\n");
        for (int i = 0; i < NUM_ROUNDS; i++) {
            if (scorecard[i] == 0) {
                printf("%s\n", scoringOptions[i]);
            }
        }
        printf("Choose a category (0-12): ");
        scanf("%d", &category);
        
        if (category < 0 || category > 12) {
            printf("Invalid category. Please select a valid option.\n");
        } else if (has_category_been_chosen(category, user_score)) {
            printf("Category already chosen. Select another.\n");
        }
    } while (category < 0 || category > 12 || has_category_been_chosen(category, user_score));

    user_score[category] = calculate_score(dice, category, scorecard, user_score);
    scorecard[category] = 1;
}

// Calculate Score based on category
int calculate_score(int dice[], int category, int scorecard[], int user_score[]) {
    int score = 0, ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixs = 0;
    
    // Count dice occurrences
    for (int i = 0; i < NUM_DICE; i++) {
        switch (dice[i]) {
            case 1: ones++; break;
            case 2: twos++; break;
            case 3: threes++; break;
            case 4: fours++; break;
            case 5: fives++; break;
            case 6: sixs++; break;
        }
    }
    
    // Implement scoring based on category
    switch (category) {
        case 0: score = ones; break;  // Ones
        case 1: score = twos * 2; break;  // Twos
        case 2: score = threes * 3; break;  // Threes
        case 3: score = fours * 4; break;  // Fours
        case 4: score = fives * 5; break;  // Fives
        case 5: score = sixs * 6; break;  // Sixes
        case 6:  // Three of a Kind
            if (ones >= 3 || twos >= 3 || threes >= 3 || fours >= 3 || fives >= 3 || sixs >= 3) {
                score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
            }
            break;
        case 7:  // Four of a Kind
            if (ones >= 4 || twos >= 4 || threes >= 4 || fours >= 4 || fives >= 4 || sixs >= 4) {
                score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
            }
            break;
        case 8:  // Full House
            if ((ones == 3 || twos == 3 || threes == 3 || fours == 3 || fives == 3 || sixs == 3) &&
                (ones == 2 || twos == 2 || threes == 2 || fours == 2 || fives == 2 || sixs == 2)) {
                score = 25;
            }
            break;
        case 9:  // Small Straight
            if ((ones && twos && threes && fours) || (twos && threes && fours && fives) || 
                (threes && fours && fives && sixs)) {
                score = 30;
            }
            break;
        case 10:  // Large Straight
            if ((ones && twos && threes && fours && fives) || (twos && threes && fours && fives && sixs)) {
                score = 40;
            }
            break;
        case 11:  // Yahtzee
            if (ones == 5 || twos == 5 || threes == 5 || fours == 5 || fives == 5 || sixs == 5) {
                score = 50;
            }
            break;
        case 12:  // Chance
            score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
            break;
        default: 
            printf("Error in score calculation.\n");
            break;
    }
    return score;
}

// Check if category has been chosen already
int has_category_been_chosen(int category, int user_score[]) {
    return user_score[category] != 0;
}

// Roll Dice Function
void roll_dice(int dice[], int keep_dices[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep_dices[i] == 0) {  // Only roll dice that are not kept
            dice[i] = rand() % 6 + 1;
        }
    }
}

// Print Dice
void print_dice(int dice[]) {
    printf("Dice: ");
    for (int i = 0; i < NUM_DICE; i++) {
        printf("%d ", dice[i]);
    }
    printf("\n");
}

// Print the scorecard
void print_score(int score[]) {
    printf("Scorecard: ");
    for (int i = 0; i < NUM_ROUNDS; i++) {
        printf("%d ", score[i]);
    }
    printf("\n");
}

// Computer's Turn
void computer_turn(int dice[], int computer_score[], int scorecard[]) {
    int roll_count = 0;
    int keep[NUM_DICE] = {0};  // The computer decides which dice to keep

    while (roll_count < NUM_ROLLS) {
        roll_dice(dice, keep);
        roll_count++;

        if (roll_count < NUM_ROLLS) {
            evaluate_dice(dice, keep);  // Basic strategy to keep useful dice
        }
    }

    int best_category = choose_best_category(dice, computer_score, scorecard);
    computer_score[best_category] = calculate_score(dice, best_category, scorecard, computer_score);
    scorecard[best_category] = 1;
}

// Evaluate the dice and choose which to keep
void evaluate_dice(int dice[], int keep[]) {
    // Simple logic: keep dice that are part of a pair or higher
    int count[6] = {0};

    for (int i = 0; i < NUM_DICE; i++) {
        count[dice[i] - 1]++;
    }

    for (int i = 0; i < NUM_DICE; i++) {
        if (count[dice[i] - 1] > 1) {
            keep[i] = 1;  // Keep dice that are part of a pair or more
        } else {
            keep[i] = 0;  // Otherwise, reroll the dice
        }
    }
}

// Choose the best scoring category for the computer
int choose_best_category(int dice[], int computer_score[], int scorecard[]) {
    int category = -1;
    int max_score = 0;

    for (int i = 0; i < NUM_ROUNDS; i++) {
        if (scorecard[i] == 0) {  // Check available categories
            int score = calculate_score(dice, i, scorecard, computer_score);
            if (score > max_score) {
                max_score = score;
                category = i;
            }
        }
    }

    return category;
}

// Main Function
int main() {
    srand(time(NULL));

    int dice[NUM_DICE] = {0};
    int user_score[NUM_ROUNDS] = {0};
    int computer_score[NUM_ROUNDS] = {0};
    int scorecard[NUM_ROUNDS] = {0};

    for (int round = 0; round < NUM_ROUNDS; round++) {
        printf("User's Turn (Round %d):\n", round + 1);
        user_turn(dice, user_score, scorecard);

        printf("Computer's Turn (Round %d):\n", round + 1);
        computer_turn(dice, computer_score, scorecard);

        printf("User's score so far:\n");
        print_score(user_score);

        printf("Computer's score so far:\n");
        print_score(computer_score);
    }

    printf("Final Scores:\n");
    printf("User's score: ");
    print_score(user_score);

    printf("Computer's score: ");
    print_score(computer_score);

    return 0;
}
