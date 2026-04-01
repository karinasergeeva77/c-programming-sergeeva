#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Структуры

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    union {
        double number;
        char identifier[64];
        char operator;
    } value;
    int line;
    int column;
} Token;

typedef enum {
    STATE_START,
    STATE_IN_NUMBER,
    STATE_IN_IDENTIFIER,
    STATE_IN_OPERATOR,
    STATE_ERROR,
    STATE_DONE
} LexerState;

typedef struct {
    const char *input;
    size_t pos;
    int line;
    int column;
    LexerState state;
} Lexer;

// Инициализация лексера

void lexer_init(Lexer *lex, const char *input) {
    if (lex == NULL) return;
    lex->input = input;
    lex->pos = 0;
    lex->line = 1;
    lex->column = 1;
    lex->state = STATE_START;
}

// Пропустить пробельные символы

void lexer_skip_whitespace(Lexer *lex) {
    if (lex == NULL) return;
    
    while (lex->input[lex->pos] != '\0') {
        char c = lex->input[lex->pos];
        if (c == ' ' || c == '\t') {
            lex->pos++;
            lex->column++;
        } else if (c == '\n') {
            lex->pos++;
            lex->line++;
            lex->column = 1;
        } else {
            break;
        }
    }
}

// Получить следующий токен

Token lexer_next_token(Lexer *lex) {
    Token token;
    token.type = TOKEN_ERROR;
    token.line = lex->line;
    token.column = lex->column;
    
    if (lex == NULL) return token;
    
    // Пропускаем пробелы
    lexer_skip_whitespace(lex);
    
    token.line = lex->line;
    token.column = lex->column;
    
    char c = lex->input[lex->pos];
    
    // Конец файла
    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    // Числа
    if (isdigit(c) || (c == '.' && isdigit(lex->input[lex->pos + 1]))) {
        lex->state = STATE_IN_NUMBER;
        
        char num_str[64];
        int i = 0;
        
        // Целая часть
        while (isdigit(lex->input[lex->pos])) {
            num_str[i++] = lex->input[lex->pos];
            lex->pos++;
            lex->column++;
        }
        
        // Дробная часть
        if (lex->input[lex->pos] == '.') {
            num_str[i++] = '.';
            lex->pos++;
            lex->column++;
            
            while (isdigit(lex->input[lex->pos])) {
                num_str[i++] = lex->input[lex->pos];
                lex->pos++;
                lex->column++;
            }
        }
        
        num_str[i] = '\0';
        token.type = TOKEN_NUMBER;
        token.value.number = atof(num_str);
        
        return token;
    }
    
    // Идентификаторы (буквы)
    if (isalpha(c)) {
        lex->state = STATE_IN_IDENTIFIER;
        
        char ident[64];
        int i = 0;
        
        while (isalnum(lex->input[lex->pos])) {
            ident[i++] = lex->input[lex->pos];
            lex->pos++;
            lex->column++;
        }
        
        ident[i] = '\0';
        token.type = TOKEN_IDENTIFIER;
        strcpy(token.value.identifier, ident);
        
        return token;
    }
    
    // Операторы и скобки
    lex->state = STATE_IN_OPERATOR;
    lex->pos++;
    lex->column++;
    
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
            token.type = TOKEN_OPERATOR;
            token.value.operator = c;
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            break;
        default:
            token.type = TOKEN_ERROR;
            break;
    }
    
    return token;
}

// Посмотреть следующий токен без извлечения

Token lexer_peek(Lexer *lex) {
    if (lex == NULL) {
        Token error = {TOKEN_ERROR, .line = 0, .column = 0};
        return error;
    }
    
    // Сохраняем состояние
    size_t saved_pos = lex->pos;
    int saved_line = lex->line;
    int saved_column = lex->column;
    LexerState saved_state = lex->state;
    
    // Получаем следующий токен
    Token token = lexer_next_token(lex);
    
    // Восстанавливаем состояние
    lex->pos = saved_pos;
    lex->line = saved_line;
    lex->column = saved_column;
    lex->state = saved_state;
    
    return token;
}

// Вывод токена

void token_print(const Token *tok) {
    if (tok == NULL) return;
    
    const char *type_str;
    switch (tok->type) {
        case TOKEN_NUMBER:     type_str = "NUMBER"; break;
        case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
        case TOKEN_OPERATOR:   type_str = "OPERATOR"; break;
        case TOKEN_LPAREN:     type_str = "LPAREN"; break;
        case TOKEN_RPAREN:     type_str = "RPAREN"; break;
        case TOKEN_EOF:        type_str = "EOF"; break;
        default:               type_str = "ERROR"; break;
    }
    
    printf("[%s] ", type_str);
    
    switch (tok->type) {
        case TOKEN_NUMBER:
            printf("%g", tok->value.number);
            break;
        case TOKEN_IDENTIFIER:
            printf("%s", tok->value.identifier);
            break;
        case TOKEN_OPERATOR:
            printf("%c", tok->value.operator);
            break;
        case TOKEN_LPAREN:
            printf("(");
            break;
        case TOKEN_RPAREN:
            printf(")");
            break;
        default:
            break;
    }
    
    printf(" (line %d, col %d)\n", tok->line, tok->column);
}

// Токенизация всей строки

size_t tokenize(const char *input, Token *tokens, size_t max_tokens) {
    if (input == NULL || tokens == NULL) return 0;
    
    Lexer lex;
    lexer_init(&lex, input);
    
    size_t count = 0;
    
    while (count < max_tokens) {
        Token tok = lexer_next_token(&lex);
        tokens[count] = tok;
        count++;
        
        if (tok.type == TOKEN_EOF || tok.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return count;
}

int main(void) {
    
    // Тестовые выражения
    const char *expressions[] = {
        "x = 3.14 * (y + 2)",
        "result = a + b - c * d / e",
        "sqrt(25) + pow(2, 3)",
        ""
    };
    
    for (int expr_idx = 0; expressions[expr_idx][0] != '\0'; expr_idx++) {
        printf("Вход: \"%s\"\n", expressions[expr_idx]);
        printf("Токены:\n");
        
        Lexer lex;
        lexer_init(&lex, expressions[expr_idx]);
        
        Token token;
        do {
            token = lexer_next_token(&lex);
            token_print(&token);
        } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
        
        printf("\n");
    }
    
    
    return 0;
}
