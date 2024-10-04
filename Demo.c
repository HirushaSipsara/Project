#include <stdio.h>
#include <ctype.h>
#include <string.h>

void formatName(char name[]) {
    char x[] = " N/A";
    int space_found = 0;
    
    name[0] = toupper(name[0]);  
    for (int i = 1; i < strlen(name); i++) {
        if (name[i-1] == ' ') {
            name[i] = toupper(name[i]); 
            space_found = 1;             
        } else if (name[i] == '\n' || name[i] == '\0') {
            if (!space_found) { 
                strcat(name, x);
            }
            break;
        } else {
            name[i] = tolower(name[i]);  // Lowercase the rest of the characters
        }
    }
}

int main() {
    char name[100];
    
    printf("Enter name: ");
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = 0; 
    
    formatName(name);
    
    printf("Formatted Name: %s\n", name);
    
    return 0;
}
