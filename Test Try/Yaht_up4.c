#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define DICE_COUNT 5
#define ROLLS 3

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
#define CATEGORY_COUNT 13

// Function prototypes
void roll_dice(int dice[]);
void reroll_dice(int dice[], int reroll[]);
void print_dice(int dice[]);
char ask_to_reroll();
void ask_which_dice_to_reroll(int reroll[]);
int score_upper_section(int dice[], int num);
int score_three_of_a_kind(int dice[]);
int score_four_of_a_kind(int dice[]);
int score_full_house(int dice[]);
int score_small_straight(int dice[]);
int score_large_straight(int dice[]);
int score_yahtzee(int dice[]);
int score_chance(int dice[]);
void computer_optimal_reroll(int dice[], int used_categories[]);
int calculate_score_for_category(int dice[], int category);
int computer_choose_category(int dice[], int used_categories[]);
int check_for_three_of_a_kind(int dice[]);
int check_for_four_of_a_kind(int dice[]);
int check_for_small_straight(int dice[]);
int check_for_large_straight(int dice[]);
void reroll_non_matching_dice(int dice[], int reroll[]);
//void reroll_for_best_upper_section(int dice[], int reroll[]);
void display_score_table(int human_scores[], int computer_scores[], int human_total, int computer_total); 
void roll_towards_full_house(int dice[]);
bool check_for_yahtzee(int dice[]);
bool check_for_full_house(int dice[]);
//void computer_optimal_low_category(int dice[], int reroll[], int category_value);
int find_most_common_dice(int dice[]);
void lock_matching_dice(int dice[], int reroll[], int value, int count);
int find_most_frequent_value(int dice[]);
//void reroll_for_large_straight(int dice[], int reroll[]);
void reroll_for_optimal_number(int dice[], int reroll[]);
void lock_high_value_dice(int dice[], int reroll[]);


// Helper functions
void initialize_scores(int scores[], int used[]);
void display_scores(int human_scores[], int computer_scores[], int human_total, int computer_total);
void reset_reroll_array(int reroll[]);
void finalize_round(int human_total, int computer_total);
int compare(const void *a, const void *b);

// Roll all dice randomly
void roll_dice(int dice[]) {
    for (int i = 0; i < DICE_COUNT; i++) {
        dice[i] = (rand() % 6) + 1; // Random number between 1 and 6
    }
}

void reroll_for_large_straight(int dice[], int reroll[]) {
    int target_sequence[] = {1, 2, 3, 4, 5}; // One example of large straight
    int found[DICE_COUNT] = {0}; // Track if each target number is found

    // Check which numbers are part of the large straight
    for (int i = 0; i < DICE_COUNT; i++) {
        for (int j = 0; j < 5; j++) {
            if (dice[i] == target_sequence[j]) {
                found[j] = 1;
                break;
            }
        }
    }

    // Reroll dice that are not part of the large straight
    for (int i = 0; i < DICE_COUNT; i++) {
        int is_in_straight = 0;
        for (int j = 0; j < 5; j++) {
            if (dice[i] == target_sequence[j] && found[j]) {
                is_in_straight = 1;
                found[j] = 0; // Mark as used
                break;
            }
        }
        reroll[i] = !is_in_straight; // Reroll if not in the straight
    }


    // Reroll dice that are not part of the small straight
    for (int i = 0; i < DICE_COUNT; i++) {
        int is_in_straight = 0;
        for (int j = 0; j < 4; j++) {
            if (dice[i] == target_sequence[j] && found[j]) {
                is_in_straight = 1;
                found[j] = 0; // Mark as used
                break;
            }
        }
        reroll[i] = !is_in_straight; // Reroll if not in the straight
    }
}

void reroll_for_optimal_number(int dice[], int reroll[]) {
    int freq[7] = {0}; // To track frequency of dice values (1 to 6)

    // Count the frequency of each dice value
    for (int i = 0; i < DICE_COUNT; i++) {
        freq[dice[i]]++;
    }

    // Find the most frequent dice value
    int target = 1; // Default to 1
    for (int i = 2; i <= 6; i++) {
        if (freq[i] > freq[target]) {
            target = i;
        }
    }

    // Lock the dice with the most frequent value and reroll others
    for (int i = 0; i < DICE_COUNT; i++) {
        if (dice[i] == target) {
            reroll[i] = 0; // Lock the most frequent value
        } else {
            reroll[i] = 1; // Reroll other dice
        }
    }
}

