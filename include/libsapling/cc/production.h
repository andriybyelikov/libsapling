#ifndef LIBSAPLING_PRODUCTION
#define LIBSAPLING_PRODUCTION

/**
 * @file production.h
 * @brief Production implementation.
 */

#include "libsapling/dm/graph.h"


typedef struct production *production_t;

/**
 * @brief Allocates a new production on the heap.
 * 
 * @param id The production's gramar symbol identifier.
 * @param body The production's body.
 * 
 * @returns pointer to the allocated production.
 */
production_t new_production(int id, node_t *body);

/**
 * @brief Frees the memory previously allocated on the heap for a production
 * with @ref new_production.
 */
void delete_production(production_t ref);

/**
 * @brief Returns the productions's grammar symbol identifier.
 * @returns the terminal's grammar symbol identifier.
 */
int production__id(production_t ref);

/**
 * @brief Returns the length of the production's body.
 * @returns the length of the production's body.
 */
int production__len(production_t ref);

/**
 * @brief Returns the grammar symbol at the specified position in the
 * production's body.
 * @returns the grammar symbol at the specified position in the production's
 * body.
 */
int *production__get(production_t ref, int pos);

#endif
