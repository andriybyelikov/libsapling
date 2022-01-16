#ifndef _LIBSAPLING_LEXER_H_
#define _LIBSAPLING_LEXER_H_

/**
 * @file lexer.h
 * @brief Lexer implementation hello
 */

#include "libsapling/dm/graph.h"

void *lexer__data(const node_t node);
void lexer__access(enum qt qt, node_t *ref, void *info, predicate_t predicate,
    apply_t apply);
void lexer__print(FILE *stream, node_t *ref, fpfdata_t fpfdata);
void lexer__println(FILE *stream, node_t *ref, fpfdata_t fpfdata);
void lexer__dump_dot(FILE *stream, node_t *ref, fpfdata_t fpfdata);
int lexer__length(const node_t *ref);

/**
 * @brief Build an automaton that accepts a string literal.
 * 
 * @param dst Where to store the reference to the initial state
 * @param expr String literal surrounded by escaped double quotes
 * (e.g. @c "\"literal\\"" )
 */
void lexer__literal(node_t *dst, const char *expr);

/**
 * @brief Build an automaton that accepts a class of characters.
 * 
 * @param dst Where to store the reference to the initial state
 * @param expr String literal with the class description, the description
 * consists of opening and closing brackets [] with all the accepted characters
 * within. Furthermore, the description accepts character ranges (e.g. @c a-z )
 * and if the dash or minus sign is adjacent to either of the brackets it is
 * considered a character of the class.
 */
void lexer__class(node_t *dst, const char *expr);

/**
 * @brief Build an automaton that accepts one of any character.
 * 
 * @param dst Where to store the reference to the initial state
 */
void lexer__anything(node_t *dst);

/**
 * @brief Build a copy of a given automaton.
 * 
 * @param dst Where to store the reference to the initial state
 * @param src Reference to the initial state of the source automaton
 */
void lexer__copy(node_t *dst, node_t *src);

/**
 * @pre *dst != NULL && *src != NULL
 * 
 * @attention Union of empty graphs is not implemented.
 * 
 * @param dst First source automaton in which the union of both automata will
 * be written
 * @param src Second source automaton which will not be overwritten
 */
void lexer__union(node_t *dst, node_t *src);

/**
 * @pre *dst != NULL && *src != NULL
 * 
 * Concatenation of empty graphs is not implemented.
 * 
 * @param dst Left hand automaton in which the concatenation of both automata
 * will be written
 * @param src Right hand automaton which will not be overwritten
 */
void lexer__concatenation(node_t *dst, node_t *src);

/**
 * @pre *dst != NULL
 * 
 * Kleene star of empty graphs is not implemented.
 * 
 * @param dst Reference to the automaton to be converted to a kleene star of
 * itself
 */
void lexer__kleene_star(node_t *dst);

/**
 * @pre *dst != NULL
 * 
 * Kleene plus of empty graphs is not implemented.
 * 
 * @param dst Reference to the automaton to be converted to a kleene plus of
 * itself
 */
void lexer__kleene_plus(node_t *dst);

/**
 * @pre *dst != NULL
 * 
 * Minimization of empty graphs is not implemented.
 * 
 * @param dst Reference to the automaton to be minimized
 */
void lexer__minimize(node_t *dst);

/**
 * @brief Sets the data pointer value of all accepting state of the automaton.
 * 
 * @param dst Reference to the initial state of the automaton
 * @param data Pointer to the user data that all accepting states will be set
 * to hold
 */
void lexer__set_accepting_states_data(node_t *ref, void *data);

/**
 * @struct lexer_state
 * 
 * @brief Lexer state object
 * 
 * @var lexer_state::buf
 * Character buffer that is being processed
 * 
 * @var lexer_state::cursor
 * Current position in the buffer
 * 
 * @var lexer_state::overread
 * Following to the longest match rule, this variable keeps count of how many
 * characters have we skipped since the last acceptable lexeme was found. If
 * another pattern begins and it is not zero, that number is subtracted from
 * the current cursor position, the last acceptable lexeme is determined and
 * the search for the next terminal will begin from there.
 * 
 * @var lexer_state::ref_to_data
 * Pointer to the data pointer of a state. When an acceptable lexeme is found
 * we store the pointer to the pointer to the data of the accepting state that
 * determined the terminal.
 * 
 * @var lexer_state::lexeme
 * For each successive call to @ref lexer__next_terminal the terminal's lexeme
 * string is stored in this array.
 * 
 * @var lexer_state::end
 * Boolean that indicates the end of buffer's string.
 */
struct lexer_state {
    const char *buf;
    int cursor;
    int overread;
    void **ref_to_data;
    char lexeme[4096];
    int lexeme_cursor;
    int end;
};

/**
 * @brief Initializes a lexer state object
 * 
 * @param ref Reference to the lexer state object
 * @param buf Character buffer that will be processed
 */
void lexer__init(struct lexer_state *ref, const char *buf);

/**
 * @brief Processes the set character buffer up until it finds the next
 * acceptable lexeme and returns a pointer to the pointer to its data.
 */
void **lexer__next_terminal(node_t *ref, struct lexer_state *sta);

/**
 * @warning Remnant of debug diagnostics
 * 
 * @brief Returns an AVL map of (node_t, int)
 * 
 * This was used in lexer__to_string to print automaton state ids as small
 * numbers instead of their hexadecimal addresses
 */
node_t lexer__enumerate_states(node_t *ref);

/**
 * @warning Remnant of debug diagnostics
 * 
 * @brief Prints a lexer automaton in a somewhat-more human readable form
 */
const char *lexer__to_string(node_t *ref, node_t *map);

#endif