void lock_high_value_dice(int dice[], int reroll[]) {
    for (int i = 0; i < DICE_COUNT; i++) {
        if (dice[i] >= 5) { // Lock 5s and 6s
            reroll[i] = 0; // Do not reroll high-value dice
        } else {
            reroll[i] = 1; // Reroll low-value dice
        }
    }
}


void roll_towards_full_house(int dice[]) {
    int count[6] = {0};  // To count occurrences of each dice face (1-6)
    int pair1 = -1, pair2 = -1;  // To store the numbers with 2 occurrences

    // Count occurrences of each face
    for (int i = 0; i < DICE_COUNT; i++) {
        count[dice[i] - 1]++;
    }

    // Identify two pairs
    for (int i = 0; i < 6; i++) {
        if (count[i] == 2) {
            if (pair1 == -1) {
                pair1 = i + 1; // First pair
            } else {
                pair2 = i + 1; // Second pair
                break;
            }
        }
    }

    // If there are exactly two pairs, reroll the odd die
    if (pair1 != -1 && pair2 != -1) {
        for (int i = 0; i < DICE_COUNT; i++) {
            // Reroll any die that is neither of the pairs
            if (dice[i] != pair1 && dice[i] != pair2) {
                dice[i] = (rand() % 2 == 0) ? pair1 : pair2; // Roll towards either pair
                break;
            }
        }
    }
}


// Initialize the scores and used category arrays
void initialize_scores(int scores[], int used[]) {
    for (int i = 0; i < 13; i++) {
        scores[i] = 0;
        used[i] = 0;
    }
}


// Print the current values of the dice
void print_dice(int dice[]) {
    printf("Current dice values: ");
    for (int i = 0; i < DICE_COUNT; i++) {
        printf("%d ", dice[i]);
    }
    printf("\n");
}

// Reroll dice based on the user's decision (1 = reroll, 0 = keep)
void reroll_dice(int dice[], int reroll[]) {
    for (int i = 0; i < DICE_COUNT; i++) {
        if (reroll[i] == 1) { // Only reroll dice marked with 1
            dice[i] = rand() % 6 + 1;
        }
    }
}

// Reset rerollll array to all zeros (no reroll)
// void reset_reroll_array(int reroll[]) {
//     for (int i = 0; i < DICE_COUNT; i++) {
//         reroll[i] = 0;
//     }
// }


const char *category_names[CATEGORY_COUNT] = {
    "1s", "2s", "3s", "4s", "5s", "6s", 
    "Three of a Kind", "Four of a Kind", "Full House", 
    "Small Straight", "Large Straight", "Yahtzee", "Chance"
};


// Function to display the score table for both players
void display_score_table(int human_scores[], int computer_scores[], int human_total, int computer_total) {

    printf("+--------------------------+------------------+----------------+\n");
    printf("|        Category          |    Player Score  | Computer Score |\n");
    printf("+--------------------------+------------------+----------------+\n");

    // Display scores for each category
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        printf("| %-24s |       %-6d     |     %-6d     |\n", category_names[i], human_scores[i], computer_scores[i]);
    }

    // printf("+--------------------------+------------------+----------------+\n");
    // // Display bonus if applicable
    // printf("| %-24s |       %-6d     |     %-6d     |\n", "Bonus", player_bonus, computer_bonus);
    printf("+--------------------------+------------------+----------------+\n");

    // Display total scores including bonuses
    printf("| %-24s |       %-6d     |     %-6d     |\n", "Total Score", human_total , computer_total);
    printf("+--------------------------+------------------+----------------+\n");
    printf("\n");
}

// Display the scores after each round
void display_scores(int human_scores[], int computer_scores[], int human_total, int computer_total) {
    printf("\n--- Current Scores ---\n");
    printf("Human: %d\n", human_total);
    printf("Computer: %d\n", computer_total);
}

// Calculate the score for upper section categories (ONES to SIXES)
int score_upper_section(int dice[], int num) {
    int score = 0;
    for (int i = 0; i < DICE_COUNT; i++) {
        if (dice[i] == num) { // Score only the dice that match the number
            score += num;
        }
    }
    return score;
}

