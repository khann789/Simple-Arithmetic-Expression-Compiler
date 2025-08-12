

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    IDENTIFIER,
    OPERATOR,
    SENTINEL,
    INVALID
} TokenType;

typedef struct {
    char lexeme[64]; // Max token length = 64
    TokenType type;
} Token;

Token tokens[32]; // Total amount of allowed tokens = 32
int tokenCount = 0;
int currentTokenIndex = 0;

char stack[128][64]; // Stack for storing lexemes (as integers)
char stac_chr[64]; // To store items being pushed to Stack (for displaying stack implementation table)
int top = -1; // Flag to see if string is accepted or not (last item must be $)
char inputStr[64]; // To store the input string
int identifierValues[32]; // To store the values of identifiers

void push(char *value) {
    if (top < 128 - 1) {
        strcpy(stack[++top], value);
    } else {
        fprintf(stderr, "Error: Stack overflow.\n");
        exit(1);
    }
}

char* pop() {
    if (top >= 0) {
        return stack[top--];
    } else {
        fprintf(stderr, "Error: Stack underflow.\n");
        exit(1);
    }
}

// Example function to pop the BOTTOM element of the stack:
char* popBottom() {
    // If the stack is empty, handle the error
    if (top < 0) {
        fprintf(stderr, "Error: Stack underflow.\n");
        exit(1);
    }

    // Temporarily store the bottom value in a static buffer
    // so we can return a pointer. (Static is used to keep it valid
    // after the function returns—alternatively, you could dynamically allocate.)
    static char bottomVal[64];
    strcpy(bottomVal, stack[0]);

    // Shift everything from index 1..top down to 0..top-1
    for (int i = 0; i < top; i++) {
        strcpy(stack[i], stack[i + 1]);
    }

    // Decrease 'top' since we removed one item
    top--;

    // Return the bottom element
    return bottomVal;
}

void reverseString(char *str) {
    
    int length = strlen(str);  // Get the length of the string
    int i;
    char temp;
    
    for (i = 0; i < length / 2; i++) {
        // Swap characters
        temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }

}

void syntaxError(char *msg) {
    printf("Syntax Error: %s\n", msg);
    printf("\nInput string is not accepted.\n");
    exit(1);
}

