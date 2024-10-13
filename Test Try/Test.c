#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define NUMDICE 5
#define NUMROUNDS 13
#define CATEGORIES 13
#define MAX_ROLLS 3

// Function declarations
void rollDice(int dice[]);
void displayDice(int dice[]);
void playerTurn(int dice[]);
void computer_turn(int dice[], int *computerScore, int computer_scorecard[]);
int scoreOnes(int dice[]);
int scoreTwos(int dice[]);
int scoreThrees(int dice[]);
int scoreFours(int dice[]);
int scoreFives(int dice[]);
int scoreSixes(int dice[]);
int scoreThreeOfKind(int dice[]);
int scoreFourOfKind(int dice[]);
int scoreFullHouse(int dice[]);
int scoreSmallStraight(int dice[]);
int scoreLargeStraight(int dice[]);
int scoreChance(int dice[NUMDICE]);
int scoreYahtzee(int dice[NUMDICE]);
void gameRound(int round, int *humanScore, int *computerScore, int computer_scorecard[CATEGORIES]);
int calculate_score(int dice[], int category);
void evaluate_dice(int dice[], int keep[]);
int choose_best_category(int dice[], int computer_scorecard[]);

int main() {
    srand(time(0));  // Seed random number generator

    int humanScore = 0, computerScore = 0;
    int used_categories[CATEGORIES] = {0};
    int keep[NUMDICE] = {0};
    int total_score = 0;
   
    int computer_scorecard[NUMROUNDS]={0};
    int dice[NUMDICE] = {0};

    printf("Welcome to Yahtzee!\n");

    // Game loop for 13 rounds
    for (int round = 1; round <= NUMROUNDS; round++) {
        printf("\nRound %d\n", round);
        gameRound(round, &humanScore, &computerScore, computer_scorecard);
        printf("After round %d: Human: %d, Computer: %d\n", round, humanScore, computerScore);
    }

    // Determine the winner
    if (humanScore > computerScore) {
        printf("\nCongratulations! You win with a score of %d to %d!\n", humanScore, computerScore);
    } else if (computerScore > humanScore) {
        printf("\nThe computer wins with a score of %d to %d!\n", computerScore, humanScore);
    } else {
        printf("\nIt's a tie with both scoring %d!\n", humanScore);
    }

    return 0;
}void rollDice(int dice[]) {
    for (int i = 0; i < NUMDICE; i++) {
        dice[i] = (rand() % 6) + 1;  // Roll each die (values 1-6)
    }
}

// Display the values of the dice
void displayDice(int dice[]) {
    printf("Dice: ");
    for (int i = 0; i < NUMDICE; i++) {
        printf("%d ", dice[i]);
    }
    printf("\n");
}



// Function for human player's turn
void playerTurn(int dice[]) {
    char reroll;
    int keep[NUMDICE] = {0}; // Array to mark dice the player wants to keep (0 for re-roll, 1 for keep)
    char input[40]; // Buffer to store user input for dice numbers

    rollDice(dice); // First roll happens here
    printf("Your turn!\n");
    displayDice(dice);

    //  re-roll for to 2 times
    for (int roll = 0; roll < 2; roll++) {
        printf("Do you want to keep any dice? (y/n): ");
        scanf(" %c", &reroll);
        
        if (reroll == 'y') {
            // Ask which dice to keep
            printf("Enter the dice numbers (1-5) you want to keep, separated by commas (or enter 0 to re-roll all): ");
            scanf("%s", input);  // Get the input as a string (e.g., "1,3,5")
            
            // comma-separated dice numbers
            char *token = strtok(input, ",");
            while (token != NULL) {
                int DieNum = atoi(token); // Convert token to an integer
                if (DieNum >= 1 && DieNum <= 5) {
                    keep[DieNum - 1] = 1; // Mark dice to keep (dice numbers are 1-5)
                }
                token = strtok(NULL, ","); // Get the next token
            }
        }

        // Ask if the player wants to re-roll any dice
        printf("Do you want to re-roll any remaining dice? (y/n): ");
        scanf(" %c", &reroll);
        if (reroll == 'y') {
            // Re-roll dice that are not kept
            for (int i = 0; i < NUMDICE; i++) {
                if (keep[i] == 0) {
                    dice[i] = (rand() % 6) + 1; // Re-roll this die
                }
            }
            displayDice(dice); // Display the new dice after re-rolling
        } else {
            break; // If the player doesn't want to re-roll, end their turn
        }
    }
}
void roll_dice(int dice[NUMDICE], int keep[NUMDICE]) {
    for (int i = 0; i < NUMDICE; i++) {
        if (!keep[i]) {
            dice[i] = rand() % 6 + 1; // Random number between 1 and 6
        }
    }
}