// Check if there are three of a kind and return the sum of all dice
int score_three_of_a_kind(int dice[]) {
    return check_for_three_of_a_kind(dice) ? score_chance(dice) : 0;
}

// Check if there are four of a kind and return the sum of all dice
int score_four_of_a_kind(int dice[]) {
    return check_for_four_of_a_kind(dice) ? score_chance(dice) : 0;
}

// Score full house (three of one number, two of another) for 25 points
int score_full_house(int dice[]) {
    int freq[7] = {0};
    for (int i = 0; i < DICE_COUNT; i++) {
        freq[dice[i]]++;
    }
    int has_three = 0, has_two = 0;
    for (int i = 1; i <= 6; i++) {
        if (freq[i] == 3) has_three = 1;
        if (freq[i] == 2) has_two = 1;
    }
    return has_three && has_two ? 25 : 0;
}

// Score small straight (four consecutive numbers) for 30 points
int score_small_straight(int dice[]) {
    qsort(dice, DICE_COUNT, sizeof(int), compare);
    if ((dice[0] == dice[1] - 1 && dice[1] == dice[2] - 1 && dice[2] == dice[3] - 1) ||
        (dice[1] == dice[2] - 1 && dice[2] == dice[3] - 1 && dice[3] == dice[4] - 1) ||
        (dice[2] == dice[3] - 1 && dice[3] == dice[4] - 1 && dice[4] == dice[5] - 1)) {
        return 30;
    }
    return 0;
}

// Score large straight (five consecutive numbers) for 40 points
int score_large_straight(int dice[]) {
    qsort(dice, DICE_COUNT, sizeof(int), compare);
    if ((dice[0] == dice[1] - 1 && dice[1] == dice[2] - 1 && dice[2] == dice[3] - 1 && dice[3] == dice[4] - 1) ||
        (dice[1] == dice[2] - 1 && dice[2] == dice[3] - 1 && dice[3] == dice[4] - 1 && dice[4] == dice[5] - 1)) {
        return 40;
    }
    return 0;
}

// Score yahtzee (all five dice the same) for 50 points
int score_yahtzee(int dice[]) {
    for (int i = 1; i < DICE_COUNT; i++) {
        if (dice[i] != dice[0]) {
            return 0; // Not all dice are the same
        }
    }
    return 50; // All dice are the same
}

// Score chance (sum of all dice, regardless of combinations)
int score_chance(int dice[]) {
    int sum = 0;
    for (int i = 0; i < DICE_COUNT; i++) {
        sum += dice[i]; // Sum up all dice values
    }
    return sum;
}

// Helper function to compare integers (for qsort)
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b); // Sorting in ascending order
}

// AI chooses the optimal category for the computer player
int computer_choose_category(int dice[], int used_categories[]) {
    // Calculate optimal scores
    int computer_current[13];
    computer_current[0] = score_upper_section(dice,1);
    computer_current[1] = score_upper_section(dice,2);
    computer_current[2] = score_upper_section(dice,3);
    computer_current[3] = score_upper_section(dice,4);
    computer_current[4] = score_upper_section(dice,5);
    computer_current[5] = score_upper_section(dice,6);
    computer_current[6] = score_three_of_a_kind(dice);
    computer_current[7] = score_four_of_a_kind(dice);
    computer_current[8] = score_full_house(dice);
    computer_current[9] = score_small_straight(dice);
    computer_current[10] = score_large_straight(dice);
    computer_current[11] = score_yahtzee(dice);
    computer_current[12] = score_chance(dice);

    int maxScoreCat = -1;
    int maxScore = 0;
    // Choose the category intelligently based on the current dice
    for (int i = 0; i < 13; i++) {
        if (computer_current[i] > maxScore && !used_categories[i]) { // If the category hasn't been used yet
            maxScore = computer_current[i];
            maxScoreCat = i; // Choose the best available category
        }
    }
    return maxScoreCat; // If all categories are used (or not good), by default choose chance
}

