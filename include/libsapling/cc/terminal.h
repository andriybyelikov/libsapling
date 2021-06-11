#ifndef LIBSAPLING_TERMINAL
#define LIBSAPLING_TERMINAL

/**
 * @file terminal.h
 * @brief Terminal implementation.
 */


typedef struct terminal *terminal_t;

/**
 * @brief Allocates a new terminal on the heap.
 * 
 * @param id The terminal's grammar symbol identifier.
 * @param lexeme The terminal's associated lexeme.
 * 
 * @returns pointer to the allocated terminal.
 */
terminal_t new_terminal(int id, const char *lexeme);

/**
 * @brief Frees the memory previously allocated on the heap for a terminal with
 * @ref new_terminal.
 */
void delete_terminal(terminal_t ref);

/**
 * @brief Returns the terminal's grammar symbol identifier.
 * @returns the terminal's grammar symbol identifier.
 */
int terminal__id(terminal_t ref);

/**
 * @brief Returns the terminal's associated lexeme.
 * @returns the terminal's associated lexeme.
 */
const char *terminal__lexeme(terminal_t ref);

#endif