// Function for computer's turn (simple random strategy for now)

// Scoring category: Ones
int scoreOnes(int dice[]) {
    int score = 0;
    for (int i = 0; i < NUMDICE; i++) {
        if (dice[i] == 1) {
            score += 1;
        }
    }
    return score;
}
int scoreTwos(int dice[]) {
    int score = 0;
    for (int i = 0; i < NUMDICE; i++) {
        if (dice[i] == 2) {
            score += 2;
        }
    }
    return score;
}
int scoreThrees(int dice[]) {
    int score = 0;
    for (int i = 0; i < NUMDICE; i++) {
        if (dice[i] == 3) {
            score += 3;
        }
    }
    return score;
}
int scoreFours(int dice[]) {
    int score = 0;
    for (int i = 0; i < NUMDICE; i++) {
        if (dice[i] == 4) {
            score += 4;
        }
    }
    return score;
}
int scoreFives(int dice[]) {
    int score = 0;
    for (int i = 0; i < NUMDICE; i++) {
        if (dice[i] == 5) {
            score += 5;
        }
    }
    return score;
}
int scoreSixes(int dice[]) {
    int score = 0;
    for (int i = 0; i < NUMDICE; i++) {
        if (dice[i] == 6) {
            score += 6;
        }
    }
    return score;
}

// Scoring category: Three of a Kind
int scoreThreeOfKind(int dice[]) {
    int counts[6] = {0};  // Array to count occurrences of each die value (1-6)

    // Count each die's value
    for (int i = 0; i < NUMDICE; i++) {
        counts[dice[i] - 1]++;
    }

    // Check for Three of a Kind
    for (int i = 0; i < 6; i++) {
        if (counts[i] >= 3) {
            // If Three of a Kind, return sum of all dice
            int sum = 0;
            for (int j = 0; j < NUMDICE; j++) {
                sum += dice[j];
            }
            return sum;
        }
    }
    return 0;  // No Three of a Kind
}
int scoreFourOfKind(int dice[]) {
    int counts[6] = {0};  // Array to count occurrences of each die value (1-6)

    // Count each die's value
    for (int i = 0; i < NUMDICE; i++) {
        counts[dice[i] - 1]++;
    }

    // Check for 4 of a Kind
    for (int i = 0; i < 6; i++) {
        if (counts[i] >= 4) {
            // If 4 of a Kind, return sum of all dice
            int sum = 0;
            for (int j = 0; j < NUMDICE; j++) {
                sum += dice[j];
            }
            return sum;
        }
    }
    return 0;  // No 4 of a Kind
}
int scoreFullHouse(int dice[]) {
    int counts[6] = {0};  

    for (int i = 0; i < NUMDICE; i++) {
        counts[dice[i] - 1]++;
    }

    // to chech full house: one value shows 3 times, another shows 2 times
    int threeOfKind = 0, twoOfKind = 0;

    for (int i = 0; i < 6; i++) {
        if (counts[i] == 3) {
            threeOfKind = 1;  // Found a set of 3
        }
        if (counts[i] == 2) {
            twoOfKind = 1;  // Found a set of 2
        }
    }

    // If both scores are found, return 25 points for Full House
    if (threeOfKind && twoOfKind) {
        return 25;  // Full House is worth 25 points
    }

    return 0;  // No Full House
}

