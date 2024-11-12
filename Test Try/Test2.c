#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define NUM_DICE 5
#define MAX_ROLLS 3
#define NUM_ROUNDS 13
#define BONUS_THRESHOLD 63  
#define BONUS_SCORE 35
#define YAHTZEE_SCORE 50
#define EXTRA_YAHTZEE_BONUS 100

// Scoring categories
#define ONES 0
#define TWOS 1
#define THREES 2
#define FOURS 3
#define FIVES 4
#define SIXES 5
#define THREE_OF_A_KIND 6
#define FOUR_OF_A_KIND 7
#define FULL_HOUSE 8
#define SMALL_STRAIGHT 9
#define LARGE_STRAIGHT 10
#define YAHTZEE 11
#define CHANCE 12

// Function to roll the dice
void roll_dice(int dice[NUM_DICE]) {
    for (int i = 0; i < NUM_DICE; i++) {
        dice[i] = (rand() % 6) + 1; // Random number between 1 and 6
    }
}

// Function to display the dice
void display_dice(int dice[NUM_DICE]) {
    printf("Dice: ");
    for (int i = 0; i < NUM_DICE; i++) {
        printf("%d ", dice[i]);
    }
    printf("\n");
}

// Function to reroll dice based on user's input
void reroll_dice(int dice[NUM_DICE], int to_reroll[NUM_DICE]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (to_reroll[i]) {
            dice[i] = (rand() % 6) + 1;
        }
    }
}

// Function to calculate upper section score (ones to sixes)
int upper_section_score(int dice[NUM_DICE], int num) {
    int score = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        if (dice[i] == num) {
            score += num;
        }
    }
    return score;
}

// Function to check if a specific number of a kind is present
bool has_of_a_kind(int dice[NUM_DICE], int kind) {
    int counts[6] = {0};
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i] >= kind) {
            return true;
        }
    }
    return false;
}

// Function to calculate the sum of all dice
int sum_of_all_dice(int dice[NUM_DICE]) {
    int sum = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        sum += dice[i];
    }
    return sum;
}

// Function to check for a full house
bool is_full_house(int dice[NUM_DICE]) {
    int counts[6] = {0};
    bool three_of_a_kind = false;
    bool two_of_a_kind = false;
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i] == 3) {
            three_of_a_kind = true;
        } else if (counts[i] == 2) {
            two_of_a_kind = true;
        }
    }
    return three_of_a_kind && two_of_a_kind;
}

// Function to check for a straight (small or large)
bool is_straight(int dice[NUM_DICE], int length) {
    int counts[6] = {0};
    int consecutive = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1] = 1;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i]) {
            consecutive++;
            if (consecutive >= length) {
                return true;
            }
        } else {
            consecutive = 0;
        }
    }
    return false;
}

// Function for the player's turn
void player_turn(int *score, int *yahtzee_bonus, int *upper_section_sum, bool used_categories[NUM_ROUNDS]) {
    int dice[NUM_DICE];
    int to_reroll[NUM_DICE];
    int rolls = 0;
    int round_score = 0;
    int choice;

    roll_dice(dice);

    while (rolls < MAX_ROLLS) {
        display_dice(dice);

        if (rolls < MAX_ROLLS - 1) {
            printf("Do you want to reroll? (y/n): ");
            char reroll_choice;
            scanf(" %c", &reroll_choice);

            if (reroll_choice == 'y' || reroll_choice == 'Y') {
                printf("Which dice to reroll (1 for yes, 0 for no, separated by spaces): ");
                for (int i = 0; i < NUM_DICE; i++) {
                    scanf("%d", &to_reroll[i]);
                }
                reroll_dice(dice, to_reroll);
            } else {
                break;
            }
        }
        rolls++;
    }

    do {
        printf("Choose a scoring category:\n");
        printf("1. Ones\n2. Twos\n3. Threes\n4. Fours\n5. Fives\n6. Sixes\n");
        printf("7. Three of a Kind\n8. Four of a Kind\n9. Full House\n");
        printf("10. Small Straight\n11. Large Straight\n12. Yahtzee\n13. Chance\n");
        printf("Enter category number: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 13) {
            printf("Invalid category! Please choose a valid category.\n");
            continue;
        }

        if (used_categories[choice - 1]) {
            printf("Category already used! Please choose another category.\n");
            continue;
        } else {
            used_categories[choice - 1] = true;
            break;
        }
    } while (true);

    switch (choice) {
        case 1: round_score = upper_section_score(dice, 1); *upper_section_sum += round_score; break;
        case 2: round_score = upper_section_score(dice, 2); *upper_section_sum += round_score; break;
        case 3: round_score = upper_section_score(dice, 3); *upper_section_sum += round_score; break;
        case 4: round_score = upper_section_score(dice, 4); *upper_section_sum += round_score; break;
        case 5: round_score = upper_section_score(dice, 5); *upper_section_sum += round_score; break;
        case 6: round_score = upper_section_score(dice, 6); *upper_section_sum += round_score; break;
        case 7: round_score = has_of_a_kind(dice, 3) ? sum_of_all_dice(dice) : 0; break;
        case 8: round_score = has_of_a_kind(dice, 4) ? sum_of_all_dice(dice) : 0; break;
        case 9: round_score = is_full_house(dice) ? 25 : 0; break;
        case 10: round_score = is_straight(dice, 4) ? 30 : 0; break;
        case 11: round_score = is_straight(dice, 5) ? 40 : 0; break;
        case 12:
            if (has_of_a_kind(dice, 5)) {
                if (*yahtzee_bonus > 0) {
                    round_score = EXTRA_YAHTZEE_BONUS;
                } else {
                    round_score = YAHTZEE_SCORE;
                    *yahtzee_bonus = 1;
                }
            } else {
                round_score = 0;
            }
            break;
        case 13: round_score = sum_of_all_dice(dice); break;
    }
    *score += round_score;
    printf("You scored %d points in this round.\n", round_score);
}

