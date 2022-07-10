# Lexical and Syntax Analyzer

  

You can run the analyzer as is once compiled, as long as the given example program is in the same folder. You may also form your own valid program according to the grammar below, but you must title it `program.rp`.

  

## Given Rules:

- \<program> -> VOID MAIN ‘(‘ ‘)’ \<block\>

- \<block> -> ‘{‘ { \<statement\> ; } ‘}’

  

## Main Rules:

- \<statement> -> ( \<switch> | \<foreach> | \<for> | \<while> | \<do_while> | \<if> | \<assignment> | \<return> )

- \<switch> -> switch ‘(‘ \<factor> ‘)’ \<switch_block>

- \<foreach> -> foreach ‘(‘ id in id ‘)’ \<block>

- \<for> -> for ‘(‘ [\<statement_list>] ; [\<bool_expr>] ; [\<statement_list>] ‘)’ \<block>

- \<while> -> while ‘(‘ \<bool_expr> ‘)’ \<block>

- \<do_while> -> do \<block> while ‘(‘ \<bool_expr> ‘)’

- \<if> -> if ‘(‘ bool_expr ‘)’ \<block> [else \<block>]

- \<assignment> -> id = \<expr>

- \<return> -> return [\<expr>]

  

## Helper Rules:

- \<switch_block> -> ‘{‘ { \<case_block> } ‘}’

- \<case_block> -> ( case \<factor> | default ): { \<statement> ; } break;

- \<statement_list> -> ( \<assignment> | \<unary> ) [ {, ( \<assignment> | \<unary> ) }]

- \<unary> -> ( ++id | --id )

  

## Expression Rules:

- \<expr> -> \<term> { (+|-) \<term> }

- \<term> -> \<factor> { (*|/|%) \<factor> }

- \<factor> -> id | int | float | ‘(‘ \<expr> ‘)’

  

## Boolean Expression Rules:

- \<bool_expr> -> \<bool_term> { || \<bool_term> }

- \<bool_term> -> \<bool_factor> { && \<bool_factor> }

- \<bool_factor> -> { ! } ( id [ ( == | != | < | <= | >= | > ) \<factor> ] | ‘(‘ \<bool_expr> ‘)’ )

  

## Rule Sources:

- switch: C

- foreach: Haxe

- for: C#

- while: C#

- do-while: C#

- if: C#

- assignment: Python

- return: C#

Boolean rules based on Java logic
