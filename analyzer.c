#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define IDENT 10
#define INT_LIT 11
#define FLOAT_LIT 12

#define VOID_CODE 26
#define MAIN_CODE 27
#define IN_CODE 28
#define DEFAULT_CODE 29
#define SWITCH_CODE 30
#define CASE_CODE 31
#define FOREACH_CODE 32
#define FOR_CODE 33
#define WHILE_CODE 34
#define DO_CODE 35
#define IF_CODE 36
#define ELSE_CODE 37
#define RETURN_CODE 38
#define BREAK_CODE 39

#define TILDE 40
#define EXCLAMATION_MARK 41
#define NUMBER_SIGN 42
#define DOLLAR_SIGN 43
#define PERCENT_SIGN 44
#define CARET 45
#define AMPERSAND 46
#define ASTERISK 47
#define LEFT_PAREN 48
#define RIGHT_PAREN 49
#define UNDERSCORE 50
#define PLUS_SIGN 51
#define VERTICAL_BAR 52
#define BACKSLASH 53
#define APOSTROPHE 54
#define MINUS_SIGN 55
#define EQUAL_SIGN 56
#define LEFT_BRACE 57
#define RIGHT_BRACE 58
#define LEFT_BRACKET 59
#define RIGHT_BRACKET 60
#define COLON 61
#define QUOTATION_MARK 62
#define SEMICOLON 63
#define OPENING_ANGLE 64
#define CLOSING_ANGLE 65
#define QUESTION_MARK 66
#define COMMA 67
#define PERIOD 68
#define SLASH 69

int main()
{
    if ((in_fp = fopen("program.rp", "r")) == NULL)
        printf("ERROR - cannot open program.rp \n");
    else
    {
        getChar();
        lex();
        program();
    }
    return 0;
}

void error(char* expected)
{
    printf("Expected '%s', got '%s' instead. \n", expected, lexeme);
    exit(1);
}

void enter_msg(char* name)
{
    printf("Enter <%s>\n", name);
}

void exit_msg(char* name)
{
    printf("Exit <%s>\n", name);
}

// <program> -> VOID MAIN ‘(‘ ‘)’ <block>
void program()
{
    enter_msg("program");

    if (nextToken == VOID_CODE) {
        lex();
        if (nextToken == MAIN_CODE) {
            lex();
            if (nextToken == LEFT_PAREN) {
                lex();
                if (nextToken == RIGHT_PAREN) {
                    lex();
                    block();
                }
                else
                {
                    error(")");
                }
            }
            else
            {
                    error("(");
            }
        }
        else
        {
            error("MAIN");
        }
    }
    else
    {
        error("VOID");
    }

    exit_msg("program");
}

// <block> -> ‘{‘ { <statement> ; } ‘}’
void block()
{
    enter_msg("block");

    if (nextToken == LEFT_BRACE)
    {
        lex();
        while (nextToken != RIGHT_BRACE)
        {
            statement();
            if (nextToken == SEMICOLON)
            {
                lex();
            }
            else
            {
                error(";");
            }
        }
    }
    else
    {
        error("{");
    }

    exit_msg("block");
}

// <statement> -> ( <switch> | <foreach> | <for> | <while> | <do_while> |
//                  <if> | <assignment> | <return> )
void statement()
{
    enter_msg("statement");

    if (nextToken == SWITCH_CODE)
    {
        switch_stmt();
    }
    else if (nextToken == FOREACH_CODE)
    {
        foreach_stmt();
    }
    else if (nextToken == FOR_CODE)
    {
        for_stmt();
    }
    else if (nextToken == WHILE_CODE)
    {
        while_stmt();
    }
    else if (nextToken == DO_CODE)
    {
        do_stmt();
    }
    else if (nextToken == IF_CODE)
    {
        if_stmt();
    }
    else if (nextToken == IDENT)
    {
        assignment_stmt();
    }
    else if (nextToken == RETURN_CODE)
    {
        return_stmt();
    }
    else
    {
        error("switch | foreach | for | while | do | if | identifier | return");
    }

    exit_msg("statement");
}

