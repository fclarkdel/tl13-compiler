%{
#include "tl13.h"
#include "tl13.tab.h"
%}

%%
" "|"\t"|"\n"

[1-9][0-9]*|0 {
	yylval.str = malloc(strlen(yytext) + 1);
	strncpy(yylval.str, yytext, strlen(yytext) + 1);

	return num_;
}

"false"|"true" {
	yylval.str = malloc(strlen(yytext) + 1);
	strncpy(yylval.str, yytext, strlen(yytext) + 1);

	return boollit_;
}

[A-Z][A-Z0-9]* {
	yylval.str = malloc(strlen(yytext) + 1);
	strncpy(yylval.str, yytext, strlen(yytext) + 1);

	return ident_;
}

"(" {
	return LP_;
}

")" {
	return RP_;
}

":=" {
	return ASGN_;
}

";" {
	return SC_;
}

"*" {
	yylval.op2_type = STAR;

	return OP2_;
}

"div" {
	yylval.op2_type = DIV;

	return OP2_;
}

"mod" {
	yylval.op2_type = MOD;

	return OP2_;
}

"+" {
	yylval.op3_type = PLUS;

	return OP3_;
}

"-" {
	yylval.op3_type = MINUS;

	return OP3_;
}

"=" {
	yylval.op4_type = EQUAL;

	return OP4_;
}

"!=" {
	yylval.op4_type = NOT_EQUAL;

	return OP4_;
}

"<" {
	yylval.op4_type = LESS;

	return OP4_;
}

">" {
	yylval.op4_type = GREATER;

	return OP4_;
}

"<=" {
	yylval.op4_type = LESS_OR_EQUAL;

	return OP4_;
}

">=" {
	yylval.op4_type = GREATER_OR_EQUAL;

	return OP4_;
}

"if" {
	return IF_;
}

"then" {
	return THEN_;
}

"else" {
	return ELSE_;
}

"begin" {
	return BEGIN_;
}

"end" {
	return END_;
}

"while" {
	return WHILE_;
}

"do" {
	return DO_;
}

"program" {
	return PROGRAM_;
}

"var" {
	return VAR_;
}

"as" {
	return AS_;
}

"int" {
	return INT_;
}

"bool" {
	return BOOL_;
}

"writeInt" {
	return WRITE_INT_;
}

"readInt" {
	return READ_INT_;
}
%%
