#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_DICE 5
#define NUM_ROLLS 3
#define NUM_ROUNDS 13

int count_yatz_user = 0;
int count_yatz_comp = 0;

// Function declarations
void user_turn(int dice[], int user_score[], int user_scorecard[]);
void select_scoring(int dice[], int user_scorecard[], int user_score[]);
int calculate_score_user(int dice[], int category, int user_score[], int *count_yatz_user);
int calculate_score_comp(int dice[], int category, int user_score[], int *count_yatz_comp);
int has_category_been_chosen(int category, int user_score[]);
void roll_dice(int dice[], int keep_dices[]);
void print_dice(int dice[]);
void print_score(int score[]);
void computer_turn(int dice[], int computer_score[], int computer_scorecard[], int round);
void evaluate_dice(int dice[], int keep[], int computer_scorecard[], int round);
int choose_best_category(int dice[], int computer_score[], int computer_scorecard[]);
void display_score_table(int user_score[], int computer_score[]);

// User's Turn
void user_turn(int dice[], int user_score[], int user_scorecard[]){
    int roll_count = 0;
    int keep_dices[NUM_DICE] = {0};
    char choice;

    while (roll_count < NUM_ROLLS)
    {
        printf("%d ", roll_count + 1);
        if(roll_count==0){
             printf("st Roll \n");
        }else if(roll_count==0){
            printf("nd Roll \n");
        }else{
            printf("rd Roll \n");
        }
        roll_dice(dice, keep_dices);
        
        
        if (roll_count < NUM_ROLLS - 1) {
            printf("Would you like to keep any dice? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                // Ask the user which dice to keep
                for (int i = 0; i < NUM_DICE; i++) {
                    printf("Keep dice %d (value: %d)? (1 = yes, 0 = no): ", i + 1, dice[i]);
                    scanf("%d", &keep_dices[i]);  // 1 = keep, 0 = re-roll
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
    select_scoring(dice, user_scorecard, user_score);
}

// Select scoring category
void select_scoring(int dice[], int user_scorecard[], int user_score[]){
    int category;
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
    user_scorecard[11] = 0;
    do {
        printf("Available scoring categories:\n");
        for(int i = 0; i < NUM_ROUNDS; i++){
            if (user_scorecard[i] == 0) {
                printf("%s\n", scoringOptions[i]);
            }
        }
         printf("Now choose a category in the above (0-12): ");
        scanf("%d", &category);
        if (category < 0 || category > 12) {
        printf("Invalid category. Please select a valid option.\n");
        } else if (has_category_been_chosen(category, user_score)) {
            printf("Category already chosen. Select another.\n");
        }
    } while (category < 0 || category > 12 || has_category_been_chosen(category, user_score));    
        
    user_score[category] = calculate_score_user(dice, category, user_score, &count_yatz_user);
    user_scorecard[category]= 1;
}

// Calculate Score based on category
int calculate_score_user(int dice[], int category, int user_score[], int *count_yatz_user) {
    int score = 0, ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixs = 0, yatz_score=0;
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
                if (*count_yatz_user == 0) {
                    score = 50;
                } else {
                    score = 50 + *count_yatz_user * 100;
                }
                (*count_yatz_user)++;
            }else{
                yatz_score = 0;
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

//Computer Score Calculating
int calculate_score_comp(int dice[], int category, int user_score[], int *count_yatz_comp) {
    int score = 0, ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixs = 0, yatz_score=0;
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
                if (*count_yatz_comp == 0) {
                    score = 50;
                } else {
                    score = 50 + *count_yatz_comp * 100;
                }
                (*count_yatz_comp)++;
            }else{
                yatz_score = 0;
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

// Check if category has been chosen already
int has_category_been_chosen(int category, int user_score[]) {
    user_score[11]=0;
    return user_score[category] != 0;
}

// Roll Dice Function
void roll_dice(int dice[], int keep_dices[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep_dices[i] == 0) {  // Roll dice that are not kept
            dice[i] = rand() % 6 + 1;  // Assign a random number between 1 and 6
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

// Print the scorecard
void print_score(int score[]) {
    printf("Scorecard: ");
    for (int i = 0; i < NUM_ROUNDS; i++) {
        printf("%d ", score[i]);
    }
    printf("\n");
}

// Computer's Turn
void computer_turn(int dice[], int computer_score[], int computer_scorecard[], int round) {
    int roll_count = 0;
    int keep_dices[NUM_DICE] = {0};  // The computer decides which dice to keep

    while (roll_count < NUM_ROLLS) {
        roll_dice(dice, keep_dices);
        roll_count++;

        if (roll_count < NUM_ROLLS) {
            evaluate_dice(dice, keep_dices, computer_scorecard, round);  // Basic strategy to keep useful dice
        }
    }

    int best_category = choose_best_category(dice, computer_score, computer_scorecard);
    computer_score[best_category] = calculate_score_comp(dice, best_category,  computer_score, &count_yatz_comp);
    computer_scorecard[best_category] = 1;
}

void evaluate_dice(int dice[], int keep[], int computer_scorecard[], int round) {
    int ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixes = 0;

    // Count occurrences of each dice value
    for (int i = 0; i < NUM_DICE; i++) {
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
    for (int i = 0; i < NUM_DICE; i++) {
        keep[i] = 0;
    }

     // Prioritize filling Ones to Sixes categories early in the game
    if (round < 6) {  // First few rounds prioritize individual number categories
        if (ones >= 1 && computer_scorecard[0] == 0) {
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 1) keep[i] = 1;
            }
            return;
        } else if (twos >= 1 && computer_scorecard[1] == 0) {
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 2) keep[i] = 1;
            }
            return;
        } else if (threes >= 1 && computer_scorecard[2] == 0) {
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 3) keep[i] = 1;
            }
            return;
        } else if (fours >= 1 && computer_scorecard[3] == 0) {
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 4) keep[i] = 1;
            }
            return;
        } else if (fives >= 1 && computer_scorecard[4] == 0) {
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 5) keep[i] = 1;
            }
            return;
        } else if (sixes >= 1 && computer_scorecard[5] == 0) {
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 6) keep[i] = 1;
            }
            return;
        }
    }

    //Yahtzee
    if (ones == 5 || twos == 5 || threes == 5 || fours == 5 || fives == 5 || sixes == 5) {
        for (int i = 0; i < NUM_DICE; i++) {
            keep[i] = 1;  
        }
        return;
    }
    

    // Prioritize Four-of-a-Kind, then Three-of-a-Kind, after early rounds
    if (round >= 6) {  // Delay these categories until mid to late rounds
        if (ones >= 4 || twos >= 4 || threes >= 4 || fours >= 4 || fives >= 4 || sixes >= 4) {
            int value_to_keep = (ones >= 4) ? 1 : (twos >= 4) ? 2 : (threes >= 4) ? 3 :
                                (fours >= 4) ? 4 : (fives >= 4) ? 5 : 6;
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == value_to_keep) {
                    keep[i] = 1;
                }
            }
            return;
        } else if (ones >= 3 || twos >= 3 || threes >= 3 || fours >= 3 || fives >= 3 || sixes >= 3) {
            int value_to_keep = (ones >= 3) ? 1 : (twos >= 3) ? 2 : (threes >= 3) ? 3 :
                                (fours >= 3) ? 4 : (fives >= 3) ? 5 : 6;
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == value_to_keep) {
                    keep[i] = 1;
                }
            }
            return;
        }
    }

    // Full House (three of one value and two of another)
    if (computer_scorecard[8] == 0) {
        if ((ones == 3 || twos == 3 || threes == 3 || fours == 3 || fives == 3 || sixes == 3) &&
            (ones == 2 || twos == 2 || threes == 2 || fours == 2 || fives == 2 || sixes == 2)) {
            for (int i = 0; i < NUM_DICE; i++) {
                keep[i] = 1;  
            }
            return;
        }
    }

    // 2. Improve Straight Detection by Sorting and Checking for Consecutive Values
    int sorted_dice[NUM_DICE];
    memcpy(sorted_dice, dice, NUM_DICE * sizeof(int));
    for (int i = 0; i < NUM_DICE - 1; i++) {
        for (int j = 0; j < NUM_DICE - i - 1; j++) {
            if (sorted_dice[j] > sorted_dice[j + 1]) {
                    int temp = sorted_dice[j];
                    sorted_dice[j] = sorted_dice[j + 1];
                    sorted_dice[j + 1] = temp;
            }
        }
    }

    // Check for Small or Large Straight
    if (computer_scorecard[9] == 0 || computer_scorecard[10] == 0) {
        // Check for Small Straight (sequence of 4 consecutive numbers)
        if (computer_scorecard[9] == 0) {
            if ((sorted_dice[0] == sorted_dice[1] - 1 &&
                sorted_dice[1] == sorted_dice[2] - 1 &&
                sorted_dice[2] == sorted_dice[3] - 1) ||
                (sorted_dice[1] == sorted_dice[2] - 1 &&
                sorted_dice[2] == sorted_dice[3] - 1 &&
                sorted_dice[3] == sorted_dice[4] - 1)) {
                for (int i = 0; i < NUM_DICE; i++) {
                    keep[i] = 1;  // Keep dice for Small Straight
                }
                return;
            }
        }

        // Check for Large Straight (sequence of 5 consecutive numbers)
        if (computer_scorecard[10] == 0) {
            if ((sorted_dice[0] == 1 && sorted_dice[1] == 2 &&
                sorted_dice[2] == 3 && sorted_dice[3] == 4 && sorted_dice[4] == 5) ||
                (sorted_dice[0] == 2 && sorted_dice[1] == 3 &&
                sorted_dice[2] == 4 && sorted_dice[3] == 5 && sorted_dice[4] == 6)) {
                for (int i = 0; i < NUM_DICE; i++) {
                    keep[i] = 1;  // Keep all dice for Large Straight
                }
                return;
            }
        }
    }
    // 3. Refine Pair-Keeping Logic
    if ((ones == 2 || twos == 2 || threes == 2 || fours == 2 || fives == 2 || sixes == 2) && (computer_scorecard[8] == 0 || computer_scorecard[7] == 0)) { // Full House or Four-of-a-Kind
        int value_to_keep = (ones == 2) ? 1 : (twos == 2) ? 2 : (threes == 2) ? 3 :
                            (fours == 2) ? 4 : (fives == 2) ? 5 : 6;
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == value_to_keep) {
                keep[i] = 1;
            }
        }
        return;
    }

    // Keep any pairs while it's chosen or not (6 to 1 search)
    if (sixes >= 2 || fives >= 2 || fours >= 2 || threes >= 2 || twos >= 2 || ones >= 2) {
        int value_to_keep = (sixes >= 2) ? 6 : (fives >= 2) ? 5 : (fours >= 2) ? 4 : (threes >= 2) ? 3 : (twos >= 2) ? 2 : 1;
        if (computer_scorecard[value_to_keep - 1] == 0) {  // Check if the category is available
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == value_to_keep) {
                    keep[i] = 1;  
                }
            }
        }
    }

    // If only a few categories are left, prioritize them
    if (round >= 10) {  // Assuming there are 13 rounds total
        if (computer_scorecard[9] == 0) { // Small Straight
            for (int i = 0; i < NUM_DICE; i++) {
                if ((dice[i] >= 1 && dice[i] <= 4) || (dice[i] >= 2 && dice[i] <= 5) || (dice[i] >= 3 && dice[i] <= 6)) {
                    keep[i] = 1;
                }
            }
        } else if (computer_scorecard[10] == 0) { // Large Straight
            for (int i = 0; i < NUM_DICE; i++) {
                if (sorted_dice[i] >= sorted_dice[0] && sorted_dice[i] <= sorted_dice[0] + 4) {
                    keep[i] = 1;
                }
            }
        }
    }
}

