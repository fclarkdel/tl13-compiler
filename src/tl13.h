#include <stdio.h>
#include "../external/uthash.h"

enum Type {
	INT,
	BOOL,
};

enum FactorType {
	IDENT,
	NUM,
	BOOLLIT,
	EXPRESSION,
};

union FactorUnion {
	char* ident;
	char* num;
	char* boollit;
	struct Expression* expression;
};

struct Factor {
	enum FactorType type;
	union FactorUnion factor;
};

enum Op2Type {
	STAR,
	DIV,
	MOD,
};

struct Term {
	struct Factor* factor0;
	enum Op2Type* op2;
	struct Factor* factor1;
};

enum Op3Type {
	PLUS,
	MINUS,
};

struct SimpleExpression {
	struct Term* term0;
	enum Op3Type* op3;
	struct Term* term1;
};

enum Op4Type {
	EQUAL,
	NOT_EQUAL,
	LESS,
	GREATER,
	LESS_OR_EQUAL,
	GREATER_OR_EQUAL,
};

struct Expression {
	struct SimpleExpression* simple_expression0;
	enum Op4Type* op4;
	struct SimpleExpression* simple_expression1;
};

struct WriteInt {
	struct Expression* expression;
};

struct WhileStatement {
	struct Expression* expression;
	struct StatementSequence* statement_sequence;
};

struct ElseClause {
	struct StatementSequence* statement_sequence;
};

struct IfStatement {
	struct Expression* expression;
	struct StatementSequence* statement_sequence;
	struct ElseClause* else_clause;
};

struct Assignment {
	char* ident;
	struct Expression* expression;
};

enum StatementType {
	ASSIGNMENT,
	IF_STATEMENT,
	WHILE_STATEMENT,
	WRITE_INT,
};

union StatementUnion {
	struct Assignment* assignment;
	struct IfStatement* if_statement;
	struct WhileStatement* while_statement;
	struct WriteInt* write_int;
};

struct Statement {
	enum StatementType type;
	union StatementUnion statement;
};

struct StatementSequence {
	struct Statement* statement;
	struct StatementSequence* statement_sequence;
};

struct Declarations {
	char* ident;
	enum Type* type;
	struct Declarations* declarations;
};

struct Program {
	struct Declarations* declarations;
	struct StatementSequence* statement_sequence;
};

struct Variable {
	char* ident;
	enum Type type;
	UT_hash_handle hh;
};

void gen_code_program(FILE* code_dest, struct Program* program);

void gen_code_declarations(FILE* code_dest, struct Declarations* declarations);

void gen_code_statement_sequence(FILE* code_dest, struct StatementSequence* statement_sequence);

void gen_code_statement(FILE* code_dest, struct Statement* statement);

void gen_code_assignment(FILE* code_dest, struct Assignment* assignment);

void gen_code_expression(FILE* code_dest, struct Expression* expression);

void gen_code_simple_expression(FILE* code_dest, struct SimpleExpression* simple_expression);

void gen_code_term(FILE* code_dest, struct Term* term);

void gen_code_factor(FILE* code_dest, struct Factor* factor);

void gen_code_if_statement(FILE* code_dest, struct IfStatement* if_statement);

void gen_code_else_clause(FILE* code_dest, struct ElseClause* else_clause);

void gen_code_while_statement(FILE* code_dest, struct WhileStatement* while_statement);

void gen_code_write_int(FILE* code_dest, struct WriteInt* write_int);