int scoreSmallStraight(int dice[]) {
    int sortedDice[NUMDICE];
    int uniqueDice[NUMDICE];
    int uniqueCount = 0;
    
    // first step is to Copy dice to a temporary array
    for (int i = 0; i < NUMDICE; i++) {
        sortedDice[i] = dice[i];
    }

    // this step Sort the dice array
    for (int i = 0; i < NUMDICE-1; i++) {
        for (int j = i+1; j < NUMDICE; j++) {
            if (sortedDice[i] > sortedDice[j]) {
                int temp = sortedDice[i];
                sortedDice[i] = sortedDice[j];
                sortedDice[j] = temp;
            }
        
            }
    }

    // this step Remove duplicates
    for (int i = 0; i < NUMDICE; i++) {
        if (i == 0 || sortedDice[i] != sortedDice[i-1]) {
            uniqueDice[uniqueCount++] = sortedDice[i];
        }
    }

    // This step Check for consecutive sequences in the uniqueDice array

    if (uniqueCount >= 4) {
        for (int i = 0; i <= uniqueCount - 4; i++) {
            if (uniqueDice[i] == uniqueDice[i+1] - 1 &&
                uniqueDice[i] == uniqueDice[i+2] - 2 &&
                uniqueDice[i] == uniqueDice[i+3] - 3) {
                return 30;  // Small Straight is worth 30 points
            }
        }
    }

    return 0;  // No Small Straight
}
int scoreLargeStraight(int dice[]) {
    int sortedDice[NUMDICE];
    int uniqueDice[NUMDICE];
    int uniqueCount = 0;

    // first step is to copy dice to a temporary array
    for (int i = 0; i < NUMDICE; i++) {
        sortedDice[i] = dice[i];
    }

    // Sort the dice array to sort the values we got from dices
    for (int i = 0; i < NUMDICE - 1; i++) {
        for (int j = i + 1; j < NUMDICE; j++) {
            if (sortedDice[i] > sortedDice[j]) {
                int temp = sortedDice[i];
                sortedDice[i] = sortedDice[j];
                sortedDice[j] = temp;
            }
        }
    }

    // in hear remove duplicates 
    for (int i = 0; i < NUMDICE; i++) {
        if (i == 0 || sortedDice[i] != sortedDice[i - 1]) {
            uniqueDice[uniqueCount++] = sortedDice[i];
        }
    }

    // in this step we check for Large Straight
    if (uniqueCount == 5) {
        if ((uniqueDice[0] == 1 && uniqueDice[1] == 2 && uniqueDice[2] == 3 &&
             uniqueDice[3] == 4 && uniqueDice[4] == 5) ||
            (uniqueDice[0] == 2 && uniqueDice[1] == 3 && uniqueDice[2] == 4 &&
             uniqueDice[3] == 5 && uniqueDice[4] == 6)) {
            return 40;  // Large Straight is worth 40 points
        }
    }

    return 0;  // No Large Straight
}
int scoreChance(int dice[NUMDICE]) {
    int score = 0;
    for (int i = 0; i < NUMDICE; i++) {
        score += dice[i]; // The score is simply the sum of the dice
    }
    return score;
}

int scoreYahtzee(int dice[NUMDICE]) {
    // Check if all dices have the same values
    for (int i = 1; i < NUMDICE; i++) {
        if (dice[i] != dice[0]) {
            return 0; // Not a Yahtzee
        }
    }
    return 50; // Yahtzee score
}