void printStack() {
    printf("\t\t|");
    for (int i = 0; i <= top; ++i) {
        printf(" %s", stack[i]);
    }
    printf("\n");
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int isValidIdentifier(char *id) {
    if (!isalpha(id[0]) && id[0] != '_') {
        return 0;
    }

    int len = strlen(id);
    for (int i = 1; i < len; ++i) {
        if (!isalnum(id[i]) && id[i] != '_') {
            return 0;
        }
    }

    return 1;
}

void lexicalAnalysis(char *input) {
    int index = 0;
    int invalidTokenFound = 0; // Flag
    char invalidToken = '\0';

    for (int i = 0; input[i] != '\0'; ++i) {
        char currentChar = input[i];

        if (isalpha(currentChar) || currentChar == '_') {
            tokens[tokenCount].type = IDENTIFIER;
            while (isalnum(currentChar) || currentChar == '_') {
                tokens[tokenCount].lexeme[index++] = currentChar;
                currentChar = input[++i];
            }
            tokens[tokenCount].lexeme[index] = '\0';
            index = 0;
            tokenCount++;
            i--;
        } else if (isOperator(currentChar)) {
            tokens[tokenCount].type = OPERATOR;
            tokens[tokenCount].lexeme[index++] = currentChar;
            tokens[tokenCount].lexeme[index] = '\0';
            index = 0;
            tokenCount++;
        } else if (currentChar == '$') {
            tokens[tokenCount].type = SENTINEL;
            tokens[tokenCount].lexeme[index++] = currentChar;
            tokens[tokenCount].lexeme[index] = '\0';
            index = 0;
            tokenCount++;
        } else {
            invalidTokenFound = 1;
            invalidToken = currentChar;
            break;
        }
    }

    for (int i = 0; i < tokenCount; ++i) {
        switch (tokens[i].type) {
            case IDENTIFIER:
                printf("Identifier: %s\n", tokens[i].lexeme);
                break;
            case OPERATOR:
                printf("Operator: %s\n", tokens[i].lexeme);
                break;
            case SENTINEL:
                printf("Sentinel: %s\n", tokens[i].lexeme);
                break;
            default:
                break;
        }
    }

    if (invalidTokenFound) {
        printf("Error! Invalid token '%c' encountered. Program terminated prematurely.\n", invalidToken);
        exit(1);
    }
}

void match(TokenType expectedType) {
    if (tokens[currentTokenIndex].type == expectedType) {
        if (tokens[currentTokenIndex].type == OPERATOR) {
            strncat(stac_chr, &(inputStr + currentTokenIndex)[0], 1);
            reverseString(stac_chr);
            printf("$%s\t\t\t%s\t\t\t%s", stac_chr ,inputStr + currentTokenIndex + 1, "Push");
            printf("\n");
            //printStack();
            currentTokenIndex++;
            //printf("\n");
        } else {
            currentTokenIndex++;
        }
    } else {
        syntaxError("Token mismatch");
    }
}

void factor() {
    if (tokens[currentTokenIndex].type == IDENTIFIER) {
        // Retrieve value from identifierValues array
        int value = identifierValues[currentTokenIndex];
        char valueStr[64];
        sprintf(valueStr, "%d", value);
        push(valueStr);
        if ((inputStr + currentTokenIndex)[0] != '$') {
            strncat(stac_chr, &(inputStr + currentTokenIndex)[0], 1);
            reverseString(stac_chr);
        } 
        printf("$%s\t\t\t%s\t\t\t%s", stac_chr ,inputStr + currentTokenIndex + 1, "Push");
        printf("\n");// 
        //printStack(); 
        // to delete one character when an identifier is pushed 
        int size = strlen(stac_chr);
        // Shift characters to the left
        for (int i = 0; i < size; i++) {
            stac_chr[i] = stac_chr[i + 1];
        }
        printf("$%s\t\t\t%s\t\t\t%s", stac_chr ,inputStr + currentTokenIndex + 1, "Pop");
        printStack();
        //printf("\n");
        match(IDENTIFIER);
    } else if (tokens[currentTokenIndex].type == SENTINEL) {
        match(SENTINEL);
    } else {
        syntaxError("Invalid factor");
    }
}

void DIV() {
    factor();

    while (currentTokenIndex < tokenCount && 
          (tokens[currentTokenIndex].type == OPERATOR &&
          (tokens[currentTokenIndex].lexeme[0] == '/'))) {

        char op = tokens[currentTokenIndex].lexeme[0];
        match(OPERATOR);
        factor();
        char *bStr = pop();
        char *aStr = pop();
        int b = atoi(bStr); // atoi = ASCII to integer
        int a = atoi(aStr); 

        int result;
        if (op == '/') {
            if (b == 0) {
                fprintf(stderr, "Error: Division by zero.\n");
                exit(1);
            }
            result = a / b;
        

        char resultStr[64];
        sprintf(resultStr, "%d", result);
        push(resultStr);
        // to delete one character when an identifier is pushed 
        int size = strlen(stac_chr);
        // Shift characters to the left
        for (int i = 0; i < size; i++) {
            stac_chr[i] = stac_chr[i + 1];
            }
        //reverseString(inputStr);// new
        printf("$%s\t\t\t%s\t\t\t%s", stac_chr, inputStr + currentTokenIndex, "Pop");
        printStack();
        //printf("\n");
        }
    }  
}

void MUL_or_DIV() {
    DIV();

    while (currentTokenIndex < tokenCount && 
          (tokens[currentTokenIndex].type == OPERATOR &&
          (tokens[currentTokenIndex].lexeme[0] == '*'))) {

        char op = tokens[currentTokenIndex].lexeme[0];
        match(OPERATOR);
        // Next is another DIV() call
        // So that if there's a factor with more divisions,
        // it gets processed first.
        DIV();

        // Now pop the two top values for multiplication
        char *bStr = pop();
        char *aStr = pop();
        int b = atoi(bStr);
        int a = atoi(aStr);

        int result;
        if (op == '*') {
            result = a * b;
        } 
        char resultStr[64];
        sprintf(resultStr, "%d", result);
        push(resultStr);
        // to delete one character when an identifier is pushed 
        int size = strlen(stac_chr);
        // Shift characters to the left
        for (int i = 0; i < size; i++) {
            stac_chr[i] = stac_chr[i + 1];
        }
        //reverseString(inputStr);// new
        printf("$%s\t\t\t%s\t\t\t%s", stac_chr, inputStr + currentTokenIndex, "Pop");
        printStack();
        //printf("\n");
    }
}

void ADD_or_SUB() {
    MUL_or_DIV();

    while (currentTokenIndex < tokenCount && 
          (tokens[currentTokenIndex].type == OPERATOR &&
          (tokens[currentTokenIndex].lexeme[0] == '+' ||
          tokens[currentTokenIndex].lexeme[0] == '-'))) {

        char op = tokens[currentTokenIndex].lexeme[0];
        match(OPERATOR);
        MUL_or_DIV();
        char *bStr = pop();
        char *aStr = pop();
        int b = atoi(bStr);
        int a = atoi(aStr);

        int result;
        if (op == '+') {
            result = a + b;
        } else {
            result = a - b;
        }

        char resultStr[64];
        sprintf(resultStr, "%d", result);
        push(resultStr);
        // to delete one character when an identifier is pushed 
        int size = strlen(stac_chr);
        // Shift characters to the left
        for (int i = 0; i < size; i++) {
            stac_chr[i] = stac_chr[i + 1];
        }
        //reverseString(inputStr);// new
        printf("$%s\t\t\t%s\t\t\t%s", stac_chr, inputStr + currentTokenIndex, "Pop");
        printStack();
        //printf("\n");
    }

    if (currentTokenIndex < tokenCount && tokens[currentTokenIndex].type == SENTINEL) {
        match(SENTINEL);
    } else {
        syntaxError("Invalid expression");
    }
}

void syntaxAnalysis(Token *tokens, int tokenCount,char *lhs) {
    printf("\nEnter integer values of the following identifiers:\n");
    for (int i = 0; i < tokenCount; ++i) {
        if (tokens[i].type == IDENTIFIER) {
            int value;
            printf("Value of %s: ", tokens[i].lexeme);
            if (scanf("%d", &value) != 1) {
                syntaxError("Invalid numerical value. Please enter a valid number.");
            }
            identifierValues[i] = value;
        }
    }

    
    printf("\n\nStack\t\t\tStack2\t\t\tAction\t\tStack3\n----------------------------------------------------------------------------------\n$\t\t\t%s\t\t\tNone\n",inputStr);


    ADD_or_SUB();

    if (top == 0) {
        printf("$\t\t\t$\t\t\tAccepted\n");
        printf("\nThe result of given expression is:\n");
        printf("%s %s\n",lhs,inputStr);
        printf("%s %s",lhs,stack[top]);
    } else {
        fprintf(stderr, "Error: Invalid expression.\n");
        printf("\nInput string is not accepted.\n");
        exit(1);
    }
}

// phase 2 fucntion
void checkLHS(char *lhs) {
    // Checking requirements of lhs

    if (strlen(lhs) > 6) {
        printf("Number of characters in <id> is more than 5\n");
        exit(1);
    }
    if (isdigit(lhs[0])) {
        printf("<id> cannot start with a digit\n");
        exit(1);
    }
}

// Phase 1: Preprocessing
void preprocess(const char *input, char *output) {
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isspace((unsigned char)input[i])) {
            continue; // Skip spaces
        }
        if (input[i] == ';') {
            output[j++] = '$'; // Replace ';' with '$'
            continue;
        }
        output[j++] = input[i];
    }
    output[j] = '\0'; // Null-terminate the string
}



