#include <stdio.h>
#include <string.h>


typedef enum {
    TOKEN_IF,
    TOKEN_LPAREN, // (
    TOKEN_RPAREN, // )
    TOKEN_PRINT,
    TOKEN_SEMICOLON, // ;
    TOKEN_EXPRESSION,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;


typedef struct {
    TokenType type;
    const char *text;
} Token;


Token tokens[] = {
    {TOKEN_IF, "if"},
    {TOKEN_LPAREN, "("},
    {TOKEN_EXPRESSION, "EXPRESSION"},
    {TOKEN_RPAREN, ")"},
    {TOKEN_PRINT, "print"},
    {TOKEN_EXPRESSION, "EXPRESSION"},
    {TOKEN_UNKNOWN, "wrong"},
    {TOKEN_SEMICOLON, ";"},
    {TOKEN_PRINT, "print"},
    {TOKEN_EXPRESSION, "EXPRESSION"},
    {TOKEN_SEMICOLON, ";"},
    {TOKEN_EOF, ""}
};
int currentTokenIndex = 0;


void parse_statement();
void parse_expression();
void error(const char *message);
void recover();
Token getCurrentToken();
void advanceToken();


int main() {
    parse_statement();
    return 0;
}


Token getCurrentToken() {
    return tokens[currentTokenIndex];
}


void advanceToken() {
    if (getCurrentToken().type != TOKEN_EOF) {
        currentTokenIndex++;
    }
}


void match(TokenType expected) {
    if (getCurrentToken().type == expected) {
        advanceToken();
    } else {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Expected '%s', but found '%s'",
                 tokens[expected].text, getCurrentToken().text);
        error(buffer);
    }
}


void parse_statement() {
    Token current = getCurrentToken();
    switch (current.type) {
        case TOKEN_IF:
            match(TOKEN_IF);
            match(TOKEN_LPAREN);
            parse_expression();
            match(TOKEN_RPAREN);
            parse_statement();
            break;
        case TOKEN_PRINT:
            match(TOKEN_PRINT);
            parse_expression();
            match(TOKEN_SEMICOLON);
            break;
        default:
            error("Unexpected token in statement.");
            recover();
            break;
    }
}


void parse_expression() {
    if (getCurrentToken().type == TOKEN_EXPRESSION) {
        advanceToken();
    } else {
        error("Expected an expression.");
        recover();
    }
}
// Handle errors
void error(const char *message) {
    printf("Syntax Error: %s at token '%s'\n", message, getCurrentToken().text);
}


void recover() {
    while (getCurrentToken().type != TOKEN_SEMICOLON && getCurrentToken().type != TOKEN_EOF) {
        advanceToken();
    }
    if (getCurrentToken().type == TOKEN_SEMICOLON) {
        advanceToken(); // Skip the semicolon
    }
}