// Function to play one round of the game
void gameRound(int round, int *humanScore, int *computerScore, int computer_scorecard[CATEGORIES]) {
    int dice[NUMDICE];

    // Player turn
    playerTurn(dice);
    int humanRoundScore = 0;

    // Array to track which categories have been used
    static bool used[CATEGORIES] = {false};

    // Display scoring options to the user
    printf("Choose your category: [ 1) Ones:%d , 2) Twos:%d , 3) Threes:%d , 4) Fours:%d , 5) Fives:%d , 6) Sixes:%d , 7) Three of a kind:%d , 8) Four of kind:%d , 9) Full House:%d , 10) Small Straight:%d , 11) Large Straight:%d , 12) Chance:%d , 13) Yahtzee:%d ]\n", 
           scoreOnes(dice), scoreTwos(dice), scoreThrees(dice), scoreFours(dice), 
           scoreFives(dice), scoreSixes(dice), scoreThreeOfKind(dice), 
           scoreFourOfKind(dice), scoreFullHouse(dice), scoreSmallStraight(dice), 
           scoreLargeStraight(dice), scoreChance(dice), scoreYahtzee(dice));

    int choices;

    // this Loop run until you enter a valid category 
 while (true) {
        printf("Enter your choice (1-13): ");
        scanf("%d", &choices);

        // Validate choice and update scores
        if (choices >= 1 && choices <= 13) {
            if (used[choices - 1]) {
                printf("Category %d is already used. Please choose another one.\n", choices);
            } else {
                // Mark the chosen category as used
                used[choices - 1] = true;

                // Calculate score based on chosen category
                switch (choices) {
                    case 1:
                        humanRoundScore = scoreOnes(dice);
                        break;
                    case 2:
                        humanRoundScore = scoreTwos(dice);
                        break;
                    case 3:
                        humanRoundScore = scoreThrees(dice);
                        break;
                    case 4:
                        humanRoundScore = scoreFours(dice);
                        break;
                    case 5:
                        humanRoundScore = scoreFives(dice);
                        break;
                    case 6:
                        humanRoundScore = scoreSixes(dice);
                        break;
                    case 7:
                        humanRoundScore = scoreThreeOfKind(dice);
                        break;
                    case 8:
                        humanRoundScore = scoreFourOfKind(dice);
                        break;
                    case 9:
                        humanRoundScore = scoreFullHouse(dice);
                        break;
                    case 10:
                        humanRoundScore = scoreSmallStraight(dice);
                        break;
                    case 11:
                        humanRoundScore = scoreLargeStraight(dice);
                        break;
                    case 12:
                        humanRoundScore = scoreChance(dice);
                        break;
                    case 13:
                        humanRoundScore = scoreYahtzee(dice);
                        break;
                }

                // Add to total human score
                *humanScore += humanRoundScore;
                printf("You scored %d points in the chosen category.\n", humanRoundScore);
                break;  // Exit the loop after a valid choice
        }
        } else {
            printf("Invalid category! Please choose a number between 1 and 13.\n");
        }

        // Re-display available categories after an invalid choice
        printf("Available categories: [ 1) Ones:%d , 2) Twos:%d , 3) Threes:%d , 4) Fours:%d , 5) Fives:%d , 6) Sixes:%d , 7) Three of a kind:%d , 8) Four of kind:%d , 9) Full House:%d , 10) Small Straight:%d , 11) Large Straight:%d , 12) Chance:%d , 13) Yahtzee:%d ]\n",
            scoreOnes(dice), scoreTwos(dice), scoreThrees(dice), scoreFours(dice), 
            scoreFives(dice), scoreSixes(dice), scoreThreeOfKind(dice), 
            scoreFourOfKind(dice), scoreFullHouse(dice), scoreSmallStraight(dice), 
            scoreLargeStraight(dice), scoreChance(dice), scoreYahtzee(dice));
    }
    
     // Add to total human score
    *humanScore = humanRoundScore;
    computer_turn(dice, computerScore, computer_scorecard);
}

void computer_turn(int dice[], int *computerScore, int computer_scorecard[]) {
    int roll_count = 0;
    int keep_dices[NUMDICE] = {0};  // The computer decides which dice to keep

    // Simulate the computer's rolling and decision-making process
    while (roll_count < MAX_ROLLS) {
        roll_dice(dice, keep_dices);
        displayDice(dice);
        roll_count++;

        if (roll_count < MAX_ROLLS) {
            evaluate_dice(dice, keep_dices);  // Basic strategy to keep useful dice
        }
    }

    // Computer chooses the best category based on its strategy
    int best_category = choose_best_category(dice, computer_scorecard);
    
    // Calculate the score for the chosen category
    int round_score = calculate_score(dice, best_category);
    
    // Add round score to the total computer score
    *computerScore += round_score;

    // Mark the category as used
    computer_scorecard[best_category] = 1;

    printf("Computer chose category %d and scored %d points.\n", best_category+1, round_score);
}