// Calculate the score for a given category based on the dice values
int calculate_score_for_category(int dice[], int category) {
    switch (category) {
        case ONES: return score_upper_section(dice, 1);
        case TWOS: return score_upper_section(dice, 2);
        case THREES: return score_upper_section(dice, 3);
        case FOURS: return score_upper_section(dice, 4);
        case FIVES: return score_upper_section(dice, 5);
        case SIXES: return score_upper_section(dice, 6);
        case THREE_OF_A_KIND: return score_three_of_a_kind(dice);
        case FOUR_OF_A_KIND: return score_four_of_a_kind(dice);
        case FULL_HOUSE: return score_full_house(dice);
        case SMALL_STRAIGHT: return score_small_straight(dice);
        case LARGE_STRAIGHT: return score_large_straight(dice);
        case YAHTZEE: return score_yahtzee(dice);
        case CHANCE: return score_chance(dice);
        default: return 0; // Invalid category
    }
}

// Finalize the game by displaying the winner
void finalize_round(int human_total, int computer_total) {
    printf("\n--- Final Scores ---\n");
    printf("Human: %d\n", human_total);
    printf("Computer: %d\n", computer_total);

    if (human_total > computer_total) {
        printf("Congratulations! Human player wins!\n");
    } else if (computer_total > human_total) {
        printf("Computer wins! Better luck next time!\n");
    } else {
        printf("It's a tie!\n");
    }
}

// Function to decide if we should attempt Yahtzee or Full House based on dice
bool attempt_yahtzee_or_full_house(int dice[], int reroll[], int used_categories[]) {
    // Check if Yahtzee and Full House are already used
    bool yahtzee_used = used_categories[YAHTZEE];
    bool full_house_used = used_categories[FULL_HOUSE];

    // Count occurrences of each dice face (1 through 6)
    int counts[6] = {0};
    for (int i = 0; i < DICE_COUNT; i++) {
        counts[dice[i] - 1]++;
    }

    // Check for two pairs scenario for Full House
    int pair_count = 0, three_of_a_kind_value = -1;
    for (int i = 0; i < 6; i++) {
        if (counts[i] == 2) pair_count++;
        if (counts[i] == 3) three_of_a_kind_value = i + 1;
    }

    // If there are two pairs and Full House isn't used, try for Full House
    if (pair_count == 2 && !full_house_used) {
        roll_towards_full_house(dice);  // Attempt to get Full House
        return true;  // Indicates that we're trying for Full House
    }

    // If we have a three-of-a-kind or a single pair and Yahtzee isn't used, try for Yahtzee
    for (int i = 0; i < 6; i++) {
        if (counts[i] >= 2 && !yahtzee_used) {  // At least a pair or more
            for (int j = 0; j < DICE_COUNT; j++) {
                reroll[j] = (dice[j] == i + 1) ? 0 : 1;  // Lock dice that match
            }
            return true;  // Indicates that we're trying for Yahtzee
        }
    }

    return false;  // No suitable pair/three-of-a-kind found, or categories already used
}



// Helper function to check if two pairs exist
bool check_for_two_pairs(int dice[]) {
    int count[6] = {0};  // To count occurrences of each dice face (1-6)
    int pair_count = 0;

    // Count how many times each number appears
    for (int i = 0; i < DICE_COUNT; i++) {
        count[dice[i] - 1]++;
    }

    // Find if there are exactly two pairs
    for (int i = 0; i < 6; i++) {
        if (count[i] == 2) {
            pair_count++;
        }
    }

    return pair_count == 2;
}

// Ask if the player wants to reroll
char ask_to_reroll() {
    char reroll_choice;
    printf("Do you want to reroll? (if 'Yes' enter: y || if 'No' enter: n) ==> ");
    scanf(" %c", &reroll_choice); // Space before %c to consume newline
    return reroll_choice;
}

// Ask which dice the player wants to reroll (1 = reroll, 0 = keep)
void ask_which_dice_to_reroll(int reroll[]) {
    printf("Enter 1 to reroll or 0 to keep for each die (separated by space): ");
    for (int i = 0; i < DICE_COUNT; i++) {
        scanf("%d", &reroll[i]);
    }
}


