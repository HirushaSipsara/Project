#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_DICE 5
#define NUM_ROLLS 3
#define NUM_ROUNDS 13

// Function Prototypes
void roll_dice(int dice[], int keep[]);
void print_dice(int dice[]);
void human_turn(int dice[], int scorecard[]);
void computer_turn(int dice[], int scorecard[]);
int calculate_score(int dice[], int category);
int choose_category(int scorecard[]);
int has_category_been_chosen(int category, int scorecard[]);
void print_scorecard(int scorecard[]);

int main() {
    srand(time(NULL));
    
    int human_scorecard[NUM_ROUNDS] = {0};
    int computer_scorecard[NUM_ROUNDS] = {0};
    int dice[NUM_DICE];
    
    for (int round = 0; round < NUM_ROUNDS; round++) {
        printf("\nRound %d\n", round + 1);
        
        // Human Player's Turn
        printf("Human's turn:\n");
        human_turn(dice, human_scorecard);
        print_scorecard(human_scorecard);
        
        // Computer Player's Turn
        printf("\nComputer's turn:\n");
        computer_turn(dice, computer_scorecard);
        print_scorecard(computer_scorecard);
    }
    
    // Final Scores
    int human_total = 0, computer_total = 0;
    for (int i = 0; i < NUM_ROUNDS; i++) {
        human_total += human_scorecard[i];
        computer_total += computer_scorecard[i];
    }
    
    printf("\nFinal Scores: Human = %d, Computer = %d\n", human_total, computer_total);
    if (human_total > computer_total)
        printf("Human wins!\n");
    else if (computer_total > human_total)
        printf("Computer wins!\n");
    else
        printf("It's a tie!\n");

    return 0;
}

// Roll Dice Function
void roll_dice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (!keep[i]) {  // Only roll dice that are not kept
            dice[i] = rand() % 6 + 1;  // Roll a dice (1 to 6)
        }
    }
    print_dice(dice);
}

// Print Dice
void print_dice(int dice[]) {
    printf("Dice: ");
    for (int i = 0; i < NUM_DICE; i++) {
        printf("%d ", dice[i]);
    }
    printf("\n");
}

// Human Turn (allows keeping dice)
void human_turn(int dice[], int scorecard[]) {
    int roll_count = 0;
    int keep[NUM_DICE] = {0};  // Tracks which dice are kept (0 = not kept, 1 = kept)
    char choice;
    
    while (roll_count < NUM_ROLLS) {
        printf("Roll %d:\n", roll_count + 1);
        roll_dice(dice, keep);
        
        if (roll_count < NUM_ROLLS - 1) {
            printf("Would you like to keep any dice? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                // Ask the user which dice to keep
                for (int i = 0; i < NUM_DICE; i++) {
                    printf("Keep dice %d (value: %d)? (1 = yes, 0 = no): ", i + 1, dice[i]);
                    scanf("%d", &keep[i]);  // 1 = keep, 0 = re-roll
                }
            }
        }

        // Ask if they want to roll again or stop rolling
        if (roll_count < NUM_ROLLS - 1) {
            printf("Do you want to re-roll remaining dice? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'n' || choice == 'N') {
                break;  // Stop rolling and go to score selection
            }
        }
        roll_count++;
    }
    
    // Select scoring category
    int category;
    do {
        printf("Choose a category (0-12): ");
        scanf("%d", &category);
    } while (has_category_been_chosen(category, scorecard));
    
    scorecard[category] = calculate_score(dice, category);
}

// Computer Turn (simple AI choosing optimal scoring)
void computer_turn(int dice[], int scorecard[]) {
    int roll_count = 0;
    int keep[NUM_DICE] = {0};  // The computer can also decide to keep dice
    
    while (roll_count < NUM_ROLLS) {
        roll_dice(dice, keep);
        roll_count++;
    }
    
    // Choose the best scoring category
    int best_category = choose_category(scorecard);
    scorecard[best_category] = calculate_score(dice, best_category);
}

// Calculate Score based on category
int calculate_score(int dice[], int category) {
    int score = 0;
    
    // Implement scoring based on category
    switch (category) {
        case 0: // Ones
            for (int i = 0; i < NUM_DICE; i++)
                if (dice[i] == 1) score += 1;
            break;
        case 1: // Twos
            for (int i = 0; i < NUM_DICE; i++)
                if (dice[i] == 2) score += 2;
            break;
        // Continue implementing for all categories (Threes, Fours, Full House, etc.)
        default:
            break;
    }
    
    return score;
}

// Choose an available category for the computer
int choose_category(int scorecard[]) {
    for (int i = 0; i < NUM_ROUNDS; i++) {
        if (scorecard[i] == 0) {
            return i;  // Choose first available category
        }
    }
    return 0;
}

// Check if category has been chosen already
int has_category_been_chosen(int category, int scorecard[]) {
    return scorecard[category] != 0;
}

// Print the scorecard
void print_scorecard(int scorecard[]) {
    printf("Scorecard: ");
    for (int i = 0; i < NUM_ROUNDS; i++) {
        printf("%d ", scorecard[i]);
    }
    printf("\n");
}