int main() {
    
    char inputString[200];
    char lhs[200];
    // Input the arithmetic expression
    printf("Enter an arithmetic expression : ");
    if (!fgets(inputString, sizeof(inputString), stdin)) {
        printf("Error: Unable to read input.\n");
        return 1;
    }

    // Phase 1: Preprocessing
    char preprocessed[200];
    preprocess(inputString, preprocessed);
    printf("\n--- Phase 1: Preprocessing Output ---\n");
    printf("%s\n", preprocessed);
    printf("\n--- Lexical Analysis output:w ---\n");


    // Find the position of '=' in the preprocessed string
    int eqPos = -1;
    for (int i = 0; preprocessed[i] != '\0'; i++) {
        if (preprocessed[i] == '=') {
            eqPos = i;
            break;
        }
    }

    // If '=' found, separate LHS (plus '=') into lhs[] and remove from preprocessed
    if (eqPos != -1) {
        // Copy everything up to and including '=' into lhs[]
        strncpy(lhs, preprocessed, eqPos + 1);
        lhs[eqPos + 1] = '\0';  // null-terminate

        // Shift the remainder (the RHS) to the beginning of preprocessed
        // eqPos+1 is the first character after '='
        int j = 0;
        for (int i = eqPos + 1; preprocessed[i] != '\0'; i++) {
            preprocessed[j++] = preprocessed[i];
        }
        preprocessed[j] = '\0';  // null-terminate
    }

    
    checkLHS(lhs);

    lexicalAnalysis(preprocessed);

    strcpy(inputStr,preprocessed);
    //reverseString(inputStr);
    syntaxAnalysis(tokens, tokenCount,lhs);

    return 0;
}