// Choose the best scoring category for the computer
int choose_best_category(int dice[], int computer_score[], int computer_scorecard[]) {
    int category = -1;
    int max_score = 0;
    int ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixes = 0;
    for (int i = 0; i < NUM_DICE; i++) {
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
    for (int i = 0; i < NUM_ROUNDS; i++) {
        computer_scorecard[11]=0;
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

void display_score_table(int user_score[], int computer_score[]) {
    int human_total = 0, computer_total = 0;   // Initialize totals
    int player_bonus = 0, computer_bonus = 0;  // Initialize bonuses
    int human_temp = 0, computer_temp = 0;     // Initialize temporary totals for bonus calculation

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

    // Accumulate total scores for each round
    for (int i = 0; i < NUM_ROUNDS; i++) {
        human_total += user_score[i];
        computer_total += computer_score[i];
    }

    printf("+--------------------------+------------------+----------------+\n");
    printf("|        Category          |    Player Score  | Computer Score |\n");
    printf("+--------------------------+------------------+----------------+\n");

    // Display scores for each category
    for (int i = 0; i < NUM_ROUNDS; i++) {
        printf("| %-24s |       %-6d     |     %-6d     |\n", scoringOptions[i], user_score[i], computer_score[i]);
    }

    // Check if the first six categories have been scored
    int all_user_scored = 1, all_computer_scored = 1;
    for (int i = 0; i < 6; i++) {
        if (user_score[i] == 0) all_user_scored = 0;
        if (computer_score[i] == 0) all_computer_scored = 0;
        human_temp += user_score[i];
        computer_temp += computer_score[i];
    }

    // Apply bonus if applicable
    if (all_user_scored && human_temp > 50) {
        player_bonus = 100;
        human_total += player_bonus;
    }
    if (all_computer_scored && computer_temp > 50) {
        computer_bonus = 100;
        computer_total += computer_bonus;
    }

    printf("+--------------------------+------------------+----------------+\n");
    // Display bonus if applicable
    printf("| %-24s |       %-6d     |     %-6d     |\n", "Bonus", player_bonus, computer_bonus);
    printf("+--------------------------+------------------+----------------+\n");

    // Display total scores including bonuses
    printf("| %-24s |       %-6d     |     %-6d     |\n", "Total Score", human_total, computer_total);
    printf("+--------------------------+------------------+----------------+\n");
    printf("\n");
}


void round_one(){
    printf(" /$$$$$$$                                      /$$        /$$$$$$                     \n");
    printf("| $$__  $$                                    | $$       /$$__  $$                    \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$      | $$  \\ $$ /$$$$$$$   /$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$      | $$  | $$| $$__  $$ /$$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$      | $$  | $$| $$  \\ $$| $$$$$$$$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      | $$  | $$| $$  | $$| $$_____/\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$      |  $$$$$$/| $$  | $$|  $$$$$$$\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/       \\______/ |__/  |__/ \\_______/\n");
}

void round_two(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$                     \n");
    printf("| $$__  $$                                    | $$      |__  $$__/                     \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$         | $$ /$$  /$$  /$$  /$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$         | $$| $$ | $$ | $$ /$$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$         | $$| $$ | $$ | $$| $$  \\ $$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$         | $$| $$ | $$ | $$| $$  | $$\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$         | $$|  $$$$$/$$$$/|  $$$$$$/\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/         |__/ \\_____/\\___/  \\______/ \n");
}

void round_three(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$ /$$                                     \n");
    printf("| $$__  $$                                    | $$      |__  $$__/| $$                                     \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$         | $$   | $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$         | $$   | $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$         | $$   | $$  \\ $$| $$  \\__/| $$$$$$$$| $$$$$$$$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$         | $$   | $$  | $$| $$      | $$_____/| $$_____/\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$         | $$   | $$  | $$| $$      |  $$$$$$$|  $$$$$$$\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/         |__/   |__/  |__/|__/       \\_______/ \\_______/\n");
}

void round_four(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$                           \n");
    printf("| $$__  $$                                    | $$      | $$_____/                           \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$      | $$     /$$$$$$  /$$   /$$  /$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$      | $$$$$ /$$__  $$| $$  | $$ /$$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$      | $$__/| $$  \\ $$| $$  | $$| $$  \\__/\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      | $$   | $$  | $$| $$  | $$| $$      \n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$      | $$   |  $$$$$$/|  $$$$$$/| $$      \n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/      |__/    \\______/  \\______/ |__/      \n");
}

void round_five(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$ /$$                    \n");
    printf("| $$__  $$                                    | $$      | $$_____/|__/                    \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$      | $$       /$$ /$$    /$$ /$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$      | $$$$$   | $$|  $$  /$$//$$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$      | $$__/   | $$ \\  $$/$$/| $$$$$$$$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      | $$      | $$  \\  $$$/ | $$_____/\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$      | $$      | $$   \\  $/  |  $$$$$$$\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/      |__/      |__/    \\_/    \\_______/\n");
}

void round_six(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$        /$$$$$$  /$$          \n");
    printf("| $$__  $$                                    | $$       /$$__  $$|__/          \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$      | $$  \\__/ /$$ /$$   /$$\n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$      |  $$$$$$ | $$|  $$ /$$/\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$       \\____  $$| $$ \\  $$$$/ \n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$       /$$  \\ $$| $$  >$$  $$ \n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$      |  $$$$$$/| $$ /$$/\\  $$\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/       \\______/ |__/|__/  \\__/\n");
}

void round_seven(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$        /$$$$$$                                         \n");
    printf("| $$__  $$                                    | $$       /$$__  $$                                        \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$      | $$  \\__/  /$$$$$$  /$$    /$$ /$$$$$$  /$$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$      |  $$$$$$  /$$__  $$|  $$  /$$//$$__  $$| $$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$       \\____  $$| $$$$$$$$ \\  $$/$$/| $$$$$$$$| $$  \\ $$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$       /$$  \\ $$| $$_____/  \\  $$$/ | $$_____/| $$  | $$\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$      |  $$$$$$/|  $$$$$$$   \\  $/  |  $$$$$$$| $$  | $$\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/       \\______/  \\_______/    \\_/    \\_______/|__/  |__/\n");
}

void round_eight(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$ /$$           /$$         /$$    \n");
    printf("| $$__  $$                                    | $$      | $$_____/|__/          | $$        | $$    \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$      | $$       /$$  /$$$$$$ | $$$$$$$  /$$$$$$  \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$      | $$$$$   | $$ /$$__  $$| $$__  $$|_  $$_/  \n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$      | $$__/   | $$| $$  \\ $$| $$  \\ $$  | $$    \n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      | $$      | $$| $$  | $$| $$  | $$  | $$ /$$ \n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$      | $$$$$$$$| $$|  $$$$$$$| $$  | $$  |  $$$$/ \n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/      |________/|__/ \\____  $$|__/  |__/   \\___/  \n");
    printf("                                                                       /$$  \\ $$                    \n");
    printf("                                                                      |  $$$$$$/                    \n");
    printf("                                                                       \\______/                      \n");
}

void round_nine(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$   /$$ /$$                    \n");
    printf("| $$__  $$                                    | $$      | $$$ | $$|__/                    \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$      | $$$$| $$ /$$ /$$$$$$$   /$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$      | $$ $$ $$| $$| $$__  $$ /$$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$      | $$  $$$$| $$| $$  \\ $$| $$$$$$$$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      | $$\\  $$$| $$| $$  | $$| $$_____/ \n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$      | $$ \\  $$| $$| $$  | $$|  $$$$$$$ \n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/      |__/  \\__/|__/|__/  |__/ \\_______/\n");
}

void round_ten(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$                 \n");
    printf("| $$__  $$                                    | $$      |__  $$__/                 \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$         | $$  /$$$$$$  /$$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$         | $$ /$$__  $$| $$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$         | $$| $$$$$$$$| $$  \\ $$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$         | $$| $$_____/| $$  | $$\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$         | $$|  $$$$$$$| $$  | $$\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/         |__/ \\_______/|__/  |__/\n");
}

void round_eleven(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$ /$$                                        \n");
    printf("| $$__  $$                                    | $$      | $$_____/| $$                                        \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$      | $$      | $$  /$$$$$$  /$$    /$$ /$$$$$$  /$$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$      | $$$$$   | $$ /$$__  $$|  $$  /$$//$$__  $$| $$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$      | $$__/   | $$| $$$$$$$$ \\  $$/$$/| $$$$$$$$| $$  \\ $$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      | $$      | $$| $$_____/  \\  $$$/ | $$_____/| $$  | $$\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$      | $$$$$$$$| $$|  $$$$$$$   \\  $/  |  $$$$$$$| $$  | $$\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/      |________/|__/ \\_______/    \\_/    \\_______/|__/  |__/\n");
}

void round_twelve(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$                      /$$                    \n");
    printf("| $$__  $$                                    | $$      |__  $$__/                     | $$                    \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$         | $$ /$$  /$$  /$$  /$$$$$$ | $$ /$$    /$$ /$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$         | $$| $$ | $$ | $$ /$$__  $$| $$|  $$  /$$//$$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$         | $$| $$ | $$ | $$| $$$$$$$$| $$ \\  $$/$$/| $$$$$$$$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$         | $$| $$ | $$ | $$| $$_____/| $$  \\  $$$/ | $$_____/ \n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$         | $$|  $$$$$/$$$$/|  $$$$$$$| $$   \\  $/  |  $$$$$$$ \n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/         |__/ \\_____/\\___/  \\_______/|__/    \\_/    \\_______/\n");
}

void round_thirteen(){
    printf("\n");
    printf(" /$$$$$$$                                      /$$       /$$$$$$$$ /$$       /$$             /$$                                  \n");
    printf("| $$__  $$                                    | $$      |__  $$__/| $$      |__/            | $$                                  \n");
    printf("| $$  \\ $$  /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$$         | $$   | $$$$$$$  /$$  /$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$  /$$$$$$$ \n");
    printf("| $$$$$$$/ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$         | $$   | $$__  $$| $$ /$$__  $$|_  $$_/   /$$__  $$ /$$__  $$| $$__  $$\n");
    printf("| $$__  $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$         | $$   | $$  \\ $$| $$| $$  \\__/  | $$    | $$$$$$$$| $$$$$$$$| $$  \\ $$\n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$         | $$   | $$  | $$| $$| $$        | $$ /$$| $$_____/| $$_____/| $$  | $$\n");
    printf("| $$  | $$|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$         | $$   | $$  | $$| $$| $$        |  $$$$/|  $$$$$$$|  $$$$$$$| $$  | $$\n");
    printf("|__/  |__/ \\______/  \\______/ |__/  |__/ \\_______/         |__/   |__/  |__/|__/|__/         \\___/   \\_______/ \\_______/|__/  |__/\n");
}

int main() {
    srand(time(NULL));

    int user_score[NUM_ROUNDS] = {0};
    int computer_score[NUM_ROUNDS] = {0};
    int user_scorecard[NUM_ROUNDS]={0};
    int computer_scorecard[NUM_ROUNDS]={0};
    int dice[NUM_DICE] = {0};
    int round = 1;
    for (int i = 0; i < NUM_ROUNDS; i++) {
        switch (i){
        case 0:round_one(); break;
        case 1:round_two(); break;
        case 2:round_three(); break;
        case 3:round_four(); break;
        case 4:round_five(); break;
        case 5:round_six(); break;
        case 6:round_seven(); break;
        case 7:round_eight(); break;
        case 8:round_nine(); break;
        case 9:round_ten(); break;
        case 10:round_eleven(); break;
        case 11:round_twelve(); break;
        case 12:round_thirteen(); break;
        default:break;
        }
        
        
        // Human Player
        printf("User's turn:\n");
        user_turn(dice, user_score, user_scorecard);
        printf("User's score so far:\n");
        print_score(user_score);
        
        // Computer Player
        printf("\nComputer's turn:\n");
        computer_turn(dice, computer_score, computer_scorecard, round);
        printf("Computer's score so far:\n");
        print_score(computer_score);
        display_score_table(user_score, computer_score);
        round+=1;
    }
    
    // Final Scores
    int human_total = 0, computer_total = 0;
    int human_temp = 0, computer_temp = 0;
    int player_bonus = 0, computer_bonus = 0;

    int all_user_scored = 1, all_computer_scored = 1;
    for (int i = 0; i < 6; i++) {
        if (user_score[i] == 0) all_user_scored = 0;
        if (computer_score[i] == 0) all_computer_scored = 0;
        human_temp += user_score[i];
        computer_temp += computer_score[i];
    }

    // Apply bonus if applicable
    if (all_user_scored && human_temp > 50) {
        player_bonus = 100;
        human_total += player_bonus;
    }
    if (all_computer_scored && computer_temp > 50) {
        computer_bonus = 100;
        computer_total += computer_bonus;
    }

    for (int i = 0; i < NUM_ROUNDS; i++) {
        human_total += user_score[i];
        computer_total += computer_score[i];
    }
        
    printf("\nFinal Scores: Human = %d and Computer = %d\n", human_total, computer_total);
    if(human_total > computer_total){
        printf("Human wins!\n");
    } 
    else if (computer_total > human_total){
        printf("Computer wins!\n");
    }else{
        printf("It's a tie!\n");
    }

    printf("\nGame Over!\n");
    printf("Final User Score: ");
    print_score(user_score);
    printf("Final Computer Score: ");
    print_score(computer_score);

    printf("\nThank you for playing! Press Enter to exit.\n");
    getchar();  // Wait for user input
    getchar();  // To
    return 0;
}