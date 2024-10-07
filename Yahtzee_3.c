#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_DICE 5
#define NUM_ROLLS 3
#define NUM_ROUNDS 13

// Function declarations
void user_turn(int dice[], int user_score[], int user_scorecard[]);
void select_scoring(int dice[], int user_scorecard[], int user_score[]);
int calculate_score(int dice[], int category, int user_score[]);
int has_category_been_chosen(int category, int user_score[]);
void roll_dice(int dice[], int keep_dices[]);
void print_dice(int dice[]);
void print_score(int score[]);
void computer_turn(int dice[], int computer_score[], int computer_scorecard[]);
void print_computer_choose(int best_category);
void evaluate_dice(int dice[], int keep[], int computer_scorecard[]);
int choose_best_category(int dice[], int computer_score[], int computer_scorecard[]);

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
        
    user_score[category] = calculate_score(dice, category, user_score);
    user_scorecard[category]= 1;
}

// Calculate Score based on category
int calculate_score(int dice[], int category, int user_score[]) {
    int score = 0, ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixs = 0;
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

// Check if category has been chosen already
int has_category_been_chosen(int category, int user_score[]) {
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
void computer_turn(int dice[], int computer_score[], int computer_scorecard[]) {
    int roll_count = 0;
    int keep_dices[NUM_DICE] = {0};  // The computer decides which dice to keep

    while (roll_count < NUM_ROLLS) {
        roll_dice(dice, keep_dices);
        roll_count++;

        if (roll_count < NUM_ROLLS) {
            evaluate_dice(dice, keep_dices, computer_scorecard);  // Basic strategy to keep useful dice
        }
    }

    int best_category = choose_best_category(dice, computer_score, computer_scorecard);
    print_computer_choose(best_category);
    computer_score[best_category] = calculate_score(dice, best_category,  computer_score);
    computer_scorecard[best_category] = 1;
}

void print_computer_choose(int best_category){
    const char *scoringOptions[] = {
        "Ones",
        "Twos",
        "Threes",
        "Fours",
        "Fives",
        "Sixes",
        "Three of a Kind",
        "Four of a Kind",
        "Full House",
        "Small Straight",
        "Large Straight",
        "Yahtzee",
        "Chance"
    };
    for(int i = 0; i < NUM_ROUNDS; i++){
        if (i == best_category) {
            printf("Computer choose %s option\n", scoringOptions[i]);
        }
    }
}

//Choose which dices to keep or not
void evaluate_dice(int dice[], int keep[], int computer_scorecard[]) {
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

    // Yahtzee (five of the same number)
    if (ones == 5 || twos == 5 || threes == 5 || fours == 5 || fives == 5 || sixes == 5) {
        for (int i = 0; i < NUM_DICE; i++) {
            keep[i] = 1;  // Keep all dice for Yahtzee
        }
        return;
    }

    // Three-of-a-kind
    if (ones >= 3 || twos >= 3 || threes >= 3 || fours >= 3 || fives >= 3 || sixes >= 3) {
        int value_to_keep = (ones >= 3) ? 1 : (twos >= 3) ? 2 : (threes >= 3) ? 3 :
                            (fours >= 3) ? 4 : (fives >= 3) ? 5 : 6;
        for (int i = 0; i < NUM_DICE; i++) {
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
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == value_to_keep) {
                keep[i] = 1;  // Keep all dice that match the value
            }
        }
        return;
    }

    // Full House (three of one value and two of another)
    if ((ones == 3 || twos == 3 || threes == 3 || fours == 3 || fives == 3 || sixes == 3) &&
        (ones == 2 || twos == 2 || threes == 2 || fours == 2 || fives == 2 || sixes == 2)) {
        for (int i = 0; i < NUM_DICE; i++) {
            keep[i] = 1;  // Keep all dice for a Full House
        }
        return;
    }

    // Check for Small or Large Straight
    if ((ones && twos && threes && fours) || (twos && threes && fours && fives) || (threes && fours && fives && sixes)) {
        for (int i = 0; i < NUM_DICE; i++) {
            if ((dice[i] >= 1 && dice[i] <= 4) || (dice[i] >= 2 && dice[i] <= 5) || (dice[i] >= 3 && dice[i] <= 6)) {
                keep[i] = 1;  // Keep dice for a Small Straight
            }
        }
        return;
    }

    // Check if any pairs exist
    int value_to_keep = -1;
    if (ones >= 2) value_to_keep = 1;
    else if (twos >= 2) value_to_keep = 2;
    else if (threes >= 2) value_to_keep = 3;
    else if (fours >= 2) value_to_keep = 4;
    else if (fives >= 2) value_to_keep = 5;
    else if (sixes >= 2) value_to_keep = 6;

    // If a pair exists and the corresponding category is still unchosen, mark them to be kept
    if (value_to_keep != -1 && computer_scorecard[value_to_keep - 1] == 0) {
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == value_to_keep) {
                keep[i] = 1;  // Keep all dice that form a pair
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
        computer_turn(dice, computer_score, computer_scorecard);
        printf("Computer's score so far:\n");
        print_score(computer_score);
    }
    
    // Final Scores
    int human_total = 0, computer_total = 0;
    for (int i = 0; i < NUM_ROUNDS; i++) {
        human_total += user_score[i];
        computer_total += computer_score[i];
    }
    
    printf("\nFinal Scores: Human = %d and Computer = %d\n", human_total, computer_total);
    if (human_total > computer_total)
        printf("Human wins!\n");
    else if (computer_total > human_total)
        printf("Computer wins!\n");
    else
        printf("It's a tie!\n");

    return 0;
}