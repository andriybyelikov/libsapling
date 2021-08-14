#include <string.h>

void text__escape(char *dst, const char *src)
{
    for (int i = 0; i < strlen(src); i++) {
        char c = src[i];
        switch (c) {
        case '\\':
            *dst++ = '\\';
            *dst++ = '\\';
            break;
        case '\"':
            *dst++ = '\\';
            *dst++ = '\"';
            break;
        case '\t':
            *dst++ = '\\';
            *dst++ = 't';
            break;
        case '\r':
            *dst++ = '\\';
            *dst++ = 'r';
            break;
        case '\n':
            *dst++ = '\\';
            *dst++ = 'n';
            break;
        default:
            *dst++ = c;
            break;
        }
    }
    *dst++ = 0;
}

void text__unescape(char *dst, const char *src)
{
    for (int i = 0; i < strlen(src); i++) {
        char c = src[i];
        if (c == '\\') {
            c = src[++i];
            switch (c) {
            case '\\':
                *dst++ = '\\';
                break;
            case '\"':
                *dst++ = '\"';
                break;
            case 't':
                *dst++ = '\t';
                break;
            case 'r':
                *dst++ = '\r';
                break;
            case 'n':
                *dst++ = '\n';
                break;
            default:
                break;
            }
        } else {
            *dst++ = c;
        }
    }
    *dst++ = 0;
}