void evaluate_dice(int dice[], int keep[]) {
    int ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixes = 0;

    // Count occurrences of each dice value
    for (int i = 0; i < NUMDICE; i++) {
        switch (dice[i]) {
            case 1: ones++; break;
            case 2: twos++; break;
            case 3: threes++; break;
            case 4: fours++; break;
            case 5: fives++; break;
            case 6: sixes++; break;
        }
    }

    // Reset the keep array to 0 (don't keep any dice initially)
    for (int i = 0; i < NUMDICE; i++) {
        keep[i] = 0;
    }

    // Yahtzee (five of the same number)
    if (ones == 5 || twos == 5 || threes == 5 || fours == 5 || fives == 5 || sixes == 5) {
        for (int i = 0; i < NUMDICE; i++) {
            keep[i] = 1;  // Keep all dice for Yahtzee
        }
        return;
    }

    // Three-of-a-kind
    if (ones >= 3 || twos >= 3 || threes >= 3 || fours >= 3 || fives >= 3 || sixes >= 3) {
        int value_to_keep = (ones >= 3) ? 1 : (twos >= 3) ? 2 : (threes >= 3) ? 3 :
                            (fours >= 3) ? 4 : (fives >= 3) ? 5 : 6;
        for (int i = 0; i < NUMDICE; i++) {
            if (dice[i] == value_to_keep) {
                keep[i] = 1;  // Keep all dice that match the value
            }
        }
        return;
    }

    // Four-of-a-kind
    if (ones >= 4 || twos >= 4 || threes >= 4 || fours >= 4 || fives >= 4 || sixes >= 4) {
        int value_to_keep = (ones >= 4) ? 1 : (twos >= 4) ? 2 : (threes >= 4) ? 3 :
                            (fours >= 4) ? 4 : (fives >= 4) ? 5 : 6;
        for (int i = 0; i < NUMDICE; i++) {
            if (dice[i] == value_to_keep) {
                keep[i] = 1;  // Keep all dice that match the value
            }
        }
        return;
    }

    // Full House (three of one value and two of another)
    if ((ones == 3 || twos == 3 || threes == 3 || fours == 3 || fives == 3 || sixes == 3) &&
        (ones == 2 || twos == 2 || threes == 2 || fours == 2 || fives == 2 || sixes == 2)) {
        for (int i = 0; i < NUMDICE; i++) {
            keep[i] = 1;  // Keep all dice for a Full House
        }
        return;
    }

    // Check for Small or Large Straight
    if ((ones && twos && threes && fours) || (twos && threes && fours && fives) || (threes && fours && fives && sixes)) {
        for (int i = 0; i < NUMDICE; i++) {
            if ((dice[i] >= 1 && dice[i] <= 4) || (dice[i] >= 2 && dice[i] <= 5) || (dice[i] >= 3 && dice[i] <= 6)) {
                keep[i] = 1;  // Keep dice for a Small Straight
            }
        }
        return;
    }

    // Keep any pairs
    if (ones >= 2 || twos >= 2 || threes >= 2 || fours >= 2 || fives >= 2 || sixes >= 2) {
        int value_to_keep = (ones >= 2) ? 1 : (twos >= 2) ? 2 : (threes >= 2) ? 3 :
                            (fours >= 2) ? 4 : (fives >= 2) ? 5 : 6;
        for (int i = 0; i < NUMDICE; i++) {
            if (dice[i] == value_to_keep) {
                keep[i] = 1;  // Keep all dice that form a pair
            }
        }
    }
}