// <switch> -> switch ‘(‘ <factor> ‘)’ <switch_block>
void switch_stmt()
{
    enter_msg("switch");

    lex();
    if (nextToken == LEFT_PAREN)
    {
        lex();
        factor();
        if (nextToken == RIGHT_PAREN)
        {
            lex();
            switch_block();
            lex();
        }
        else
        {
            error(")");
        }
    }
    else
    {
        error("(");
    }

    exit_msg("switch");
}
// <foreach> -> foreach ‘(‘ id in id ‘)’ <block>
void foreach_stmt()
{
    enter_msg("foreach");

    lex();
    if (nextToken == LEFT_PAREN) {
        lex();
        if (nextToken == IDENT) {
            lex();
            if (nextToken == IN_CODE)
            {
                lex();
                if (nextToken == IDENT) {
                    lex();
                    if (nextToken == RIGHT_PAREN)
                    {
                        lex();
                        block();
                        lex();
                    }
                    else
                    {
                        error(")");
                    }
                }
                else
                {
                    error("identifier");
                }
            }
            else
            {
                error("in");
            }
        }
        else
        {
            error("identifier");
        }
    }
    else
    {
        error("(");
    }

    exit_msg("foreach");
}

// <for> -> for ‘(‘ [<statement_list>] ; [<bool_expr>] ; [<statement_list>] ‘)’ <block>
void for_stmt()
{
    enter_msg("for");

    lex();
    if (nextToken == LEFT_PAREN) {
        lex();
        if (nextToken != SEMICOLON)
        {
            statement_list();
        }
        if (nextToken == SEMICOLON)
        {
            lex();
            if (nextToken != SEMICOLON)
            {
                bool_expr();
            }
            if (nextToken == SEMICOLON)
            {
                lex();
                if (nextToken != RIGHT_PAREN)
                {
                    statement_list();
                }
                if (nextToken == RIGHT_PAREN)
                {
                    lex();
                    block();
                    lex();
                }
                else
                {
                    error(")");
                }
            }
            else
            {
                error(";");
            }
        }
        else
        {
            error(";");
        }
    }
    else
    {
        error("(");
    }

    exit_msg("for");
}

// <while> -> while ‘(‘ <bool_expr> ‘)’ <block>
void while_stmt()
{
    enter_msg("while");

    lex();
    if (nextToken == LEFT_PAREN) {
        lex();
        bool_expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
            block();
            lex();
        }
        else
        {
            error(")");
        }
    }
    else
    {
        error("(");
    }

    exit_msg("while");
}

// <do_while> -> do <block> while ‘(‘ <bool_expr> ‘)’
void do_stmt()
{
    enter_msg("do_while");

    lex();
    block();
    lex();
    if (nextToken == WHILE_CODE) {
        lex();
        if (nextToken == LEFT_PAREN) {
            lex();
            bool_expr();
            if (nextToken == RIGHT_PAREN) {
                lex();
            }
            else
            {
                error(")");
            }
        }
        else
        {
            error("(");
        }
    }
    else
    {
        error("while");
    }

    exit_msg("do_while");
}

// <if> -> if ‘(‘ bool_expr ‘)’ <block> [else <block>]
void if_stmt()
{
    enter_msg("if");

    lex();
    if (nextToken == LEFT_PAREN) {
        lex();
        bool_expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
            block();
            lex();
            if (nextToken == ELSE_CODE) {
                lex();
                block();
                lex();
            }
        }
        else
        {
            error(")");
        }
    }
    else
    {
        error("(");
    }

    exit_msg("if");
}

// <assignment> -> id = <expr>
void assignment_stmt()
{
    enter_msg("assignment");

    lex();
    if (nextToken == EQUAL_SIGN)
    {
        lex();
        expr();
    }
    else
    {
        error("=");
    }

    exit_msg("assignment");
}

