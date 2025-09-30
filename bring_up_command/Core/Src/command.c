/*
 * command.c
 *
 *  Created on: Sep 30, 2025
 *      Author: rodrigo-che
 */

#include "command.h"
#include "main.h" // Para acesso a HAL_GPIO_WritePin, etc. e a huart2
#include "fw_version.h"
#include <string.h>
#include <stdio.h>

// --- Protótipos das funções de comando (privadas a este módulo) ---
static void cmd_led_on(void);
static void cmd_led_off(void);
static void cmd_version(void);
static void cmd_help(void);

extern UART_HandleTypeDef huart2;

// Função auxiliar para enviar respostas ao console
static void console_print(const char* str) {
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

// --- Tabela de Comandos ---
// Esta é a implementação do "registro" do Command Pattern.
// Para adicionar um novo comando, basta criar a função e adicioná-la aqui.
static const Command commands[] = {
    {"led on",    cmd_led_on,    "Liga o LED de usuário (LD2)."},
    {"led off",   cmd_led_off,   "Desliga o LED de usuário (LD2)."},
    {"version",   cmd_version,   "Mostra a versão do firmware."},
    {"help",      cmd_help,      "Mostra esta ajuda."}
};

static const int num_commands = sizeof(commands) / sizeof(commands[0]);


// --- Implementação das Funções de Comando ---
static void cmd_led_on(void) {
    // Assumindo que o LED do usuário (LD2) está no pino PA5 na placa Nucleo
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    console_print("LED ON\r\n");
}

static void cmd_led_off(void) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    console_print("LED OFF\r\n");
}

static void cmd_version(void) {
    char buffer[32];
    sprintf(buffer, "Firmware v%s\r\n", FW_VERSION); // FIRMWARE_VERSION definido no main.h
    console_print(buffer);
}

static void cmd_help(void) {
    console_print("--- Comandos Disponiveis ---\r\n");
    for (int i = 0; i < num_commands; i++) {
        char buffer[128];
        sprintf(buffer, "%-10s: %s\r\n", commands[i].name, commands[i].help_text);
        console_print(buffer);
    }
    console_print("---------------------------\r\n");
}

// --- Processador de Comandos ---
void command_parser_process(char *command_string) {
    if (strlen(command_string) == 0) {
        return;
    }
    uint16_t sizeOfcmd = strlen(command_string);
    for (int i = 0; i < num_commands; i++) {
        if (strcmp(command_string, commands[i].name) == 0) {
            commands[i].action(); // Encontrou o comando, executa a função associada
            return;
        }
    }

    console_print("Comando nao reconhecido. Digite 'help' para ver a lista.\r\n");
}

/*** end of file ***/
