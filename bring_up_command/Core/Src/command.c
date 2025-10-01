/**
 * @file command.c
 * @brief Command parser implementation for Nucleo L073 board console.
 *
 * This module implements a simple command parser using the Command Pattern.
 * It provides user commands via UART to control the LED, print firmware version,
 * and display available commands.
 *
 * @date Sep 30, 2025
 * @author
 *   Rodrigo Che
 */

#include "command.h"
#include "main.h"       // For HAL_GPIO_WritePin, huart2, LD2_GPIO_Port, etc.
#include "fw_version.h"
#include <string.h>
#include <stdio.h>

/// Forward declarations of private command handler functions
static void cmd_led_on(void);
static void cmd_led_off(void);
static void cmd_version(void);
static void cmd_help(void);

/// UART handle defined externally in main.c
extern UART_HandleTypeDef huart2;

/**
 * @brief Helper function to send strings to the console over UART.
 *
 * @param str Null-terminated string to be transmitted.
 */
static void console_print(const char* str) {
  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

/**
 * @brief Table of supported console commands.
 *
 * Each entry contains the command string, the handler function,
 * and a short help text.
 */
static const Command kCommands[] = {
    {"led-on",  cmd_led_on,  "Turn on user LED (LD2)."},
    {"led-off", cmd_led_off, "Turn off user LED (LD2)."},
    {"version", cmd_version, "Show firmware version."},
    {"help",    cmd_help,    "Show this help message."}
};

/// Number of registered commands in the table
static const int kNumCommands = sizeof(kCommands) / sizeof(kCommands[0]);

// -----------------------------------------------------------------------------
// Command Handler Implementations
// -----------------------------------------------------------------------------

/**
 * @brief Turns the user LED (LD2) on.
 */
static void cmd_led_on(void) {
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  console_print("LED ON\r\n");
}

/**
 * @brief Turns the user LED (LD2) off.
 */
static void cmd_led_off(void) {
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  console_print("LED OFF\r\n");
}

/**
 * @brief Prints the firmware version to the console.
 */
static void cmd_version(void) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "Firmware v%s\r\n", FW_VERSION);
  console_print(buffer);
}

/**
 * @brief Prints a list of all available commands.
 */
static void cmd_help(void) {
  console_print("--- Available Commands ---\r\n");
  for (int i = 0; i < kNumCommands; i++) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%-10s: %s\r\n",
             kCommands[i].name, kCommands[i].help_text);
    console_print(buffer);
  }
  console_print("---------------------------\r\n");
}

// -----------------------------------------------------------------------------
// Command Parser
// -----------------------------------------------------------------------------

/**
 * @brief Processes a command string entered by the user.
 *
 * This function compares the given command string against
 * the registered command table and executes the corresponding action.
 *
 * @param command_string Null-terminated string containing the command.
 */
void command_parser_process(const uint8_t* command_string) {
  if (strlen((const char*)command_string) == 0) {
    return;
  }

  for (int i = 0; i < kNumCommands; i++) {
    if (strcmp((const char*)command_string, kCommands[i].name) == 0) {
      kCommands[i].action();  ///< Execute associated command handler
      return;
    }
  }

  console_print("Command not recognized. Type 'help' to see the list.\r\n");
}

/*** end of file ***/