// <return> -> return [<expr>]
void return_stmt()
{
    enter_msg("return");

    lex();
    if (nextToken != SEMICOLON)
    {
        expr();
    }

    exit_msg("return");
}

// <switch_block> -> ‘{‘ { <case_block> } ‘}’
void switch_block()
{
    enter_msg("switch_block");

    if (nextToken == LEFT_BRACE)
    {
        lex();

        while (nextToken != RIGHT_BRACE)
        {
            case_block();
        }
    }
    else
    {
        error("{");
    }
    
    exit_msg("switch_block");
}

// <case_block> -> ( case <factor> | default ): { <statement> ; } break;
void case_block()
{
    enter_msg("case_block");

    if (nextToken == CASE_CODE)
    {
        lex();
        factor();
    }
    else if (nextToken == DEFAULT_CODE)
    {
        lex();
    }
    else
    {
        error("case | default");
    }
    if (nextToken == COLON)
    {
        lex();
        while (nextToken != BREAK_CODE)
        {
            statement();
            if (nextToken == SEMICOLON)
            {
                lex();
            }
            else
            {
                error(";");
            }
        }
        lex();
        if (nextToken == SEMICOLON)
        {
            lex();
        }
        else
        {
            error(";");
        }
    }
    else
    {
        error(":");
    }
    
    exit_msg("case_block");
}

// <statement_list> -> ( <assignment> | <unary> ) [ {, ( <assignment> | <unary> )  }]
void statement_list()
{
    enter_msg("statement_list");

    if (nextToken == IDENT)
    {
        assignment_stmt();
    }
    else if (nextToken == PLUS_SIGN || nextToken == MINUS_SIGN)
    {
        unary();
    }
    else
    {
        error("identifier | + | -");
    }

    while (nextToken == COMMA)
    {
        lex();
        if (nextToken == IDENT)
        {
            assignment_stmt();
        }
        else if (nextToken == PLUS_SIGN || nextToken == MINUS_SIGN)
        {
            unary();
        }
        else
        {
            error("identifier | + | -");
        }
    }

    exit_msg("statement_list");
}

// <unary> -> ( ++id | --id )
void unary()
{
    enter_msg("unary");

    if (nextToken == PLUS_SIGN)
    {
        lex();
        if (nextToken == PLUS_SIGN)
        {
            lex();
            if (nextToken == IDENT)
            {
                lex();
            }
            else
            {
                error("identifier");
            }
        }
        else
        {
            error("+");
        }
    }
    else if (nextToken == MINUS_SIGN)
    {
        lex();
        if (nextToken == MINUS_SIGN)
        {
            lex();
            if (nextToken == IDENT)
            {
                lex();
            }
            else
            {
                error("identifier");
            }
        }
        else
        {
            error("-");
        }
    }
    else
    {
        error("+ | -");
    }
    
    exit_msg("unary");
}

// <expr> -> <term> { (+|-) <term> }
void expr()
{
    enter_msg("expr");

    term();
    while (nextToken == PLUS_SIGN || nextToken == MINUS_SIGN)
    {
        lex();
        term();
    }

    exit_msg("expr");
}

// <term> -> <factor> { (*|/|%) <factor> }
void term()
{
    enter_msg("term");

    factor();
    while (nextToken == ASTERISK || nextToken == SLASH || nextToken == PERCENT_SIGN)
    {
        lex();
        factor();
    }
    
    exit_msg("term");
}

// <factor> -> id | int | float | ‘(‘ <expr> ‘)’
void factor()
{
    enter_msg("factor");
    
    if (nextToken == IDENT || nextToken == INT_LIT || nextToken == FLOAT_LIT)
    {
        lex();
    }
    else
    {
        if (nextToken == LEFT_PAREN)
        {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN)
            {
                lex();
            }
            else
            {
                error(")");
            }
        }
        else
        {
            error("(");
        }
    }
    
    exit_msg("factor");
}

