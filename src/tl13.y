%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tl13.h"

int yylex(void);
void yyerror(char*);
%}

%union {
	char* str;
	enum Op3Type op3_type;
	enum Op4Type op4_type;
	enum Op2Type op2_type;

	struct Program* program;
	struct Declarations* declarations;
	enum Type type;
	struct StatementSequence* statement_sequence;
	struct Statement* statement;
	struct Assignment* assignment;
	struct Expression* expression;
	struct IfStatement* if_statement;
	struct ElseClause* else_clause;
	struct WhileStatement* while_statement;
	struct WriteInt* write_int;
	struct SimpleExpression* simple_expression;
	struct Term* term;
	struct Factor* factor;
}

%token PROGRAM_
%token BEGIN_
%token END_
%token VAR_
%token <str> ident_
%token AS_
%token SC_
%token INT_
%token BOOL_
%token ASGN_
%token READ_INT_
%token IF_
%token THEN_
%token WHILE_
%token DO_
%token WRITE_INT_
%token <op4_type> OP4_
%token ELSE_
%token <op3_type> OP3_
%token <op2_type> OP2_
%token <str> num_
%token <str> boollit_
%token LP_
%token RP_

%type <program> Program
%type <declarations> Declarations
%type <type> Type
%type <statement_sequence> StatementSequence
%type <statement> Statement
%type <assignment> Assignment
%type <expression> Expression
%type <else_clause> ElseClause
%type <if_statement> IfStatement
%type <while_statement> WhileStatement
%type <write_int> WriteInt
%type <simple_expression> SimpleExpression
%type <term> Term
%type <factor> Factor

%start Program

%%
Program:
	PROGRAM_ Declarations BEGIN_ StatementSequence END_
	{
		struct Program* program = malloc(sizeof(struct Program));
		program->declarations = $2;
		program->statement_sequence = $4;

        gen_code_program(stdout, program);
	};

Declarations:
	VAR_ ident_ AS_ Type SC_ Declarations
	{
		struct Declarations* declarations = malloc(sizeof(struct Declarations));
		declarations->ident = malloc(strlen($2) + 1);
		strcpy(declarations->ident, $2);

		declarations->type = malloc(sizeof(enum Type));
		*declarations->type = $4;
		declarations->declarations = $6;

		$$ = declarations;
	}
	|
	/* empty */
	{
		struct Declarations* declarations = malloc(sizeof(struct Declarations));
		declarations->ident = NULL;
		declarations->type = NULL;
		declarations->declarations = NULL;

		$$ = declarations;
	};

StatementSequence:
	Statement SC_ StatementSequence
	{
		struct StatementSequence* statement_sequence = malloc(sizeof(struct StatementSequence));
		statement_sequence->statement = $1;
		statement_sequence->statement_sequence = $3;

		$$ = statement_sequence;
	}
	|
	/* empty */
	{
		struct StatementSequence* statement_sequence = malloc(sizeof(struct StatementSequence));
		statement_sequence->statement = NULL;
		statement_sequence->statement_sequence = NULL;

		$$ = statement_sequence;
	};

Type:
	INT_
	{
		$$ = INT;
	}
	|
	BOOL_
	{
		$$ = BOOL;
	};

Statement:
	Assignment
	{
		struct Statement* statement = malloc(sizeof(struct Statement));
        statement->type = ASSIGNMENT;
		statement->statement.assignment = $1;

		$$ = statement;
	}
	|
	IfStatement
	{
		struct Statement* statement = malloc(sizeof(struct Statement));
		statement->type = IF_STATEMENT;
		statement->statement.if_statement = $1;

		$$ = statement;
	}
	|
	WhileStatement
	{
		struct Statement* statement = malloc(sizeof(struct Statement));
		statement->type = WHILE_STATEMENT;
		statement->statement.while_statement = $1;

		$$ = statement;
	}
	|
	WriteInt
	{
		struct Statement* statement = malloc(sizeof(struct Statement));
		statement->type = WRITE_INT;
		statement->statement.write_int = $1;

		$$ = statement;
	};

Assignment:
	ident_ ASGN_ Expression
	{
		struct Assignment* assignment = malloc(sizeof(struct Assignment));
		assignment->ident = malloc(strlen($1) + 1);
		strcpy(assignment->ident, $1);

		assignment->expression = $3;

		$$ = assignment;
	}
	|
	ident_ ASGN_ READ_INT_
	{
		struct Assignment* assignment = malloc(sizeof(struct Assignment));
		assignment->ident = malloc(strlen($1) + 1);
		strcpy(assignment->ident, $1);

		assignment->expression = NULL;

		$$ = assignment;
	};

