#include "tl13.h"
#include <stdlib.h>

struct Variable* variables = NULL;

enum Error {
	UNKNOWN_ERROR = 1,
	UNKNOWN_DECLARATION_TYPE,
	INVALID_IDENTIFIER,
	UNKNOWN_STATEMENT_TYPE,
	EXPECTED_EXPRESSION,
	UNKNOWN_OPERATOR,
	EXPECTED_SIMPLE_EXPRESSION,
	EXPECTED_TERM,
	EXPECTED_FACTOR,
	UNKNOWN_FACTOR_TYPE,
	EXPECTED_STATEMENT,
	EXPECTED_ASSIGNMENT,
	EXPECTED_IF_STATEMENT,
	EXPECTED_STATEMENT_SEQUENCE,
	EXPECTED_ELSE_CLAUSE,
	EXPECTED_DECLARATIONS,
	EXPECTED_TYPE,
	EXPECTED_PROGRAM,
	EXPECTED_OPERATOR,
	EXPECTED_WHILE_STATEMENT,
	EXPECTED_WRITE_INT,
	VARIABLE_ALREADY_DECLARED,
};

void raise_error(enum Error error) {
	switch (error) {
		case UNKNOWN_ERROR:
			fprintf(stderr, "Error: Unknown error.\n");

			break;
		case UNKNOWN_DECLARATION_TYPE:
			fprintf(stderr, "Error: Unknown declaration type.\n");

			break;
		case INVALID_IDENTIFIER:
			fprintf(stderr, "Error: Invalid identifier.\n");

			break;
		case UNKNOWN_STATEMENT_TYPE:
			fprintf(stderr, "Error: Unknown statement type.\n");

			break;
		case EXPECTED_EXPRESSION:
			fprintf(stderr, "Error: Expected expression.\n");

			break;
		case UNKNOWN_OPERATOR:
			fprintf(stderr, "Error: Unknown operator.\n");

			break;
		case EXPECTED_SIMPLE_EXPRESSION:
			fprintf(stderr, "Error: Expected simple expression.\n");

			break;
		case EXPECTED_TERM:
			fprintf(stderr, "Error: Expected term.\n");

			break;
		case EXPECTED_FACTOR:
			fprintf(stderr, "Error: Expected factor.\n");

			break;
		case EXPECTED_STATEMENT:
			fprintf(stderr, "Error: Expected statement.\n");

			break;
		case EXPECTED_ASSIGNMENT:
			fprintf(stderr, "Error: Expected assignment.\n");

			break;
		case EXPECTED_IF_STATEMENT:
			fprintf(stderr, "Error: Expected if statement.\n");

			break;
		case EXPECTED_STATEMENT_SEQUENCE:
			fprintf(stderr, "Error: Expected statement sequence.\n");

			break;
		case EXPECTED_ELSE_CLAUSE:
			fprintf(stderr, "Error: Expected else clause.\n");

			break;
		case EXPECTED_DECLARATIONS:
			fprintf(stderr, "Error: Expected declarations.\n");

			break;
		case EXPECTED_TYPE:
			fprintf(stderr, "Error: Expected type.\n");

			break;
		case EXPECTED_PROGRAM:
			fprintf(stderr, "Error: Expected program.\n");

			break;
		case EXPECTED_OPERATOR:
			fprintf(stderr, "Error: Expected operator.\n");

			break;
		case EXPECTED_WHILE_STATEMENT:
			fprintf(stderr, "Error: Expected while statement.\n");

			break;
		case EXPECTED_WRITE_INT:
			fprintf(stderr, "Error: Expected write int.\n");

			break;
		case VARIABLE_ALREADY_DECLARED:
			fprintf(stderr, "Error: Variable already declared.\n");

			break;
		default:
			raise_error(UNKNOWN_ERROR);

			return;
	}

	exit(error);
}

void define_read_int(FILE* code_dest) {
	fprintf(code_dest, "int read_int() {\n");
	fprintf(code_dest, "	int value;\n");
	fprintf(code_dest, "\n");
	fprintf(code_dest, "	if (scanf(\"%%d\", &value) != 1)\n");
	fprintf(code_dest, "		exit(1);\n");
	fprintf(code_dest, "\n");
	fprintf(code_dest, "	return value;\n");
	fprintf(code_dest, "}\n");
}

void define_write_int(FILE* code_dest) {
	fprintf(code_dest, "void write_int(int value) {\n");
	fprintf(code_dest, "	printf(\"%%d\", value);\n");
	fprintf(code_dest, "}\n");
}

void gen_code_program(FILE* code_dest, struct Program* program) {
	if (!program) {
		raise_error(EXPECTED_PROGRAM);

		return;
	}

	fprintf(code_dest, "#include <stdio.h>\n");
	fprintf(code_dest, "#include <stdlib.h>\n");
	fprintf(code_dest, "\n");

	define_read_int(code_dest);
	fprintf(code_dest, "\n");

	define_write_int(code_dest);
	fprintf(code_dest, "\n");

	fprintf(code_dest, "int main() {\n");

	if (!program->declarations) {
		raise_error(EXPECTED_DECLARATIONS);

		return;
	}
	gen_code_declarations(code_dest, program->declarations);

	if (!program->statement_sequence) {
		raise_error(EXPECTED_STATEMENT_SEQUENCE);

		return;
	}
	gen_code_statement_sequence(code_dest, program->statement_sequence);

	fprintf(code_dest, "	return 0;\n");
	fprintf(code_dest, "}\n");
}

