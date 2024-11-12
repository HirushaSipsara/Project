#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define NUM_DICE 5
#define NUM_ROLLS 3
#define NUM_ROUNDS 13
#define NUM_CATEGORIES 13

// Function prototypes
void initializeGame();
void rollDice(int dice[], bool keep[]);
void displayDice(const int dice[]);
void resetKeepArray(bool keep[]);
int calculateScore(int dice[], int category);
void humanPlay(int dice[], bool keep[]);
void computerPlay(int dice[], bool keep[], int categoriesUsed[]);
int chooseCategory(int categoriesUsed[]);
int bestCategory(int dice[], int categoriesUsed[]);
bool isCategoryUsed(int category, int categoriesUsed[]);
void displayScores(int humanScore, int computerScore);
void showCategoryOptions();
void displayFinalScores(int humanScore, int computerScore);
void explainRules();
void displayMenu();
void displayTurnInfo(int round, int player);
void sortDice(int dice[]);
int calculateUpperSection(int dice[], int targetValue);
bool isThreeOfAKind(int dice[]);
bool isFourOfAKind(int dice[]);
bool isFullHouse(int dice[]);
bool isSmallStraight(int dice[]);
bool isLargeStraight(int dice[]);
bool isYahtzee(int dice[]);
int sumOfDice(int dice[]);
void playYahtzee();
void displayGameHeader();

// Global variables for the game
int humanScore = 0, computerScore = 0;
int humanCategoriesUsed[NUM_CATEGORIES] = {0};
int computerCategoriesUsed[NUM_CATEGORIES] = {0};

// Main function
int main() {
    // Seed random number generator
    srand(time(NULL));
    
    displayGameHeader();
    
    explainRules();
    
    // Main menu loop
    while (1) {
        displayMenu();
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                playYahtzee();
                break;
            case 2:
                explainRules();
                break;
            case 3:
                printf("Exiting game. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}

// Function to explain the rules of the game
void explainRules() {
    printf("\n--- Yahtzee Game Rules ---\n");
    printf("1. The game consists of 13 rounds.\n");
    printf("2. You will roll five dice up to three times per round.\n");
    printf("3. You will choose a category to score your roll in each round.\n");
    printf("4. Categories include:\n");
    printf("   Ones, Twos, Threes, Fours, Fives, Sixes\n");
    printf("   Three of a Kind, Four of a Kind, Full House, Small Straight,\n");
    printf("   Large Straight, Yahtzee, Chance\n");
    printf("5. The player with the highest score after 13 rounds wins!\n");
}

// Function to display the game header
void displayGameHeader() {
    printf("===================================\n");
    printf("           Welcome to Yahtzee!     \n");
    printf("===================================\n");
}

// Function to display the main menu
void displayMenu() {
    printf("\n--- Main Menu ---\n");
    printf("1. Play Yahtzee\n");
    printf("2. View Rules\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

// Function to start the game
void playYahtzee() {
    // Initialize scores and category usage tracking
    humanScore = 0;
    computerScore = 0;
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        humanCategoriesUsed[i] = 0;
        computerCategoriesUsed[i] = 0;
    }
    
    int dice[NUM_DICE];
    bool keep[NUM_DICE] = {false};  // Track which dice to keep
    
    // Play 13 rounds
    for (int round = 1; round <= NUM_ROUNDS; round++) {
        printf("\n--- Round %d ---\n", round);
        
        // Human's turn
        displayTurnInfo(round, 1);  // 1 for human
        humanPlay(dice, keep);
        int humanCategory = chooseCategory(humanCategoriesUsed);
        humanScore += calculateScore(dice, humanCategory);
        
        // Computer's turn
        displayTurnInfo(round, 2);  // 2 for computer
        computerPlay(dice, keep, computerCategoriesUsed);
        int computerCategory = bestCategory(dice, computerCategoriesUsed);
        computerScore += calculateScore(dice, computerCategory);
        
        // Display scores after each round
        displayScores(humanScore, computerScore);
    }
    
    // Display final scores and winner
    displayFinalScores(humanScore, computerScore);
}

// Function to roll dice
void rollDice(int dice[], bool keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (!keep[i]) {
            dice[i] = (rand() % 6) + 1;  // Roll a new number if the dice is not kept
        }
    }
}

// Function to display the current dice values
void displayDice(const int dice[]) {
    printf("Dice: ");
    for (int i = 0; i < NUM_DICE; i++) {
        printf("%d ", dice[i]);
    }
    printf("\n");
}

// Function to reset the keep array (all dice will be re-rolled)
void resetKeepArray(bool keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        keep[i] = false;
    }
}

// Function to display the turn info
void displayTurnInfo(int round, int player) {
    if (player == 1) {
        printf("\nHuman's turn - Round %d\n", round);
    } else {
        printf("\nComputer's turn - Round %d\n", round);
    }
}

// Function to calculate score based on category
int calculateScore(int dice[], int category) {
    int score = 0;
    switch (category) {
        case 1: score = calculateUpperSection(dice, 1); break;  // Ones
        case 2: score = calculateUpperSection(dice, 2); break;  // Twos
        case 3: score = calculateUpperSection(dice, 3); break;  // Threes
        case 4: score = calculateUpperSection(dice, 4); break;  // Fours
        case 5: score = calculateUpperSection(dice, 5); break;  // Fives
        case 6: score = calculateUpperSection(dice, 6); break;  // Sixes
        case 7: score = isThreeOfAKind(dice) ? sumOfDice(dice) : 0; break;
        case 8: score = isFourOfAKind(dice) ? sumOfDice(dice) : 0; break;
        case 9: score = isFullHouse(dice) ? 25 : 0; break;
        case 10: score = isSmallStraight(dice) ? 30 : 0; break;
        case 11: score = isLargeStraight(dice) ? 40 : 0; break;
        case 12: score = isYahtzee(dice) ? 50 : 0; break;
        case 13: score = sumOfDice(dice); break;  // Chance
        default: score = 0; break;
    }
    return score;
}

// Function to calculate score for upper section (Ones, Twos, etc.)
int calculateUpperSection(int dice[], int targetValue) {
    int score = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        if (dice[i] == targetValue) {
            score += targetValue;
        }
    }
    return score;
}

// Function to simulate human player's turn
void humanPlay(int dice[], bool keep[]) {
    resetKeepArray(keep);  // Reset keep array at the start of each turn
    
    for (int i = 0; i < NUM_ROLLS; i++) {
        rollDice(dice, keep);
        displayDice(dice);
        
        if (i < NUM_ROLLS - 1) {
            printf("Enter 1 to keep the dice, 0 to re-roll:\n");
            for (int j = 0; j < NUM_DICE; j++) {
                printf("Keep dice %d? ", j + 1);
                scanf("%d", (int*)&keep[j]);  // Cast to (int*) to store into bool
            }
        }
    }
}

// Function to simulate computer's turn
void computerPlay(int dice[], bool keep[], int categoriesUsed[]) {
    resetKeepArray(keep);  // Reset keep array at the start of each turn
    
    for (int i = 0; i < NUM_ROLLS; i++) {
        rollDice(dice, keep);
        displayDice(dice);
        
        if (i < NUM_ROLLS - 1) {
            // Simple AI logic: try to maximize dice for best category
            // Here, we could add more sophisticated strategies
            // (Placeholder for AI strategy)
        }
    }
}

// Function for human to choose a scoring category
int chooseCategory(int categoriesUsed[]) {
    int category;
    do {
        printf("Choose a category to score (1-13): ");
        scanf("%d", &category);
    } while (isCategoryUsed(category, categoriesUsed));
    categoriesUsed[category - 1] = 1;  // Mark category as used
    return category;
}

// Function to determine the best category for the computer
int bestCategory(int dice[], int categoriesUsed[]) {
    int bestCategory = -1;
    int maxScore = -1;
    
    // Evaluate each category and select the one with the highest score
    for (int category = 1; category <= NUM_CATEGORIES; category++) {
        if (!isCategoryUsed(category, categoriesUsed)) {
            int score = calculateScore(dice, category);
            if (score > maxScore) {
                maxScore = score;
                bestCategory = category;
            }
        }
    }
    categoriesUsed[bestCategory - 1] = 1;  // Mark category as used
    return bestCategory;
}

// Function to check if a category has been used
bool isCategoryUsed(int category, int categoriesUsed[]) {
    return categoriesUsed[category - 1];
}

// Function to display current scores
void displayScores(int humanScore, int computerScore) {
    printf("Current Scores - Human: %d, Computer: %d\n", humanScore, computerScore);
}

// Function to display final scores and determine the winner
void displayFinalScores(int humanScore, int computerScore) {
    printf("\n--- Final Scores ---\n");
    printf("Human: %d, Computer: %d\n", humanScore, computerScore);
    if (humanScore > computerScore) {
        printf("Congratulations! You win!\n");
    } else if (humanScore < computerScore) {
        printf("Computer wins! Better luck next time!\n");
    } else {
        printf("It's a tie!\n");
    }
}

// Utility functions to calculate specific categories
bool isThreeOfAKind(int dice[]) {
    int counts[6] = {0};  // To count occurrences of each face
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i] >= 3) return true;
    }
    return false;
}