// AI's logic for rerolling dice
void computer_optimal_reroll(int dice[], int used_categories[]) {
    int reroll[DICE_COUNT];
    reset_reroll_array(reroll);

    // First, check for perfect scores that shouldn't be rerolled
    
    // 1. Check for Yahtzee (50 points)
    if (check_for_yahtzee(dice) && !used_categories[YAHTZEE]) {
        return;  // Keep Yahtzee, don't reroll
    }

    // 4.1 Check for Full House (25 points)
    if (check_for_full_house(dice) && !used_categories[FULL_HOUSE]) {
        return;
    }
     
    // 1.1 Try to attempt Yahtzee or Full House based on dice configuration
    if (attempt_yahtzee_or_full_house(dice, reroll, used_categories)) {
        reroll_dice(dice, reroll);
        return;
    }
    
    // 2. Check for Large Straight (40 points)
    if (check_for_large_straight(dice) && !used_categories[LARGE_STRAIGHT]) {
        return;  // Keep Large Straight, don't reroll
    }
    
    // 3. Check for Small Straight (30 points)
    if (check_for_small_straight(dice) && !used_categories[SMALL_STRAIGHT]) {
        return;  // Keep Small Straight, don't reroll
    }
    
    // 4. Check for two pairs (setup for Full House)
    if (check_for_two_pairs(dice) && !used_categories[FULL_HOUSE]) {
        roll_towards_full_house(dice); // Reroll to try and form a Full House
        return;
    }
    
    // 4.1 Check for Full House (25 points)
    if (check_for_full_house(dice) && !used_categories[FULL_HOUSE]) {
        roll_towards_full_house(dice); // Reroll to try and form a Full House
        return;
    }

    // 5. Check for optimal upper section scores
    for (int num = 6; num >= 1; num--) {  // Check from 6's down to 1's
        int count = 0;
        for (int i = 0; i < DICE_COUNT; i++) {
            if (dice[i] == num) count++;
        }
        if (count >= 4 && !used_categories[num-1]) {  // If we have 4 or 5 of the same number
            return;  // Keep this excellent upper section score
        }
    }

    // If no perfect scores, proceed with regular reroll strategy
    if (check_for_four_of_a_kind(dice) && !used_categories[FOUR_OF_A_KIND]) {
        int target = find_most_frequent_value(dice);
        lock_matching_dice(dice, reroll, target, 4);
    }
    else if (check_for_three_of_a_kind(dice) && !used_categories[THREE_OF_A_KIND]) {
        int target = find_most_frequent_value(dice);
        lock_matching_dice(dice, reroll, target, 3);
    }
    // Use reroll_non_matching_dice() if attempting to form a straight but we don't have one yet
    else if (!used_categories[LARGE_STRAIGHT] && !check_for_large_straight(dice)) {
        reroll_non_matching_dice(dice, reroll); // Try to reroll for a Large Straight
    }
    else if (!used_categories[SMALL_STRAIGHT] && !check_for_small_straight(dice)) {
        reroll_non_matching_dice(dice, reroll); // Try to reroll for a Small Straight
    }
    else if (!used_categories[CHANCE]) {
        lock_high_value_dice(dice, reroll);
    }
    else {
        reroll_for_optimal_number(dice, reroll);
    }

    // Only reroll if we haven't returned earlier due to a perfect score
    reroll_dice(dice, reroll);
}


void reset_reroll_array(int reroll[]) {
    for (int i = 0; i < DICE_COUNT; i++) {
        reroll[i] = 0; // Set all dice to unlocked (0)
    }
}

// Function to check if there's a Yahtzee (all five dice are the same)
bool check_for_yahtzee(int dice[]) {
    for (int i = 1; i < DICE_COUNT; i++) {
        if (dice[i] != dice[0]) {
            return false;  // If any dice is different, not a Yahtzee
        }
    }
    return true;  // All dice are the same, it's a Yahtzee
}

// Function to check if there's a Full House (three of one number, two of another)
bool check_for_full_house(int dice[]) {
    int freq[7] = {0};  // To count occurrences of each dice face (1-6)

    // Count how many times each number appears
    for (int i = 0; i < DICE_COUNT; i++) {
        freq[dice[i]]++;
    }

    bool has_three_of_a_kind = false;
    bool has_pair = false;

    // Check if we have both a triplet and a pair
    for (int i = 1; i <= 6; i++) {
        if (freq[i] == 3) {
            has_three_of_a_kind = true;
        }
        if (freq[i] == 2) {
            has_pair = true;
        }
    }

    return has_three_of_a_kind && has_pair;  // Full house if both conditions are met
}


