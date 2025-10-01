/**
 * @file command_parser.h
 * @brief Interface for the command parser module.
 *
 * This module defines the command structure and parser function
 * used to interpret console commands via UART on the Nucleo L073 board.
 *
 * @date Sep 30, 2025
 * @author
 *   Rodrigo Che
 */

#ifndef SRC_COMMAND_PARSER_H_
#define SRC_COMMAND_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
 * @brief Function pointer type for command execution callbacks.
 */
typedef void (*ExecuteCommand)();

/**
 * @struct Command
 * @brief Represents a single console command.
 *
 * A command has a name, an action (callback function),
 * and a short help text that describes its purpose.
 */
typedef struct {
  const char* name;          ///< Command string typed by the user
  ExecuteCommand action;     ///< Function executed when the command is matched
  const char* help_text;     ///< Short description of the command
} Command;

/**
 * @brief Parses and executes a command string.
 *
 * This function compares the given string against all registered
 * commands and, if a match is found, executes the associated action.
 *
 * @param command_string Null-terminated string containing the command.
 */
void CommandParserProcess(const uint8_t* command_string);

#ifdef __cplusplus
}
#endif

#endif  // SRC_COMMAND_PARSER_H_
