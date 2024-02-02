// implement simple lexical analyser for c program compiler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isArithmeticOperator(const char* sequence) {
    const char* operators[] = {"+", "-", "*", "/", "^", "%"};
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (strcmp(sequence, operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isAssignmentOperator(const char* sequence) {
    const char* operators[] = {"=","+=","-=","/=","*="};
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (strcmp(sequence, operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}


int isRelationalOperator(const char* sequence) {
    const char* operators[] = {"==", "!=", "<", "<=", ">", ">="};
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (strcmp(sequence, operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main()
{
    FILE *fp, *fop, *fsym, *fnum;
    int ch;
    char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default",
                        "do", "double", "else", "enum", "extern", "float", "for", "goto",
                        "if", "int", "long", "register", "return", "short", "signed",
                        "sizeof", "static", "struct", "switch", "typedef", "union",
                        "unsigned", "void", "volatile", "while"};
    char *operators[] = {"+", "-", "*", "/", "%", "=", "==", ">", "<", ">=", "<=", "++", "--", "!="};
    char *separators[] = {",", ";", "{", "}", "[", "]", "(", ")"};
    char *delimiters[] = {" ", "\t", "\n", "\0", "\b"}; // compare with ascii value
    fp = fopen("source.txt", "r");
    fop = fopen("operator.txt", "w");
    fsym = fopen("symbol.txt", "w");
    fnum = fopen("number.txt", "w");
    if (fp == NULL || fop == NULL || fsym == NULL || fnum == NULL)
    {
        printf("Error opening file\n");
        exit(0);
    }


    
    int state = 0;
    // char buffer[30];
    char* buffer = (char*)malloc(30 * sizeof(char));
    int buf_idx = 0;

    int keywordflag = 0;
    
    int OpIndex = 1;
    int SymIndex = 1;
    int NumIndex = 1;

    while((ch = fgetc(fp)) != EOF) {
        switch (state){
            case 0:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 0\n");
                if (ch == 32 || ch == 9 || ch == 10 || ch == 0 || ch == 8 || ch == 92){ // ascii values of space, tab, newline, null, backspace, backslash
                    state = 1;
                }
                else if (isalpha(ch)){
                    buffer[buf_idx++] = ch;
                    state = 3;
                }
                else if (isdigit(ch)){
                    buffer[buf_idx++]=ch;
                    state = 5;
                }
                else if (ch == '<' || ch == '>' || ch == '!' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '='){
                    fseek(fp, -1, SEEK_CUR);
                    state = 14;
                }
                else if (ch == ',' || ch == ';' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == '(' || ch == ')'){
                    state = 0;
                }
                else{
                    printf("Invalid character %c\n", ch);
                    exit(0);
                }
                break;
            case 1:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 1\n");
                if (ch == 32 || ch ==32){ // ascii value of space
                    state = 1; // 1 = 1
                }
                else if(ch == 92){
                    fseek(fp, 1, SEEK_CUR);
                }
                else{
                    state = 0;
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 3:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 3\n");
                if (isalnum(ch)){
                    buffer[buf_idx++] = ch;
                    state = 3;
                }
                else{
                    state = 0;
                    // check if buffer has a keyword
                    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++){
                        if (strcmp(buffer, keywords[i]) == 0){
                            keywordflag = 1;
                            break;
                        }
                    }
                    if (keywordflag == 1){
                        fprintf(fsym, "\t%d\t\t\t%s\t\t\t%s\n", SymIndex++, buffer, buffer);
                        keywordflag = 0;
                    }
                    else{
                        fprintf(fsym, "\t%d\t\t\t%s\t\t\tId\n", SymIndex++, buffer);
                    }
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 5:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 5\n");
                if (isdigit(ch)){
                    buffer[buf_idx++] = ch;
                    state = 5;
                }
                else if (ch == '.'){
                    buffer[buf_idx++] = ch;
                    state = 6;
                }
                else if (ch == 'e' || ch == 'E'){
                    buffer[buf_idx++] = ch;
                    state = 8;
                }
                else{
                    state = 0;
                    fprintf(fnum, "\t%d\t\t\t%s\t\t\tNum\n", NumIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 6:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 6\n");
                if (isdigit(ch)){
                    buffer[buf_idx++] = ch;
                    state = 7;
                }
                else{
                    printf("Invalid character %c\n", ch);
                    exit(0);
                }
                break;
            case 7:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 7\n");
                if (isdigit(ch)){
                    buffer[buf_idx++] = ch;
                    state = 7;
                }
                else if (ch == 'e' || ch == 'E'){
                    buffer[buf_idx++] = ch;
                    state = 8;
                }
                else{
                    state = 0;
                    fprintf(fnum, "\t%d\t\t\t%s\t\t\tNum\n", NumIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 8:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 8\n");
                if (ch == '+' || ch == '-'){
                    buffer[buf_idx++] = ch;
                    state = 9;
                }
                else if (isdigit(ch)){
                    buffer[buf_idx++] = ch;
                    state = 10;
                }
                else{
                    printf("Invalid character %c\n", ch);
                    exit(0);
                }
                break;
            case 9:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 9\n");
                if (isdigit(ch)){
                    buffer[buf_idx++] = ch;
                    state = 10;
                }
                else{
                    printf("Invalid character %c\n", ch);
                    exit(0);
                }
                break;
            case 10:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 10\n");
                if (isdigit(ch)){
                    buffer[buf_idx++] = ch;
                    state = 10;
                }
                else{
                    state = 0;
                    fprintf(fnum, "\t%d\t\t\t%s\t\t\tNum\n", NumIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 14:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 14\n");
                if (ch == '<'){
                    buffer[buf_idx++] = ch;
                    state = 15;
                }
                else if (ch == '='){
                    buffer[buf_idx++] = ch;
                    state = 18;                 
                }
                else if (ch == '>'){
                    buffer[buf_idx++] = ch;
                    state = 21;
                }
                else if (ch == '+'){
                    buffer[buf_idx++] = ch;
                    state = 24;
                }
                else if (ch == '-'){
                    buffer[buf_idx++] = ch;
                    state = 28;
                }
                else if (ch == '*'){
                    buffer[buf_idx++] = ch;
                    state = 32;
                }
                else if (ch == '/'){
                    buffer[buf_idx++] = ch;
                    state = 34;
                }
                else if (ch == '!'){
                    buffer[buf_idx++] = ch;
                    state = 36;
                }
                else if (ch == '%'){
                    buffer[buf_idx++] = ch;
                    state = 38;
                }
                else{
                    printf("Invalid character %c\n", ch);
                    exit(0);
                }
                break;
            case 15:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 15\n");
                if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tRelOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else{
                    state = 0;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tRelOp\n", OpIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 18:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 18\n");
                if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tRelOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else{
                    state = 0;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tAssOp\n", OpIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 21:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 21\n");
                if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tRelOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else{
                    state = 0;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tRelOp\n", OpIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 24:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 24\n");
                if (ch == '+'){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tArOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tAssOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else{
                    state = 0;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tArOp\n", OpIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 28:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 28\n");
                if (ch == '-'){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tArOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tAssOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else{
                    state = 0;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tArOp\n", OpIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 32:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 32\n");
                if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tAssOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else{
                    state = 0;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tArOp\n", OpIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 34:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 34\n");
                if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tAssOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else if (ch == '/'){
                    state = 0;
                    fscanf(fp, "%*[^\n]");
                }
                else if (ch == '*'){
                    state = 0;
                    while((ch = fgetc(fp)) != EOF){
                        if (ch == '*'){
                            if ((ch = fgetc(fp)) == '/'){
                                break;
                            }
                        }
                    }
                }
                else{
                    state = 0;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tArOp\n", OpIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;
            case 36:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 36\n");
                if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tRelOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else{
                    printf("Invalid character %c\n", ch);
                    exit(0);
                }
                break;
            case 38:
                buffer[buf_idx] = '\0';
                printf("%s\n", buffer);
                printf("State 38\n");
                if (ch == '='){
                    buffer[buf_idx++] = ch;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tAssOp\n", OpIndex++, buffer);
                    state = 0;
                }
                else{
                    state = 0;
                    fprintf(fop, "\t%d\t\t\t%s\t\t\tArOp\n", OpIndex++, buffer);
                    fseek(fp, -1, SEEK_CUR);
                }
                break;    
        }
        if (state == 0){
            for (int i = 0; i < 30; i++){
                buffer[i] = '\0';
                buf_idx = 0;
            }
        }
    }
    
    return 0;
}