bool isFourOfAKind(int dice[]) {
    int counts[6] = {0};  // To count occurrences of each face
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i] >= 4) return true;
    }
    return false;
}

bool isFullHouse(int dice[]) {
    int counts[6] = {0};  // To count occurrences of each face
    bool twoFound = false, threeFound = false;
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i] == 2) twoFound = true;
        if (counts[i] == 3) threeFound = true;
    }
    return (twoFound && threeFound);
}

bool isSmallStraight(int dice[]) {
    sortDice(dice);
    int uniqueCount = 1;
    for (int i = 1; i < NUM_DICE; i++) {
        if (dice[i] != dice[i - 1]) {
            uniqueCount++;
        }
    }
    return (uniqueCount >= 4);  // Small straight requires at least 4 unique consecutive numbers
}

bool isLargeStraight(int dice[]) {
    sortDice(dice);
    int uniqueCount = 1;
    for (int i = 1; i < NUM_DICE; i++) {
        if (dice[i] != dice[i - 1]) {
            uniqueCount++;
        }
    }
    return (uniqueCount == 5);  // Large straight requires all 5 unique consecutive numbers
}

bool isYahtzee(int dice[]) {
    for (int i = 1; i < NUM_DICE; i++) {
        if (dice[i] != dice[0]) return false;
    }
    return true;
}

int sumOfDice(int dice[]) {
    int sum = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        sum += dice[i];
    }
    return sum;
}

// Function to sort the dice array
void sortDice(int dice[]) {
    for (int i = 0; i < NUM_DICE - 1; i++) {
        for (int j = 0; j < NUM_DICE - i - 1; j++) {
            if (dice[j] > dice[j + 1]) {
                int temp = dice[j];
                dice[j] = dice[j + 1];
                dice[j + 1] = temp;
            }
        }
    }
}
