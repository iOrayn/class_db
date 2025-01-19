#ifndef REPL_H
#define REPL_H

#include <stddef.h>
#include <stdbool.h>

#define TABLE_MAX_ROWS 100

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { 
  PREPARE_SUCCESS, 
  PREPARE_UNRECOGNIZED_STATEMENT 
} PrepareResult;

typedef enum { 
  STATEMENT_INSERT, 
  STATEMENT_SELECT 
} StatementType;

typedef struct {
  StatementType type;
  int id;
  char username[32];
  char email[255];
} Statement;

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef struct {
  int id;
  char username[32];
  char email[255];
} Row;

typedef struct {
  Row rows[TABLE_MAX_ROWS];
  size_t num_rows;
} Table;

InputBuffer* new_input_buffer();
void print_prompt();
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);
MetaCommandResult do_meta_command(InputBuffer* input_buffer);
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
void execute_statement(Statement* statement, Table* table);
void execute_insert(Statement* statement, Table* table);
void execute_select(Table* table);
void repl();

#endif

