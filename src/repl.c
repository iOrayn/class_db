#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "repl.h"
#include "btree.h"

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

void print_prompt() {
    printf("db > ");
}

void print_help() {
    printf("Welcome to the database C Project Made By Dylan 3SI3!\n");
    printf("Available commands:\n");
    printf("  insert <id> <username> <email>   - Insert a new record into the database\n");
    printf("  select                          - Display all records in the database\n");
    printf("  select <id>                     - Display a specific record by ID\n");
    printf("  delete <id>                     - Delete a record by ID\n");
    printf("  .exit                           - Exit the application\n");
    printf("\n");
}

void read_input(InputBuffer* input_buffer) {
    if (input_buffer->buffer == NULL) {
        input_buffer->buffer = malloc(1024);
        input_buffer->buffer_length = 1024;
    }
    if (fgets(input_buffer->buffer, input_buffer->buffer_length, stdin) == NULL) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    input_buffer->input_length = strlen(input_buffer->buffer);
    if (input_buffer->buffer[input_buffer->input_length - 1] == '\n') {
        input_buffer->buffer[input_buffer->input_length - 1] = '\0';
        input_buffer->input_length--;
    }
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
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %31s %254s",
                                   &statement->id, statement->username, statement->email);
        if (args_assigned < 3) {
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strncmp(input_buffer->buffer, "select", 6) == 0) {
        int args_assigned = sscanf(input_buffer->buffer, "select %d", &statement->id);
        if (args_assigned == 1) {
            statement->type = STATEMENT_SELECT;
            return PREPARE_SUCCESS;
        } else {
            statement->type = STATEMENT_SELECT;
            statement->id = -1; 
            return PREPARE_SUCCESS;
        }
    }
    if (strncmp(input_buffer->buffer, "delete", 6) == 0) {
        int args_assigned = sscanf(input_buffer->buffer, "delete %d", &statement->id);
        if (args_assigned == 1) {
            statement->type = STATEMENT_DELETE;
            return PREPARE_SUCCESS;
        }
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement, Table* table) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            execute_insert(statement, table);
            break;
        case STATEMENT_SELECT:
            execute_select(statement, table);
            break;
	case STATEMENT_DELETE:
	    table->root = delete_by_id(table->root, statement->id);
            printf("Deleted ID %d.\n", statement->id);
            break;
    }
}

void execute_insert(Statement* statement, Table* table) {
    Row row = { .id = statement->id };
    strcpy(row.username, statement->username);
    strcpy(row.email, statement->email);

    if (insert_row(table, &row)) {
        printf("Inserted.\n"); 
    }
}


void execute_select(Statement* statement, Table* table) {
    if (statement->id == -1) {
        btree_select(table);
    } else {
        Row* result = find_by_id(table->root, statement->id);
        if (result != NULL) {
            printf("(%d, %s, %s)\n", result->id, result->username, result->email);
        } else {
            printf("No record found with ID %d.\n", statement->id);
        }
    }
}



void repl() {
    Table table = { .root = NULL, .num_rows = 0 };
    InputBuffer* input_buffer = new_input_buffer();
    print_help();

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
                execute_statement(&statement, &table);
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }
    }
}
