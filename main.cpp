// Created by: Anthony Heitzeberg BSCPE-V
// Finished On: Feb. 15, 2020

#include <cstdio>
#include <iostream>
#include <cstring>
#include <regex>
#include <cstdlib>
#include <string>
#include <fstream>

// contains most common keywords of C++ language
bool isKeyword(char* str){
    return !strcmp(str, "if") || !strcmp(str, "else") ||
           !strcmp(str, "while") || !strcmp(str, "do") ||
           !strcmp(str, "break") || !strcmp(str, "continue") ||
           !strcmp(str, "int") || !strcmp(str, "double") || !strcmp(str, "float") ||
           !strcmp(str, "return") || !strcmp(str, "char") || !strcmp(str, "case") ||
           !strcmp(str, "char") || !strcmp(str, "sizeof") || !strcmp(str, "long") ||
           !strcmp(str, "short") || !strcmp(str, "typedef") || !strcmp(str, "switch") ||
           !strcmp(str, "unsigned") || !strcmp(str, "void") || !strcmp(str, "static") ||
           !strcmp(str, "struct") || !strcmp(str, "goto") || !strcmp(str, "main") ||
           !strcmp(str, "std") || !strcmp(str, "using") || !strcmp(str, "namespace") ||
           !strcmp(str, "cout") || !strcmp(str, "cin");
}

// must not start with number, and only contain numbers, letters and/or underscores
// regex pattern: [_a-zA-Z][_a-zA-Z0-9
bool isVariable(char* str){
    std::regex ident("[_a-zA-Z][_a-zA-Z0-9]*");
    return std::regex_match(str, ident);
}

// must be any input with double quotations around it
// UPDATE: must also take whitespaces as well in between "<string>"
// regex pattern: "([^"]*)"
bool isString(char* str){
    std::string string = str;
    std::regex s("\"(.*?)\"");
    return std::regex_match(str, s);
}

//must be any input with single quotations around it, should be only 3 chars long, '<char>'
//regex pattern: ("\'(.{1})\'")
bool isChar(char* str){
    std::regex c("\'(.{1})\'");
    return std::regex_match(str, c);
}

// this includes, int, floats and other number systems
// regex pattern: [-+]?([0-9]*\.[0-9]+|[0-9]+)
bool isNum(char* str){
    std::regex i("[-+]?([0-9]*\\.[0-9]+|[0-9]+)|0[xX][0-9a-fA-F]+");
    return std::regex_match(str, i);
}

// all operation symbols
// regex pattern: ([<>]?=|==|>|<|=|\&\&|\|\|)|(-|\+)?|(\*|/)
bool isSymbol(char* str){
    std::regex symb(R"(([!]?=|[<>]?=|==|!|>|%|<|\&\&|\|\|)|(-|\+)?|(\*|/)|[<>]?[<>])");
    return std::regex_match(str, symb);
}

bool isWhiteSpace(char ch){
    if(ch == ' ' || ch == '\n' || ch == '\0'){
        return true;
    }
    else{
        return false;
    }
}

bool isDelimiter(char ch)
{
    return ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
           ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
           ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
           ch == '[' || ch == ']' || ch == '{' || ch == '}';
}

bool isOperator(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' ||
           ch == '/' || ch == '>' || ch == '<' ||
           ch == '=' || ch == '!';
}

char* subStringGetter(const char* str, int left, int right){
    int iter;
    char* subStr = (char*)malloc(sizeof(char) * (right - left + 2));

    for(iter = left; iter <= right; iter++){
        subStr[iter - left] = str[iter];
    }
    subStr[right - left + 1] = '\0';
    return (subStr);
}

void parseAndScanInputs(char* str){
    int left = 0, right = 0;
    int len = strlen(str);
    while(right <= len && left <= right){
        if (!isDelimiter(str[right])){
            right++;
        }
        if (isDelimiter(str[right]) && left == right) {
            // printf("%c = delimiter\n", str[right]);
            if((isOperator(str[right]) && isOperator(str[right+1]))){
                char* subStr = subStringGetter(str, left, left + 1);
                if (isSymbol(subStr))
                printf("%s = operator\n", subStr);
                right++;
                }
            else if (isOperator(str[right]) && !isOperator(str[right+1]))
                printf("%c = operator\n", str[right]);
            right++;
            left = right;
        } else if ((isDelimiter(str[right]) && left != right)
                   || (right == len && left != right)) {
            char* subStr = subStringGetter(str, left, right - 1);

            if (isKeyword(subStr))
                printf("%s = reserve word\n", subStr);

            else if (isNum(subStr))
                printf("%s = number\n", subStr);

            else if(str[left] == '"'){
                int i = 1;
                while(str[left+i] != '"'){
                    i++;
                }
                char* s_subStr = subStringGetter(str, left, left + i);
                if (isString(s_subStr))
                    printf("%s = string\n", s_subStr);
            }
            else if (isVariable(subStr)
                     && !isDelimiter(str[right - 1]))
                printf("%s = variable\n", subStr);

            else if (isString(subStr)
                     && !isDelimiter(str[right - 1]))
                printf("%s = string\n", subStr);

            else if (isChar(subStr)
                     && !isDelimiter(str[right - 1]))
                printf("%s = character\n", subStr);

            else if (str[right] == '=' && isOperator(str[right - 1])){
                char* subStr = subStringGetter(str, left, right);
                if (isSymbol(subStr))
                    printf("%s = operator\n", subStr);
                right++;
            }

            left = right;
        }
    }
}


int main() {
    std::string str;
    std::ifstream in("program.txt");
    std::cout << "---BASIC LEXICAL ANALYSIS---" << std::endl;
    if(!in) {
        std::cout << "Cannot open input file.\n";
        return 1;
    }
    while (std::getline(in, str)) {
        // output the line
        char input[str.size() + 1];
        strcpy(input, str.c_str());
        parseAndScanInputs(input);
    }
    in.close();
    return 0;
}
