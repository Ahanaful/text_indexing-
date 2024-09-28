#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PARAGRAPH_LENGTH 10000
#define MAX_WORD_LENGTH 50

void removePunct(char* str);
void makeLower(char* str);
void insertSrt(char** words, int n);


int main(int argc, char** argv) {
    char *fname = NULL;
    int mode = 0; // 0 - normal, 1 - verbose

    if (argc < 3) {
        printf("Not enough arguments. Need mode and filename, e.g.: 0 caps.txt\n");
        return 0;
    } else {
        fname = argv[2];
        mode = atoi(argv[1]);
    }

    printf("mode: %d  file: %s\n", mode, fname);  // KEEP THIS LINE

    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        perror("Error opening file");
        
    }

    char paragraph[MAX_PARAGRAPH_LENGTH];
    if (fgets(paragraph, MAX_PARAGRAPH_LENGTH, file) == NULL) {
        perror("Error reading paragraph");
        fclose(file);
        
    }

   
    makeLower(paragraph);
    removePunct(paragraph);

    
    char *words[MAX_PARAGRAPH_LENGTH / 2];
    int wordCount = 0;
    char *token = strtok(paragraph, " \n");
    while (token != NULL) {
        words[wordCount++] = token;
        token = strtok(NULL, " \n");
    }

    
    if (mode == 1) {
        printf("\n-- Original data --\n");
        printf("i | pointers[i] | word\n");
        printf("--|-------------|------------------\n");
        for (int i = 0; i < wordCount; i++) {
            printf("%d | %p | %s\n", i, (void *)words[i], words[i]);
        }
    } else {
        
        printf("\n-- Original data --\n");
        for (int i = 0; i < wordCount; i++) {
            printf("%d %s\n", i, words[i]);
        }
    }

    
    insertSrt(words, wordCount);

    
    if (mode == 1) {
        printf("\n-- Clean and sorted data --\n");
        printf("i | pointers[i] | word\n");
        printf("--|-------------|------------------\n");
        for (int i = 0; i < wordCount; i++) {
            printf("%d | %p | %s\n", i, (void *)words[i], words[i]);
        }
    } else {
        
        printf("\n-- Clean and sorted data --\n");
        for (int i = 0; i < wordCount; i++) {
            printf("%d %s\n", i, words[i]);
        }
    }

    
    char searchWord[MAX_WORD_LENGTH];
    while (fgets(searchWord, MAX_WORD_LENGTH, file) != NULL) {
        
        searchWord[strcspn(searchWord, "\n")] = 0;

        
        makeLower(searchWord);

        
        int iterations = 0;
        int left = 0, right = wordCount - 1;
        int mid = -1;
        int index = -1;

        printf("\n%s\n", searchWord);
        while (left <= right) {
            iterations++;
            mid = left + (right - left) / 2;

            
            if (iterations > 1) {
                printf(", ");
            }
            printf("%d", mid);

            int cmp = strcmp(words[mid], searchWord);
            if (cmp == 0) {
                index = mid;
                break;
            } else if (cmp < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        
        printf(" (%d iterations) %s\n", iterations, (index != -1) ? "found" : "not found");
    }

    fclose(file);
    return 0;
}


void removePunct(char* str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!ispunct((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}


void insertSrt(char** words, int n) {
    for (int i = 1; i < n; i++) {
        char *key = words[i];
        int j = i - 1;

       
        while (j >= 0 && strcmp(words[j], key) > 0) {
            words[j + 1] = words[j];
            j--;
        }
        words[j + 1] = key;
    }
}

void makeLower(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

