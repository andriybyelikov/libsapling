#ifndef _LIBSAPLING_PARSER_LR0_ITEM_H_
#define _LIBSAPLING_PARSER_LR0_ITEM_H_

/**
 * @file lr0_item.h
 * @brief LR(0) item implementation.
 */

#include "libsapling/cc/production.h"

typedef struct lr0_item *lr0_item_t;

/**
 * @brief Allocates a new LR(0) item on the heap.
 * 
 * @returns pointer to the allocated grammar.
 */
lr0_item_t new_lr0_item(production_t production, int cursor);

/**
 * @brief Frees the memory previously allocated on the heap for an LR(0) item
 * with @ref new_lr0_item.
 */
void delete_lr0_item(lr0_item_t item);

/**
 * @brief Returns the LR(0) item's production.
 * @returns the LR(0) item's production.
 */
production_t lr0_item__production(lr0_item_t item);

/**
 * @brief Returns the LR(0) item's cursor position.
 * @returns the LR(0) item's cursor position.
 */
int lr0_item__cursor(lr0_item_t item);

int lr0_item__equals(const void *a, const void *b);

void lr0_item__print(FILE *stream, const void *data);

#endif
