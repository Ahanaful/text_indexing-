#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PARAGRAPH_LENGTH 10000
#define MAX_WORD_LENGTH 50

// Function prototypes
void toLowercase(char* str);
void removePunctuation(char* str);
void insertionSort(char** words, int n);
int binarySearch(char** words, int n, char* target, int* iterations);

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
        return 1;
    }

    char paragraph[MAX_PARAGRAPH_LENGTH];
    if (fgets(paragraph, MAX_PARAGRAPH_LENGTH, file) == NULL) {
        perror("Error reading paragraph");
        fclose(file);
        return 1;
    }

    // Convert entire paragraph to lowercase to ensure consistency in sorting and searching
    toLowercase(paragraph);
    removePunctuation(paragraph);

    // Make an array of pointers to each word in the paragraph
    char *words[MAX_PARAGRAPH_LENGTH / 2];
    int wordCount = 0;
    char *token = strtok(paragraph, " \n");
    while (token != NULL) {
        words[wordCount++] = token;
        token = strtok(NULL, " \n");
    }

    // Print original data (verbose mode)
    if (mode == 1) {
        printf("-- Original data --\n");
        printf("i | pointers[i] | word\n");
        printf("--|-------------|------------------\n");
        for (int i = 0; i < wordCount; i++) {
            printf("%d | %p | %s\n", i, (void *)words[i], words[i]);
        }
    } else {
        // Normal mode original data
        printf("-- Original data --\n");
        for (int i = 0; i < wordCount; i++) {
            printf("%d %s\n", i, words[i]);
        }
    }

    // Sort the words using indirect insertion sort
    insertionSort(words, wordCount);

    // Print cleaned and sorted data (verbose mode)
    if (mode == 1) {
        printf("\n-- Clean and sorted data --\n");
        printf("i | pointers[i] | word\n");
        printf("--|-------------|------------------\n");
        for (int i = 0; i < wordCount; i++) {
            printf("%d | %p | %s\n", i, (void *)words[i], words[i]);
        }
    } else {
        // Normal mode sorted data
        printf("\n-- Clean and sorted data --\n");
        for (int i = 0; i < wordCount; i++) {
            printf("%d %s\n", i, words[i]);
        }
    }

    // Read each word in the list and search for it
    char searchWord[MAX_WORD_LENGTH];
    while (fgets(searchWord, MAX_WORD_LENGTH, file) != NULL) {
        // Remove newline character from search word
        searchWord[strcspn(searchWord, "\n")] = 0;

        // Convert the search word to lowercase for case-insensitive comparison
        toLowercase(searchWord);

        // Perform binary search
        int iterations = 0;
        int left = 0, right = wordCount - 1;
        int mid = -1;
        int index = -1;

        printf("\n%s\n", searchWord);
        while (left <= right) {
            iterations++;
            mid = left + (right - left) / 2;

            // Print the middle index
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

        // Print the final result
        printf(" (%d iterations) %s\n", iterations, (index != -1) ? "found" : "not found");
    }

    fclose(file);
    return 0;
}

// Function to convert a string to lowercase
void toLowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to remove punctuation from a string
void removePunctuation(char* str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!ispunct((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

// Function to sort an array of strings using insertion sort
void insertionSort(char** words, int n) {
    for (int i = 1; i < n; i++) {
        char *key = words[i];
        int j = i - 1;

        // Compare the words in a case-insensitive manner
        while (j >= 0 && strcmp(words[j], key) > 0) {
            words[j + 1] = words[j];
            j--;
        }
        words[j + 1] = key;
    }
}
