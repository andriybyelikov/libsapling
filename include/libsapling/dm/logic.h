#ifndef _LIBSAPLING_DM_LOGIC_H_
#define _LIBSAPLING_DM_LOGIC_H_

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
 * @brief Generic comparator function prototype
 */
typedef int (*cmp_t)(const void *a, const void *b);

#endif