// Check if there are at least three of the same value in the dice
int check_for_three_of_a_kind(int dice[]) {
    int freq[7] = {0}; // Frequency array for dice values (1-6)
    for (int i = 0; i < DICE_COUNT; i++) {
        freq[dice[i]]++;
    }
    for (int i = 1; i <= 6; i++) {
        if (freq[i] >= 3) {
            return 1; // At least three of a kind
        }
    }
    return 0; // No three of a kind
}

// Check if there are at least four of the same value in the dice
int check_for_four_of_a_kind(int dice[]) {
    int freq[7] = {0}; // Frequency array for dice values (1-6)
    for (int i = 0; i < DICE_COUNT; i++) {
        freq[dice[i]]++;
    }
    for (int i = 1; i <= 6; i++) {
        if (freq[i] >= 4) {
            return 1; // At least four of a kind
        }
    }
    return 0; // No four of a kind
}

// Check if there is a small straight (four consecutive values)
int check_for_small_straight(int dice[]) {
    qsort(dice, DICE_COUNT, sizeof(int), compare);
    if ((dice[0] == dice[1] - 1 && dice[1] == dice[2] - 1 && dice[2] == dice[3] - 1) ||
        (dice[1] == dice[2] - 1 && dice[2] == dice[3] - 1 && dice[3] == dice[4] - 1) ||
        (dice[0] == dice[1] - 1 && dice[1] == dice[2] - 1 && dice[2] == dice[3] - 1 && dice[3] == dice[4] - 1)) {
        return 1; // Small straight found
    }
    return 0; // No small straight
}

// Check if there is a large straight (five consecutive values)
int check_for_large_straight(int dice[]) {
    qsort(dice, DICE_COUNT, sizeof(int), compare);
    for (int i = 1; i < DICE_COUNT; i++) {
        if (dice[i] != dice[i-1] + 1) {
            return 0; // Not a large straight
        }
    }
    return 1; // Large straight found
}

// AI logic for going towards Ones, Twos, Threes, etc.
void computer_optimal_low_category(int dice[], int reroll[], int category_value) {
    // Find the most common number from the first roll
    int most_common = find_most_common_dice(dice);

    // If no dice match the category_value on the first roll, reroll all
    bool has_matching_dice = false;

    for (int i = 0; i < DICE_COUNT; i++) {
        if (dice[i] == category_value) {
            has_matching_dice = true;  // We found at least one matching dice
            reroll[i] = 0;  // Lock the dice that match the category_value
        } else {
            reroll[i] = 1;  // Mark other dice for reroll
        }
    }

    // If no dice match the target number, reroll all
    if (!has_matching_dice) {
        for (int i = 0; i < DICE_COUNT; i++) {
            reroll[i] = 1;  // Reroll all dice
        }
    }
}

// Helper function to find the most frequent dice value
int find_most_common_dice(int dice[]) {
    int freq[7] = {0};  // Frequency array to count occurrences of each dice face (1-6)

    // Count how many times each number appears
    for (int i = 0; i < DICE_COUNT; i++) {
        freq[dice[i]]++;
    }

    // Find the number with the highest frequency
    int most_common_value = 1;
    int max_count = freq[1];  // Start by assuming the first number is the most common

    for (int i = 2; i <= 6; i++) {
        if (freq[i] > max_count) {
            max_count = freq[i];
            most_common_value = i;
        }
    }

    return most_common_value;
}

//This function will return the dice value that appears most frequently.
int find_most_frequent_value(int dice[]) {
    int freq[7] = {0};
    for (int i = 0; i < DICE_COUNT; i++) {
        freq[dice[i]]++;
    }

    int max_count = 0, most_frequent_value = 0;
    for (int i = 1; i <= 6; i++) {
        if (freq[i] > max_count) {
            max_count = freq[i];
            most_frequent_value = i;
        }
    }
    return most_frequent_value;
}

//This function locks a given number of dice with the same value.
void lock_matching_dice(int dice[], int reroll[], int value, int count) {
    int locked_count = 0;
    for (int i = 0; i < DICE_COUNT; i++) {
        if (dice[i] == value && locked_count < count) {
            reroll[i] = 0; // Lock the die
            locked_count++;
        } else {
            reroll[i] = 1; // Mark for reroll
        }
    }
}


