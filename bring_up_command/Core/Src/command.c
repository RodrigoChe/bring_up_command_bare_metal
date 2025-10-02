// command.c
// Created on: Sep 30, 2025
// Author: Rodrigo Che
//
// Command parser implementation for UART console commands.

#include "command.h"
#include "main.h"       // For HAL_GPIO_WritePin, etc.
#include "fw_version.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;  // Declared in main.c

// -----------------------------------------------------------------------------
// Internal function prototypes
// -----------------------------------------------------------------------------
static void CmdLedOn();
static void CmdLedOff();
static void CmdVersion();
static void CmdHelp();
static void ConsolePrint(const char* str);

// -----------------------------------------------------------------------------
// Command table (acts as the "registry" for the command pattern)
// -----------------------------------------------------------------------------
static const Command kCommands[] = {
    {"led-on",   CmdLedOn,   "Turn on the user LED (LD2)."},
    {"led-off",  CmdLedOff,  "Turn off the user LED (LD2)."},
    {"version",  CmdVersion, "Show firmware version."},
    {"help",     CmdHelp,    "Show this help message."}
};

static const int kNumCommands = sizeof(kCommands) / sizeof(kCommands[0]);

// -----------------------------------------------------------------------------
// Internal helper functions
// -----------------------------------------------------------------------------
/**
 * @brief Sends a string over UART (blocking).
 *
 * @param str Null-terminated string to send.
 */
static void ConsolePrint(const char* str) {
  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

/**
 * @brief Command: Turn LED on.
 */
static void CmdLedOn() {
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  ConsolePrint("LED ON\r\n");
}

/**
 * @brief Command: Turn LED off.
 */
static void CmdLedOff() {
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  ConsolePrint("LED OFF\r\n");
}

/**
 * @brief Command: Show firmware version.
 */
static void CmdVersion() {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "Firmware V%s\r\n", FW_VERSION);
  ConsolePrint(buffer);
}

/**
 * @brief Command: Show list of available commands.
 */
static void CmdHelp() {
  ConsolePrint("--- Available Commands ---\r\n");
  for (int i = 0; i < kNumCommands; ++i) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%-10s: %s\r\n",
             kCommands[i].name, kCommands[i].help_text);
    ConsolePrint(buffer);
  }
  ConsolePrint("---------------------------\r\n");
}

// -----------------------------------------------------------------------------
// Public function implementation
// -----------------------------------------------------------------------------
/**
 * @brief Parses a command string and executes the associated action.
 *
 * @param command_string Null-terminated string with command input.
 */
void CommandParserProcess(const uint8_t* command_string) {
  if (strlen((const char*)command_string) == 0) {
    return;
  }

  for (int i = 0; i < kNumCommands; ++i) {
    if (strcmp((const char*)command_string, kCommands[i].name) == 0) {
      kCommands[i].action();  // Execute associated function
      return;
    }
  }

  ConsolePrint("Unrecognized command. Type 'help' for a list.\r\n");
}