void gen_code_declarations(FILE* code_dest, struct Declarations* declarations) {
	if (!declarations) {
		raise_error(EXPECTED_DECLARATIONS);

		return;
	}

	if (!declarations->ident && !declarations->type && !declarations->declarations)
		return;

	if (!declarations->ident) {
		raise_error(INVALID_IDENTIFIER);

		return;
	}

    struct Variable* variable;

    HASH_FIND_STR(variables, declarations->ident, variable);

    if(variable) {
    	raise_error(VARIABLE_ALREADY_DECLARED);

        return;
    }

	if (!declarations->type) {
		raise_error(EXPECTED_TYPE);

		return;
	}

	variable = malloc(sizeof(struct Variable));

    variable->ident = malloc(strlen(declarations->ident) + 1);
    strcpy(variable->ident, declarations->ident);
    variable->type = *declarations->type;

    HASH_ADD_STR(variables, ident, variable);

	switch (*declarations->type) {
		case INT:
			fprintf(code_dest, "	int %s = 0;\n", declarations->ident);

			break;
		case BOOL:
			fprintf(code_dest, "	bool %s = 0;\n", declarations->ident);

			break;
		default:
			raise_error(UNKNOWN_DECLARATION_TYPE);

			return;
	}

	if (!declarations->declarations) {
		raise_error(EXPECTED_DECLARATIONS);

		return;
	}
	gen_code_declarations(code_dest, declarations->declarations);
}

void gen_code_statement_sequence(FILE* code_dest, struct StatementSequence* statement_sequence) {
	if (!statement_sequence) {
		raise_error(EXPECTED_STATEMENT_SEQUENCE);

		return;
	}

	if (!statement_sequence->statement && !statement_sequence->statement_sequence)
		return;

	if (!statement_sequence->statement) {
		raise_error(EXPECTED_STATEMENT);

		return;
	}
	gen_code_statement(code_dest, statement_sequence->statement);

	if (!statement_sequence->statement_sequence) {
		raise_error(EXPECTED_STATEMENT_SEQUENCE);

		return;
	}
	gen_code_statement_sequence(code_dest, statement_sequence->statement_sequence);
}

void gen_code_statement(FILE* code_dest, struct Statement* statement) {
	if (!statement) {
		raise_error(EXPECTED_STATEMENT);

		return;
	}

	switch (statement->type) {
		case ASSIGNMENT:
			gen_code_assignment(code_dest, statement->statement.assignment);

			break;
		case IF_STATEMENT:
			gen_code_if_statement(code_dest, statement->statement.if_statement);

			break;
		case WHILE_STATEMENT:
			gen_code_while_statement(code_dest, statement->statement.while_statement);

			break;
		case WRITE_INT:
			gen_code_write_int(code_dest, statement->statement.write_int);

			break;
		default:
			raise_error(UNKNOWN_STATEMENT_TYPE);

			return;
	}
}

void gen_code_assignment(FILE* code_dest, struct Assignment* assignment) {
	if (!assignment) {
		raise_error(EXPECTED_ASSIGNMENT);

		return;
	}

	if (!assignment->ident) {
		raise_error(INVALID_IDENTIFIER);

		return;
	}
	fprintf(code_dest, "	%s = ", assignment->ident);

	if (assignment->expression)
		gen_code_expression(code_dest, assignment->expression);
	else
		fprintf(code_dest, " read_int()");

	fprintf(code_dest, ";\n");
}

void gen_code_expression(FILE* code_dest, struct Expression* expression) {
	if (!expression) {
		raise_error(EXPECTED_EXPRESSION);

		return;
	}

	if (!expression->simple_expression0) {
		raise_error(EXPECTED_SIMPLE_EXPRESSION);

		return;
	}
	gen_code_simple_expression(code_dest, expression->simple_expression0);

	if (!expression->op4 && !expression->simple_expression1)
		return;


	if (!expression->op4) {
		raise_error(EXPECTED_OPERATOR);

		return;
	}
	switch (*expression->op4) {
		case EQUAL:
			fprintf(code_dest, " == ");

			break;
		case NOT_EQUAL:
			fprintf(code_dest, " != ");

			break;
		case LESS:
			fprintf(code_dest, " < ");

			break;
		case GREATER:
			fprintf(code_dest, " > ");

			break;
		case LESS_OR_EQUAL:
			fprintf(code_dest, " <= ");

			break;
		case GREATER_OR_EQUAL:
			fprintf(code_dest, " >= ");

			break;
		default:
			raise_error(UNKNOWN_OPERATOR);

			return;
	}

	if (!expression->simple_expression1) {
		raise_error(EXPECTED_SIMPLE_EXPRESSION);

		return;
	}
	gen_code_simple_expression(code_dest, expression->simple_expression1);
}

