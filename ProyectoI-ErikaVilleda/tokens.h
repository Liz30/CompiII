
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM_INT = 258,
     ID = 259,
     STRING_LITERAL = 260,
     KW_BOOL = 261,
     KW_BREAK = 262,
     KW_PRINT = 263,
     KW_READ = 264,
     KW_CONTINUE = 265,
     KW_CLASS = 266,
     KW_ELSE = 267,
     KW_EXTENDS = 268,
     KW_FALSE = 269,
     KW_FOR = 270,
     KW_IF = 271,
     KW_INT = 272,
     KW_NEW = 273,
     KW_NULL = 274,
     KW_RETURN = 275,
     KW_ROT = 276,
     KW_TRUE = 277,
     KW_VOID = 278,
     KW_WHILE = 279,
     KW_MAIN = 280,
     TK_EOF = 281,
     TK_IGUAL = 282,
     TK_SUMA = 283,
     TK_RESTA = 284,
     TK_MULT = 285,
     TK_DIV = 286,
     TK_MOD = 287,
     TK_ASIG = 288,
     TK_NEG = 289,
     TK_BIZQ = 290,
     TK_BDER = 291,
     OP_AND = 292,
     OP_OR = 293,
     OP_LT = 294,
     OP_LTE = 295,
     OP_NE = 296,
     OP_GT = 297,
     OP_GTE = 298
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 22 "DeCaf.y"

    char *id_t;
    int  num_t;
    Statement *statement_t;
    Expr *expr_t;
    Type_v t;
    StatementList *sList_t;
    ExprList *eList_t;



/* Line 1676 of yacc.c  */
#line 107 "tokens.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