// <bool_expr> -> <bool_term> { || <bool_term> }
void bool_expr()
{
    enter_msg("bool_expr");

    bool_term();
    while (nextToken == VERTICAL_BAR)
    {
        lex();
        if (nextToken == VERTICAL_BAR)
        {
            lex();
            bool_term();
        }
        else
        {
            error("|");
        }
    }

    exit_msg("bool_expr");
}

// <bool_term> -> <bool_factor> { && <bool_factor> }
void bool_term()
{
    enter_msg("bool_term");

    bool_factor();
    while (nextToken == AMPERSAND)
    {
        lex();
        if (nextToken == AMPERSAND)
        {
            lex();
            bool_factor();
        }
        else
        {
            error("&");
        }
    }
    
    exit_msg("bool_term");
}

// <bool_factor> -> { ! } ( id [ ( == | != | < | <= | >= | > ) <factor> ] | ‘(‘ <bool_expr> ‘)’ )
void bool_factor()
{
    enter_msg("bool_factor");

    if (nextToken == EXCLAMATION_MARK)
    {
        lex();
    }
    if (nextToken == IDENT)
    {
        lex();
        if (nextToken == EQUAL_SIGN || nextToken == EXCLAMATION_MARK || nextToken == OPENING_ANGLE || nextToken == CLOSING_ANGLE)
        {
            lex();
            if (nextToken == EQUAL_SIGN)
            {
                lex();
            }
            factor();
        }
    }
    else if (nextToken == LEFT_PAREN)
    {
        lex();
        bool_expr();
        if (nextToken == RIGHT_PAREN)
        {
            lex();
        }
        else
        {
            error(")");
        }
    }
    else
    {
        error("identifier | (");
    }

    exit_msg("bool_factor");
}

