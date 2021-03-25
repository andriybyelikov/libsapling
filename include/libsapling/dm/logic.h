#ifndef LIBSAPLING_LOGIC
#define LIBSAPLING_LOGIC

/**
 * @file logic.h
 * @brief Logic related definitions
 */

/** @brief Quantifier definitions */
enum qt {

    /** Universal quantifier */
    U_QT,

    /** Existential quantifier */
    E_QT
};

/**
 * @brief Generic comparator function prototype.
 * @returns Should return the difference between the two values.
 */
typedef int (*cmp_t)(const void *a, const void *b);

#endif
