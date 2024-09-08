#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char **lexFunc(FILE *fptr, char *wordToken, char **tokenList, int dynamicWordSize, int dynamicTokenSize) {
    // if file was sucessfully opened
    int charCount = 0;
    int strSizeCount = 0;
    int listSizeCount = 0;

    do
    {
        char c = fgetc(fptr);
        // checking for end of file
        if (feof(fptr)) {
            if (strSizeCount == dynamicWordSize) {
                dynamicWordSize += 1;
                wordToken = (char *)realloc(wordToken, dynamicWordSize * sizeof(char));
                wordToken[strSizeCount] = '\0';
            }
            else {
                wordToken[strSizeCount] = '\0';
            }
            if (listSizeCount == dynamicTokenSize) {
                dynamicTokenSize += 2;
                tokenList = (char **)realloc(tokenList, dynamicTokenSize * sizeof(char *));
                tokenList[listSizeCount++] = wordToken;
                tokenList[listSizeCount] = NULL;
            }
            else {
                dynamicTokenSize += 1;
                tokenList = (char **)realloc(tokenList, dynamicTokenSize * sizeof(char *));
                tokenList[listSizeCount++] = wordToken;
                tokenList[listSizeCount] = NULL;
            }
            break;
        }

        if ((c == '\n' || c == '\t' || c == ' ') && charCount) {
            // check if current character will cause a memory allocated
            // error for wordToken       
            if (strSizeCount == dynamicWordSize) {
                dynamicWordSize += 1;
                wordToken = (char *)realloc(wordToken, dynamicWordSize * sizeof(char));
                wordToken[strSizeCount] = '\0';
            }
            else {
                wordToken[strSizeCount] = '\0';
            }

            // check if current wordToken will cause a memory allocated
            // error for tokenList
            if (listSizeCount == dynamicTokenSize) {
                dynamicTokenSize += 1;
                tokenList = (char **)realloc(tokenList, dynamicTokenSize * sizeof(char *));
                tokenList[listSizeCount++] = wordToken;
            }
            else {
                tokenList[listSizeCount++] = wordToken;
            }

            charCount = 0;
            strSizeCount = 0;
            dynamicWordSize += 5;
            // make a new wordToken
            wordToken = (char *)malloc(dynamicWordSize * sizeof(char));
        }
        else if (c == '\n' || c == '\t' || c == ' ') {
            continue;
        }
        else if (c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == '[' || c == ']') {
            bool enteredstrSize = false;
            if (strSizeCount) {
                if (strSizeCount == dynamicWordSize) {
                    dynamicWordSize += 1;
                    wordToken = (char *)realloc(wordToken, dynamicWordSize * sizeof(char));
                    wordToken[strSizeCount] = '\0';
                }
                else {
                    wordToken[strSizeCount] = '\0';
                }
                enteredstrSize = true;           
            }
            // get special char
            char *gottenChar = (char *)malloc(2 * sizeof(char));
            gottenChar[0] = c;
            gottenChar[1] = '\0';

            // increase token size by adding 2 to put gotten char and/or wordToken
            dynamicTokenSize += 2;
            tokenList = (char **)realloc(tokenList, dynamicTokenSize * sizeof(char *));
            if (enteredstrSize) {
                tokenList[listSizeCount++] = wordToken;
            }
            tokenList[listSizeCount++] = gottenChar;

            charCount = 0;
            strSizeCount = 0;
            wordToken = (char *)malloc(dynamicWordSize * sizeof(char));
        }
        else {
            if (strSizeCount == dynamicWordSize) {
                dynamicWordSize += 5;
                wordToken = (char *)malloc(dynamicWordSize * sizeof(char));
                wordToken[strSizeCount++] = c;
            }
            else {
                wordToken[strSizeCount++] = c;
            }
            charCount++;
        }
    } while(1);

    return tokenList;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Command Line argument should be one file");
    }
    char *filename = argv[1];

    // create a file pointer to store the memory address of another file
    // pointer returned by the fopen()
    FILE* fptr;

    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("The file couldn't be found or opened due to permissions");
        printf("Program will now exit.");
        return 1;
    }


    int dynamicWordSize = 10;
    int dynamicTokenSize = 10;
    char *wordToken = (char *)malloc(dynamicWordSize * sizeof(char));
    char **tokenList = (char **)malloc(dynamicTokenSize * sizeof(char *));

    if (wordToken == NULL) {
        printf("Unable to allocate memory for wordToken.");
        return 1;
    }
    if (tokenList == NULL) {
        printf("Unable to allocate memory for tokenList.");
        return 1;
    }

    char **rtrToken = lexFunc(fptr, wordToken, tokenList, dynamicWordSize, dynamicTokenSize);
    int tokenIndex = 0;

    while (rtrToken[tokenIndex] != NULL) {
        printf("%s\n", rtrToken[tokenIndex++]);
    }
    free(rtrToken);
    free(wordToken);
    free(tokenList);
}