// Reroll dice that do not match the target numbers (used for forming straights)
void reroll_non_matching_dice(int dice[], int reroll[]) {
    qsort(dice, DICE_COUNT, sizeof(int), compare); // Sort the dice for easier comparison

    // Try to reroll any dice that do not contribute to forming the straight
    for (int i = 0; i < DICE_COUNT - 1; i++) {
        if (dice[i + 1] != dice[i] + 1) {
            reroll[i + 1] = 1; // Mark the die for reroll if it breaks the consecutive sequence
        }
    }
}

int main() {
    srand(time(0)); // Initialize random seed

    // Initialize arrays to store human and computer scores and used categories
    int human_scores[13], computer_scores[13], human_used[13], computer_used[13];
    
    // Initialize scores and used category arrays
    initialize_scores(human_scores, human_used);
    initialize_scores(computer_scores, computer_used);

    int dice[DICE_COUNT]; // Array to store dice values
    int round, player_choice, computer_choice;
    int reroll[5];
    
    // Declare human and computer total score variables
    int human_total = 0, computer_total = 0;

    // Main game loop, playing 13 rounds
    for (round = 0; round < 13; round++) {
        reset_reroll_array(reroll);
        printf("----------- Round %d -----------\n", round + 1);

        // Human player's turn
        printf("\033[1;33m\nHuman's turn:\n\033[0m");
        roll_dice(dice);
        print_dice(dice); // Show initial dice values

        // Allow human to reroll up to ROLLS - 1 times
        for (int i = 1; i < ROLLS; i++) {
            if (ask_to_reroll() == 'y') { // Ask if the player wants to reroll
                ask_which_dice_to_reroll(reroll); // Ask which dice to reroll
                reroll_dice(dice, reroll); // Reroll the selected dice
                print_dice(dice); // Show new dice values
            } else {
                break; // If the player chooses not to reroll, break the loop
            }
        }

        // Human chooses category to score
        while (1) {
            printf("\nPlease choose a category to score:\n");
            printf(" 1. ONES\t\t2. TWOS\t\t3. THREES\t4. FOURS\n 5. FIVES\t\t6. SIXES\t7. THREE OF A KIND\t8. FOUR OF A KIND\n 9. FULL HOUSE\t10. SMALL STRAIGHT\t11. LARGE STRAIGHT\t12. YAHTZEE\t13. CHANCE\n");
            printf("Enter the chosen type Number: ");
            scanf("%d", &player_choice);
            
            player_choice--; // Convert to zero-based index

            // Validate player's choice
            if (player_choice >= 0 && player_choice < 13 && !human_used[player_choice]) {
                human_used[player_choice] = 1; // Mark the category as used
                human_scores[player_choice] = calculate_score_for_category(dice, player_choice); // Calculate score
                break; // Exit the loop after a valid choice
            } else {
                printf("Invalid choice or category already used. Please choose again.\n");
            }
        }

        // Computer player's turn
        printf("\033[1;33m\nComputer's turn:\n\033[0m");
        roll_dice(dice); // Computer rolls dice
        score_large_straight(dice);
        print_dice(dice); // Show dice rolled

        // Computer rerolls intelligently based on current game state
        for (int i = 1; i < ROLLS; i++) {
            computer_optimal_reroll(dice, computer_used); // AI's reroll decision
            print_dice(dice); // Show new dice values after reroll
        }

        // Computer chooses category to score
        computer_choice = computer_choose_category(dice, computer_used);
        computer_used[computer_choice] = 1; // Mark category as used
        computer_scores[computer_choice] = calculate_score_for_category(dice, computer_choice);

        // Calculate total scores for display
        human_total = 0;
        computer_total = 0;
        for (int i = 0; i < 13; i++) {
            human_total += human_scores[i];
            computer_total += computer_scores[i];
        }

        // Display score after the round
        display_scores(human_scores, computer_scores, human_total, computer_total);
        display_score_table(human_scores, computer_scores, human_total, computer_total);
    }

    // Finalize the game and display the winner
    finalize_round(human_total, computer_total);

    return 0;
}

