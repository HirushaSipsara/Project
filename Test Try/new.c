#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_DICE 5
#define NUM_ROLLS 3
#define NUM_ROUNDS 13

void user_turn(int dice[], int user_score[], int scorecard[]){
    int roll_count = 0;
    int keep_dices[NUM_DICE] = {0};
    char choice;

    while (roll_count < NUM_ROLLS)
    {
        printf("%d Roll :\n", roll_count + 1);
        if(roll_count==0){
             printf("st Roll");
        }else if(roll_count==0){
            printf("nd Roll");
        }else{
            printf("rd Roll");
        }
        roll_dice(dice, keep_dices);
        
        if (roll_count < NUM_ROLLS - 1) 
        {
            printf("Would you like to keep any dice? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                // Ask the user which dice to keep
                for (int i = 0; i < NUM_DICE; i++) {
                    printf("Keep dice %d (value: %d)? (1 = yes, 0 = no): ", i + 1, dice[i]);
                    scanf("%d", &keep_dices[i]);  // 1 = keep, 0 = re-roll
                }
            }else{
                if (roll_count < NUM_ROLLS - 1) 
                {
                    printf("Do you want to re-roll remaining dice? (y/n): ");
                    scanf(" %c", &choice);
                    if (choice == 'n' || choice == 'N') {
                        break;  
                    }
                }
                 roll_count++; 
            }
        }
        
        int category;
        do {
            char *scoringOptions[] = {"0 = Ones","1 = Twos","2 = Threes","3 = Fours","4 = Fives","5 = Sixes","6 = Three of a Kind","7 = Four of a Kind","8 = Full House","9 = Small Straight","10 = Large Straight","11 = Yahtzee","12 = Chance"};
            for(int i = 0; i < NUM_ROUNDS; i++){
                if(scorecard[i] == 0){
                    printf("%s", scoringOptions[i]);
                }
            }
            printf("Now choose a category in the above (0-12): ");
            scanf("%d", &category);
        } while (has_category_been_chosen(category, user_score));
        
        user_score[category] = calculate_score(dice, category);
        scorecard[category]= 1;
    }
}


// Calculate Score based on category
int calculate_score(int dice[], int category) {
    int score = 0, ones = 0, twos = 0, threes = 0, fours = 0, fives = 0, sixs = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        if (dice[i] == 1) {
            ones++;
        } else if (dice[i] == 2) {
            twos++;
        } else if (dice[i] == 3) {
            threes++;
        } else if (dice[i] == 4) {
            fours++;
        } else if (dice[i] == 5) {
            fives++;
        } else if (dice[i] == 6) {
            sixs++;
        }
    }
    
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
        case 2:
            for (int i = 0; i < NUM_DICE; i++)
                if (dice[i] == 3) score += 3;
            break;
        case 3:
            for (int i = 0; i < NUM_DICE; i++)
                if (dice[i] == 4) score += 4;
            break;
        case 4:
            for (int i = 0; i < NUM_DICE; i++)
                if (dice[i] == 5) score += 5;
            break;
        case 5:
            for (int i = 0; i < NUM_DICE; i++)
                if (dice[i] == 6) score += 6;
            break;
        case 6:
            if(ones >= 3 || twos >= 3 || threes >= 3 || fours >= 3 || fives >= 3 || sixs >= 3){
                score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
            }else{
                printf("Wrong Selection, Do Re-do it again..");

            }
            break;
        case 7:
            if(ones >= 4 || twos >= 4 || threes >= 4 || fours >= 4 || fives >= 4 || sixs >= 4){
                score = dice[0] + dice[1] + dice[2] + dice[3] + dice[4];
            }
            break;
        case 8:
            if(ones >= 5 || twos >= 5 || threes >= 5 || fours >= 5 || fives >= 5 || sixs >= 5){
                score = 25;
            }
            break;
        case 9:
            if((ones == 1 && twos == 1 && threes == 1 && fours == 1 ) || (twos == 1 && threes == 1 && fours == 1 && fives ==1) || (threes == 1 && fours == 1 && fives ==1 && sixs == 1)){
                score = 30;
            }
            break;
        case 10:
            if((ones == 1 && twos == 1 && threes == 1 && fours == 1 && fives == 1) || (twos == 1 && threes == 1 && fours == 1 && fives ==1 && sixs == 1)){
                score = 40;
            }
            break;
        case 11:
            if(ones == 6 || twos == 6 || threes == 6 || fours == 6 || fives == 6 || sixs == 6){
                score = 50;
            }
            break;
            break;
        case 12:
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
void roll_dice(int dice[], int keep_dices[]) 
{
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep_dices[i] != 0) {  // Only roll dice that are not kept
            dice[i] = rand() % 6 + 1; 
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
void print_scorecard(int score[]) {
    printf("Scorecard: ");
    for (int i = 0; i < NUM_ROUNDS; i++) {
        printf("%d ", score[i]);
    }
    printf("\n");
}

int main() {
    int user_score[NUM_ROUNDS] = {0};
    int computer_score[NUM_ROUNDS] = {0};
    int scorecard[NUM_ROUNDS]={0};
    int dice[NUM_DICE];
    
    for (int i = 0; i < NUM_ROUNDS; i++) {
        printf("\nRound %d\n", i + 1);
        
        // Human Player
        printf("Human's turn:\n");
        user_turn(dice, user_score, scorecard);
        print_score(user_score);
        
        // Computer Player
        printf("\nComputer's turn:\n");
        computer_turn(dice, computer_score);
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