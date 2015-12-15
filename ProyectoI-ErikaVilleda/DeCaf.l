%option noyywrap

%{
#include <cstdlib>
#include <cstring>
#include "ast.h"
#include "tokens.h"

int line = 0;

%}

DIGIT [0-9]
LETTER [a-zA-Z]

%x comment

%%
"(" { return '('; }
")" { return ')'; }
";" { return ';'; }
"{" { return '{'; }
"}" { return '}'; }
\.  { return '.'; }
"," { return ','; }
"[" { return '['; }
"]" { return ']'; }
"//" { BEGIN(comment); }
"==" { return TK_IGUAL; }
"+" { return TK_SUMA; }
"-" { return TK_RESTA; }
"*" { return TK_MULT; }
"/" { return TK_DIV; }
"%" { return TK_MOD; }
"=" { return TK_ASIG; }
"!" { return TK_NEG; }
"<<" { return TK_BIZQ; }
">>" { return TK_BDER; }
"<" { return OP_LT; }
">" { return OP_GT; }
"<=" { return OP_LTE; }
">=" { return OP_GTE; }
"!=" { return OP_NE; }
"&&"  { return OP_AND; }
"||"  { return OP_OR; }
[ \t] /* Nada */
\n  { line++; }
"bool"    { return KW_BOOL; }
"break"   { return KW_BREAK; }
"print"   { return KW_PRINT; }
"read"    { return KW_READ; }
"continue" { return KW_CONTINUE; }
"class"   { return KW_CLASS; }
"else"    { return KW_ELSE; }
"extends" { return KW_EXTENDS; }
"false"   { return KW_FALSE; }
"for"     { return KW_FOR; }
"if"      { return KW_IF; }
"int"     { return KW_INT; }
"new"     { return KW_NEW; }
"null"    { return KW_NULL; }
"return"  { return KW_RETURN; }
"rot"     { return KW_ROT; }
"true"    { return KW_TRUE; }
"void"    { return KW_VOID; }
"while"   { return KW_WHILE; }
"main"    { return KW_MAIN; }
"\""[^']*"\""         { yytext[yyleng-1] = '\0'; yylval.id_t = strdup(&yytext[1]); return STRING_LITERAL; }

{DIGIT}+            { yylval.num_t = atoi(yytext); return NUM_INT; }

(_|{LETTER})({DIGIT}|_|{LETTER})* { yylval.id_t = strdup(yytext); return ID; }

<comment>[\n]   { line++; BEGIN(INITIAL); }
<comment>.      { /* NOTHING TO DO */ }
"//"[^\n]*

. { printf("Line %d: Invalid symbol '%c'\n", line, yytext[0]); }
%%