void gen_code_simple_expression(FILE* code_dest, struct SimpleExpression* simple_expression) {
	if (!simple_expression) {
		raise_error(EXPECTED_SIMPLE_EXPRESSION);

		return;
	}

	if (!simple_expression->term0) {
		raise_error(EXPECTED_TERM);

		return;
	}
	gen_code_term(code_dest, simple_expression->term0);

	if (!simple_expression->op3 && !simple_expression->term1)
		return;

	if (!simple_expression->op3) {
		raise_error(EXPECTED_OPERATOR);

		return;
	}
	switch (*simple_expression->op3) {
		case PLUS:
			fprintf(code_dest, " + ");

			break;
		case MINUS:
			fprintf(code_dest, " - ");

			break;
		default:
			raise_error(UNKNOWN_OPERATOR);

			return;
	}

	if (!simple_expression->term1) {
		raise_error(EXPECTED_TERM);

		return;
	}
	gen_code_term(code_dest, simple_expression->term1);
}

void gen_code_term(FILE* code_dest, struct Term* term) {
	if (!term) {
		raise_error(EXPECTED_TERM);

		return;
	}

	if (!term->factor0) {
		raise_error(EXPECTED_FACTOR);

		return;
	}
	gen_code_factor(code_dest, term->factor0);

	if (!term->op2 && !term->factor1)
		return;

	if (!term->op2) {
		raise_error(EXPECTED_OPERATOR);

		return;
	}
	switch (*term->op2) {
		case STAR:
			fprintf(code_dest, " *");

			break;
		case DIV:
			fprintf(code_dest, " /");

			break;
		case MOD:
			fprintf(code_dest, " %%");

			break;
		default:
			raise_error(UNKNOWN_OPERATOR);

			return;
	}

	if (!term->factor1) {
		raise_error(EXPECTED_FACTOR);

		return;
	}
	gen_code_factor(code_dest, term->factor1);
}

void gen_code_factor(FILE* code_dest, struct Factor* factor) {
	if (!factor) {
		raise_error(EXPECTED_FACTOR);

		return;
	}

	switch (factor->type) {
		case IDENT:
			fprintf(code_dest, "%s", factor->factor.ident);

			break;
		case NUM:
			fprintf(code_dest, "%s", factor->factor.num);

			break;
		case BOOLLIT:
			fprintf(code_dest, "%s", factor->factor.boollit);

			break;
		case EXPRESSION:
			fprintf(code_dest, " (");
			gen_code_expression(code_dest, factor->factor.expression);
			fprintf(code_dest, ")");

			break;
		default:
			raise_error(UNKNOWN_FACTOR_TYPE);

			return;
	}
}

void gen_code_if_statement(FILE* code_dest, struct IfStatement* if_statement) {
	if (!if_statement) {
		raise_error(EXPECTED_IF_STATEMENT);

		return;
	}

	fprintf(code_dest, "	if (");

	if (!if_statement->expression) {
		raise_error(EXPECTED_EXPRESSION);

		return;
	}
	gen_code_expression(code_dest, if_statement->expression);

	fprintf(code_dest, ") {\n");
	fprintf(code_dest, "	");

	if (!if_statement->statement_sequence) {
		raise_error(EXPECTED_STATEMENT_SEQUENCE);

		return;
	}
	gen_code_statement_sequence(code_dest, if_statement->statement_sequence);

	fprintf(code_dest, "	}\n");

	if (!if_statement->else_clause) {
		raise_error(EXPECTED_ELSE_CLAUSE);

		return;
	}
	gen_code_else_clause(code_dest, if_statement->else_clause);
}

void gen_code_else_clause(FILE* code_dest, struct ElseClause* else_clause) {
	if (!else_clause) {
		raise_error(EXPECTED_ELSE_CLAUSE);

		return;
	}

	if (!else_clause->statement_sequence)
		return;

	gen_code_statement_sequence(code_dest, else_clause->statement_sequence);
}

void gen_code_while_statement(FILE* code_dest, struct WhileStatement* while_statement) {
	if (!while_statement) {
		raise_error(EXPECTED_WHILE_STATEMENT);

		return;
	}

	fprintf(code_dest, "	while (");

	if (!while_statement->expression) {
		raise_error(EXPECTED_EXPRESSION);

		return;
	}
	gen_code_expression(code_dest, while_statement->expression);

	fprintf(code_dest, ") {\n");
	fprintf(code_dest, "	");

	if (!while_statement->statement_sequence) {
		raise_error(EXPECTED_STATEMENT_SEQUENCE);

		return;
	}
	gen_code_statement_sequence(code_dest, while_statement->statement_sequence);

	fprintf(code_dest, "	}\n");
}

void gen_code_write_int(FILE* code_dest, struct WriteInt* write_int) {
	if (!write_int) {
		raise_error(EXPECTED_WRITE_INT);

		return;
	}

	fprintf(code_dest, "	write_int(");

	if (!write_int->expression) {
		raise_error(EXPECTED_EXPRESSION);

		return;
	}
	gen_code_expression(code_dest, write_int->expression);

	fprintf(code_dest, ");\n");
}