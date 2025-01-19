#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

#define TABLE_MAX_ROWS 100

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

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = (char*)malloc(1024);
  input_buffer->buffer_length = 1024;
  input_buffer->input_length = 0;
  return input_buffer;
}

void print_prompt() { 
  printf("db > "); 
}

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read = (fgets(input_buffer->buffer, input_buffer->buffer_length, stdin) == NULL);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", &statement->id, statement->username, statement->email);
    if (args_assigned < 3) {
      return PREPARE_UNRECOGNIZED_STATEMENT;
    }
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement, Table* table) {
  switch (statement->type) {
    case STATEMENT_INSERT:
      execute_insert(statement, table);
      break;
    case STATEMENT_SELECT:
      execute_select(table);
      break;
  }
}

void execute_insert(Statement* statement, Table* table) {
  if (table->num_rows >= TABLE_MAX_ROWS) {
    printf("Error: Table full.\n");
    return;
  }
  Row* row = &(table->rows[table->num_rows]);
  row->id = statement->id;
  strcpy(row->username, statement->username);
  strcpy(row->email, statement->email);
  table->num_rows += 1;
  printf("Inserted.\n");
}

void execute_select(Table* table) {
  for (size_t i = 0; i < table->num_rows; i++) {
    Row* row = &(table->rows[i]);
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
  }
}

void repl() {
  Table table = { .num_rows = 0 };
  InputBuffer* input_buffer = new_input_buffer();

  while (true) {
    print_prompt();
    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case META_COMMAND_SUCCESS:
          continue;
        case META_COMMAND_UNRECOGNIZED_COMMAND:
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case PREPARE_SUCCESS:
        break;
      case PREPARE_UNRECOGNIZED_STATEMENT:
        printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
        continue;
    }

    execute_statement(&statement, &table);
  }
}