int choose_best_category(int dice[], int computer_scorecard[]) {
    int category = -1;
    int max_score = 0;
    int ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixes = 0;
    for (int i = 0; i < NUMDICE; i++) {
        switch (dice[i]) {
            case 1: ones++; break;
            case 2: twos++; break;
            case 3: threes++; break;
            case 4: fours++; break;
            case 5: fives++; break;
            case 6: sixes++; break;
        }
    }

    // Check for the highest scoring category based on the dice roll
    for (int i = 0; i < NUMROUNDS; i++) {
        if (computer_scorecard[i] == 0) {  // Only evaluate unchosen categories
            int score = 0;
            
            switch (i) {
                case 0: score = ones; break;  // Ones
                case 1: score = twos * 2; break;  // Twos
                case 2: score = threes * 3; break;  // Threes
                case 3: score = fours * 4; break;  // Fours
                case 4: score = fives * 5; break;  // Fives
                case 5: score = sixes * 6; break;  // Sixes
                case 6:  // Three-of-a-kind
                    if (ones >= 3 || twos >= 3 || threes >= 3 || 
                        fours >= 3 || fives >= 3 || sixes >= 3) {
                        score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4]; // Total of the dice
                    }
                    break;
                case 7:  // Four-of-a-kind
                    if (ones >= 4 || twos >= 4 || threes >= 4 || 
                        fours >= 4 || fives >= 4 || sixes >= 4) {
                        score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4]; // Total of the dice
                    }
                    break;
                case 8:  // Full House (3-of-a-kind + a pair)
                    if ((ones == 3 || twos == 3 || threes == 3 || 
                         fours == 3 || fives == 3 || sixes == 3) &&
                        (ones == 2 || twos == 2 || threes == 2 || 
                         fours == 2 || fives == 2 || sixes == 2)) {
                        score = 25;
                    }
                    break;
                case 9:  // Small Straight
                    if ((ones && twos && threes && fours) ||  // 1-2-3-4
                        (twos && threes && fours && fives) ||  // 2-3-4-5
                        (threes && fours && fives && sixes)) {  // 3-4-5-6
                        score = 30;
                    }
                    break;
                case 10:  // Large Straight
                    if ((ones && twos && threes && fours && fives) ||  // 1-2-3-4-5
                        (twos && threes && fours && fives && sixes)) {  // 2-3-4-5-6
                        score = 40;
                    }
                    break;
                case 11:  // Yahtzee
                    if (ones == 5 || twos == 5 || threes == 5 || 
                        fours == 5 || fives == 5 || sixes == 5) {
                        score = 50;
                    }
                    break;
                case 12:  // Chance (sum of all dice)
                    score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
                    break;
            }

            // Update the best category if this one has a higher score
            if (score > max_score) {
                max_score = score;
                category = i;
            }
        }
    }
    return category;
}

int calculate_score(int dice[], int category) {
    int score = 0, ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixs = 0;
    for (int i = 0; i < NUMDICE; i++) {
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
        case 0: score = ones; break;  
        case 1: score = twos * 2; break; 
        case 2: score = threes * 3; break; 
        case 3: score = fours * 4; break;
        case 4: score = fives * 5; break; 
        case 5: score = sixs * 6; break;  
        case 6: // Three of a Kind
            if(ones >= 3 || twos >= 3 || threes >= 3 || fours >= 3 || fives >= 3 || sixs >= 3){
                score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
            }else{
                score = 0;
            }
            break;
        case 7: // Four of a Kind
            if(ones >= 4 || twos >= 4 || threes >= 4 || fours >= 4 || fives >= 4 || sixs >= 4){
                score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
            }else{
                score = 0;
            }
            break;
        case 8:  // Full House
            if ((ones == 3 || twos == 3 || threes == 3 || fours == 3 || fives == 3 || sixs == 3) &&
                (ones == 2 || twos == 2 || threes == 2 || fours == 2 || fives == 2 || sixs == 2)) {
                score = 25;
            } else {
                score = 0;
            }
            break;
        case 9: // Small Straight
            if ((ones && twos && threes && fours) || (twos && threes && fours && fives) || 
                (threes && fours && fives && sixs)) {
                score = 30;
            }else{
                score = 0;
            }
            break;
        case 10: // Large Straight
            if ((ones && twos && threes && fours && fives) || (twos && threes && fours && fives && sixs)) {
                score = 40;
            }else{
                score = 0;
            }
            break;
        case 11: // Yahtzee
            if (ones == 5 || twos == 5 || threes == 5 || fours == 5 || fives == 5 || sixs == 5) {
                score = 50;
            }else{
                score = 0;
            }
            break;
        case 12: // Chance
            score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4]; 
            break;
        default:
            printf("Error.\n");
            break;
    }   
     return score;
}




