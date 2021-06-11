#ifndef LIBSAPLING_TEXT
#define LIBSAPLING_TEXT

/**
 * @file text.h
 * @brief Text utilities
 */


/**
 * @brief Escapes some characters in a string.
 * 
 * @param dst Pointer to the destination array where the escaped string will be
 *   stored.
 * @param src String to be escaped.
 * 
 * @attention Only escapes the following characters:
 * - \c \\ (Backslash)
 * - \c \" (Double quotation mark)
 * - \c \t (Horizontal Tab)
 * - \c \r (Carriage Return)
 * - \c \n (Newline/Line Feed)
 * 
 * @warning The destination array must be long enough to contain the escaped
 *   string (including the terminating null character). To be safe it should be
 *   twice as long as the original string plus one for the terminating null
 *   character (worst case: the original string only contains escapable
 *   characters). The terminating null character IS copied over to the
 *   destination array.
 */
void text__escape(char *dst, const char *src);

/**
 * @brief Unescapes some characters in a string.
 * 
 * @param dst Pointer to the destination array where the unescaped string will
 *   be stored.
 * @param src String to be unescaped.
 * 
 * @attention Only unescapes the following characters:
 * - \c \\ (Backslash)
 * - \c \" (Double quotation mark)
 * - \c \t (Horizontal Tab)
 * - \c \r (Carriage Return)
 * - \c \n (Newline/Line Feed)
 * 
 * @warning The destination array must be long enough to contain the unescaped
 *   string (including the terminating null character). To be safe it should be
 *   as long as the original string plus one for the terminating null character
 *   (worst case: the original string contains no characters to unescape). The
 *   terminating null character IS copied over to the destination array.
 */
void text__unescape(char *dst, const char *src);

#endif