IfStatement:
	IF_ Expression THEN_ StatementSequence ElseClause END_
	{
		struct IfStatement* if_statement = malloc(sizeof(struct IfStatement));
		if_statement->expression = $2;
		if_statement->statement_sequence = $4;
		if_statement->else_clause = $5;

        $$ = if_statement;
	};

WhileStatement:
	WHILE_ Expression DO_ StatementSequence END_
	{
		struct WhileStatement* while_statement = malloc(sizeof(struct WhileStatement));
        while_statement->expression = $2;
        while_statement->statement_sequence = $4;

        $$ = while_statement;
	};

WriteInt:
	WRITE_INT_ Expression
	{
		struct WriteInt* write_int = malloc(sizeof(struct WriteInt));
		write_int->expression = $2;

		$$ = write_int;
	};

Expression:
	SimpleExpression
	{
		struct Expression* expression = malloc(sizeof(struct Expression));
		expression->simple_expression0 = $1;
		expression->op4 = NULL;
		expression->simple_expression1 = NULL;

		$$ = expression;
	}
	|
	SimpleExpression OP4_ SimpleExpression
	{
		struct Expression* expression = malloc(sizeof(struct Expression));
        expression->simple_expression0 = $1;

		expression->op4 = malloc(sizeof(enum Op4Type));
		*expression->op4 = $2;

        expression->simple_expression1 = $3;

		$$ = expression;
	};

ElseClause:
	ELSE_ StatementSequence
	{
		struct ElseClause* else_clause = malloc(sizeof(struct ElseClause));

		else_clause->statement_sequence = $2;

		$$ = else_clause;
	}
	|
	/* empty */
	{
		struct ElseClause* else_clause = malloc(sizeof(struct ElseClause));

		else_clause->statement_sequence = NULL;

		$$ = else_clause;
	};

SimpleExpression:
	Term OP3_ Term
	{
		struct SimpleExpression* simple_expression = malloc(sizeof(struct SimpleExpression));
		simple_expression->term0 = $1;

		simple_expression->op3 = malloc(sizeof(enum Op3Type));
		*simple_expression->op3 = $2;

		simple_expression->term1 = $3;

        $$ = simple_expression;
	}
	|
	Term
	{
		struct SimpleExpression* simple_expression = malloc(sizeof(struct SimpleExpression));
        simple_expression->term0 = $1;

		simple_expression->op3 = malloc(sizeof(enum Op3Type));
        simple_expression->op3 = NULL;

        simple_expression->term1 = NULL;

        $$ = simple_expression;
	};

Term:
	Factor OP2_ Factor
	{
		struct Term* term = malloc(sizeof(struct Term));
		term->factor0 = $1;

		term->op2 = malloc(sizeof(enum Op2Type));
		*term->op2 = $2;

		term->factor1 = $3;

		$$ = term;
	}
	|
	Factor
	{
		struct Term* term = malloc(sizeof(struct Term));
        term->factor0 = $1;
		term->op2 = NULL;
        term->factor1 = NULL;

        $$ = term;
    };

Factor:
	ident_
	{
		struct Factor* factor = malloc(sizeof(struct Factor));
		factor->type = IDENT;

		factor->factor.ident = malloc(strlen($1) + 1);
		strcpy(factor->factor.ident, $1);

		$$ = factor;
	}
	|
	num_
	{
		struct Factor* factor = malloc(sizeof(struct Factor));
		factor->type = NUM;

		factor->factor.num = malloc(strlen($1) + 1);
		strcpy(factor->factor.num, $1);

		$$ = factor;
	}
	|
	boollit_
	{
		struct Factor* factor = malloc(sizeof(struct Factor));
		factor->type = BOOLLIT;

		factor->factor.boollit = malloc(strlen($1) + 1);
		strcpy(factor->factor.boollit, $1);

		$$ = factor;
	}
	|
	LP_ Expression RP_
	{
		struct Factor* factor = malloc(sizeof(struct Factor));
		factor->type = EXPRESSION;
		factor->factor.expression = $2;

		$$ = factor;
	};
%%

void yyerror(char* s) {
	printf("Syntax Error\n");
}

int main(void) {
	int yyret = yyparse();

	if (yyret == 0) {
		printf("SUCCESS\n");
	} else {
		printf("FAILURE\n");
	}

	return yyret;
}
