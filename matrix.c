
#include <stdio.h>

int main() {
    int n = 5; 
    int i, j;
    for(i = 1; i <= n; i++) {
        for(j = 1; j <= n; j++) {
          
            if(i == 1  i == n  j == 1 || j == n) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}
 


/*
#include <stdio.h>

int main() {
    int n = 5; 
    int i, j;
    char border_char = '*';  
    char inner_char = 'X'; 
    for(i = 1; i <= n; i++) {
        for(j = 1; j <= n; j++) 
            if(i == 1  i == n  j == 1 || j == n) {
                printf("%c", border_char); 
            else if(i == n / 2 + 1 && j == n / 2 + 1) {
                printf("%c", inner_char);  
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}
*/