// Function for the computer's turn (AI)
void computer_turn(int *score, int *yahtzee_bonus, int *upper_section_sum, bool used_categories[NUM_ROUNDS]) {
    int dice[NUM_DICE];
    int to_reroll[NUM_DICE] = {0};
    int rolls = 0;
    int round_score = 0;

    roll_dice(dice);

    while (rolls < MAX_ROLLS) {
        // Simple AI strategy: Keep dice that could lead to high scoring categories
        int counts[6] = {0};
        for (int i = 0; i < NUM_DICE; i++) {
            counts[dice[i] - 1]++;
        }

        // Determine which dice to reroll
        for (int i = 0; i < NUM_DICE; i++) {
            to_reroll[i] = 1; // Default to reroll
        }

        // Keep dice based on counts
        int max_count = 0;
        int max_value = 0;
        for (int i = 0; i < 6; i++) {
            if (counts[i] > max_count) {
                max_count = counts[i];
                max_value = i + 1;
            }
        }

        // Keep dice that match max_value
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == max_value) {
                to_reroll[i] = 0;
            }
        }

        // Reroll decision
        if (rolls < MAX_ROLLS - 1) {
            reroll_dice(dice, to_reroll);
        }

        rolls++;
    }

    // AI chooses a category
    int choice = -1;
    if (!used_categories[YAHTZEE] && has_of_a_kind(dice, 5)) {
        choice = YAHTZEE;
        if (*yahtzee_bonus > 0) {
            round_score = EXTRA_YAHTZEE_BONUS;
        } else {
            round_score = YAHTZEE_SCORE;
            *yahtzee_bonus = 1;
        }
    } else if (!used_categories[LARGE_STRAIGHT] && is_straight(dice, 5)) {
        choice = LARGE_STRAIGHT;
        round_score = 40;
    } else if (!used_categories[FULL_HOUSE] && is_full_house(dice)) {
        choice = FULL_HOUSE;
        round_score = 25;
    } else if (!used_categories[FOUR_OF_A_KIND] && has_of_a_kind(dice, 4)) {
        choice = FOUR_OF_A_KIND;
        round_score = sum_of_all_dice(dice);
    } else if (!used_categories[THREE_OF_A_KIND] && has_of_a_kind(dice, 3)) {
        choice = THREE_OF_A_KIND;
        round_score = sum_of_all_dice(dice);
    } else {
        // Try to fill upper section
        for (int i = SIXES; i >= ONES; i--) {
            if (!used_categories[i]) {
                choice = i;
                round_score = upper_section_score(dice, i + 1);
                *upper_section_sum += round_score;
                break;
            }
        }
        // If no other categories available, choose Chance
        if (choice == -1 && !used_categories[CHANCE]) {
            choice = CHANCE;
            round_score = sum_of_all_dice(dice);
        }
    }

    if (choice != -1 && !used_categories[choice]) {
        used_categories[choice] = true;
        *score += round_score;
        // For demonstration, you might want to print what category the computer chose
        printf("Computer chose category %d and scored %d points.\n", choice + 1, round_score);
    } else {
        printf("Computer could not choose a category.\n");
    }
}

int main() {
    srand(time(NULL));

    int player_score = 0, computer_score = 0;
    int player_upper_section_sum = 0, computer_upper_section_sum = 0;
    int player_yahtzee_bonus = 0, computer_yahtzee_bonus = 0;
    bool player_used_categories[NUM_ROUNDS] = {false};
    bool computer_used_categories[NUM_ROUNDS] = {false};

    printf("Welcome to Yahtzee with AI!\n");

    for (int round = 0; round < NUM_ROUNDS; round++) {
        printf("\n--- Round %d ---\n", round + 1);

        // Player's turn
        printf("Your turn:\n");
        player_turn(&player_score, &player_yahtzee_bonus, &player_upper_section_sum, player_used_categories);

        // Computer's turn
        printf("\nComputer's turn:\n");
        computer_turn(&computer_score, &computer_yahtzee_bonus, &computer_upper_section_sum, computer_used_categories);

        // Display scores after each round
        printf("\nScores after Round %d:\n", round + 1);
        printf("Your Score: %d\n", player_score);
        printf("Computer's Score: %d\n", computer_score);
    }

    // Check for upper section bonus
    if (player_upper_section_sum >= BONUS_THRESHOLD) {
        player_score += BONUS_SCORE;
        printf("\nYou earned the upper section bonus of %d points!\n", BONUS_SCORE);
    }
    if (computer_upper_section_sum >= BONUS_THRESHOLD) {
        computer_score += BONUS_SCORE;
        printf("\nComputer earned the upper section bonus of %d points!\n", BONUS_SCORE);
    }

    // Final scores
    printf("\nFinal Scores:\n");
    printf("Your Final Score: %d\n", player_score);
    printf("Computer's Final Score: %d\n", computer_score);

    // Determine winner
    if (player_score > computer_score) {
        printf("\nCongratulations! You won!\n");
    } else if (computer_score > player_score) {
        printf("\nThe computer won!\n");
    } else {
        printf("\nIt's a tie!\n");
    }
    // Game over, show final scores

printf("\nThank you for playing! Press Enter to exit.\n");
getchar();  // Wait for user input
getchar();  // To capture the extra newline character
 return 0;
}