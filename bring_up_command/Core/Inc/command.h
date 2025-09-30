/*
 * command_parser.h
 *
 *  Created on: Sep 30, 2025
 *      Author: rodrigo-che
 */

#ifndef COMMAND_PARSER_H_
#define COMMAND_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

// Function pointer to execute an action
typedef void (*ExecuteCommand)(void);

// Definição da interface de um comando
// Um comando tem um nome, uma função para executar e um texto de ajuda.
typedef struct {
    const char *name;
    ExecuteCommand action;
    const char *help_text;
} Command;

// Função principal que recebe uma string e a processa
void command_parser_process(char *command_string);

#ifdef __cplusplus
}
#endif

#endif /* INC_COMMAND_PARSER_H_ */