int lookup(char ch)
{
    switch (ch)
    {
    case '~':
        addChar();
        nextToken = TILDE;
        break;
    case '!':
        addChar();
        nextToken = EXCLAMATION_MARK;
        break;
    case '#':
        addChar();
        nextToken = NUMBER_SIGN;
        break;
    case '$':;
        addChar();
        nextToken = DOLLAR_SIGN;
        break;
    case '%':
        addChar();
        nextToken = PERCENT_SIGN;
        break;
    case '^':
        addChar();
        nextToken = CARET;
        break;
    case '&':
        addChar();
        nextToken = AMPERSAND;
        break;
    case '*':
        addChar();
        nextToken = ASTERISK;
        break;
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '_':
        addChar();
        nextToken = UNDERSCORE;
        break;
    case '+':
        addChar();
        nextToken = PLUS_SIGN;
        break;
    case '|':
        addChar();
        nextToken = VERTICAL_BAR;
        break;
    case '\\':
        addChar();
        nextToken = BACKSLASH;
        break;
    case '\'':
        addChar();
        nextToken = APOSTROPHE;
        break;
    case '-':
        addChar();
        nextToken = MINUS_SIGN;
        break;
    case '=':
        addChar();
        nextToken = EQUAL_SIGN;
        break;
    case '{':
        addChar();
        nextToken = LEFT_BRACE;
        break;
    case '}':
        addChar();
        nextToken = RIGHT_BRACE;
        break;
    case '[':
        addChar();
        nextToken = LEFT_BRACKET;
        break;
    case ']':
        addChar();
        nextToken = RIGHT_BRACKET;
        break;
    case ':':
        addChar();
        nextToken = COLON;
        break;
    case '"':
        addChar();
        nextToken = QUOTATION_MARK;
        break;
    case ';':
        addChar();
        nextToken = SEMICOLON;
        break;
    case '<':
        addChar();
        nextToken = OPENING_ANGLE;
        break;
    case '>':
        addChar();
        nextToken = CLOSING_ANGLE;
        break;
    case '?':
        addChar();
        nextToken = QUESTION_MARK;
        break;
    case ',':
        addChar();
        nextToken = COMMA;
        break;
    case '.':
        addChar();
        nextToken = PERIOD;
        break;
    case '/':
        addChar();
        nextToken = SLASH;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

void addChar(void)
{
    if (lexLen <= 98)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    }
    else
        printf("Error - lexeme is too long \n");
}

void getChar(void)
{
    if ((nextChar = getc(in_fp)) != EOF)
    {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}

void getNonBlank(void)
{
    while (isspace(nextChar))
        getChar();
}

int lex(void)
{
    lexLen = 0;
    getNonBlank();
    switch (charClass)
    {
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
        {
            addChar();
            getChar();
        }

        if (strcmp(lexeme, "VOID") == 0)
        {
            nextToken = VOID_CODE;
        }
        else if (strcmp(lexeme, "MAIN") == 0)
        {
            nextToken = MAIN_CODE;
        }
        else if (strcmp(lexeme, "in") == 0)
        {
            nextToken = IN_CODE;
        }
        else if (strcmp(lexeme, "default") == 0)
        {
            nextToken = DEFAULT_CODE;
        }
        else if (strcmp(lexeme, "switch") == 0)
        {
            nextToken = SWITCH_CODE;
        }
        else if (strcmp(lexeme, "case") == 0)
        {
            nextToken = CASE_CODE;
        }
        else if (strcmp(lexeme, "foreach") == 0)
        {
            nextToken = FOREACH_CODE;
        }
        else if (strcmp(lexeme, "for") == 0)
        {
            nextToken = FOR_CODE;
        }
        else if (strcmp(lexeme, "while") == 0)
        {
            nextToken = WHILE_CODE;
        }
        else if (strcmp(lexeme, "do") == 0)
        {
            nextToken = DO_CODE;
        }
        else if (strcmp(lexeme, "if") == 0)
        {
            nextToken = IF_CODE;
        }
        else if (strcmp(lexeme, "else") == 0)
        {
            nextToken = ELSE_CODE;
        }
        else if (strcmp(lexeme, "return") == 0)
        {
            nextToken = RETURN_CODE;
        }
        else if (strcmp(lexeme, "break") == 0)
        {
            nextToken = BREAK_CODE;
        }
        else
        {
            nextToken = IDENT;
        }
        break;
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        if (nextChar == '.')
        {
            nextToken = FLOAT_LIT;
            addChar();
            getChar();
            while (charClass == DIGIT)
            {
                addChar();
                getChar();
            }
        }
        if (nextChar == 'e' || nextChar == 'E')
        {
            nextToken = FLOAT_LIT;
            addChar();
            getChar();
            if (charClass == DIGIT || nextChar == '+' || nextChar == '-')
            {
                addChar();
                getChar();
                while (charClass == DIGIT)
                {
                    addChar();
                    getChar();
                }
            }
        }
        if (nextToken == FLOAT_LIT && (nextChar == 'f' || nextChar == 'l' || nextChar == 'F' || nextChar == 'L'))
        {   
            addChar();
            getChar();
        }
        else if (nextToken == INT_LIT && (nextChar == 'u' || nextChar == 'U')) {
            addChar();
            getChar();
            if (nextChar == 'l')
            {   
                addChar();
                getChar();
                if (nextChar == 'l')
                {   
                    addChar();
                    getChar();
                }
            }
            else if (nextChar == 'L')
            {   
                addChar();
                getChar();
                if (nextChar == 'L')
                {   
                    addChar();
                    getChar();
                }
            }
        }
        else if (nextToken == INT_LIT && nextChar == 'l') {
            addChar();
            getChar();
            if (nextChar == 'l')
            {   
                addChar();
                getChar();
            }
            if (nextChar == 'u' || nextChar == 'U')
            {   
                addChar();
                getChar();
            }
        }
        else if (nextToken == INT_LIT && nextChar == 'L') {
            addChar();
            getChar();
            if (nextChar == 'L')
            {   
                addChar();
                getChar();
            }
            if (nextChar == 'u' || nextChar == 'U')
            {   
                addChar();
                getChar();
            }
        }
        break;
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = '\0';
        break;
    }
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}