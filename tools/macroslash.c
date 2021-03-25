#include <stdio.h>

int main(int argc, const char *argv[])
{
    FILE *input_file = fopen(argv[1], "r");
    if (input_file != NULL) {
        int col = 1;
        char c = getc(input_file);
        while (c != EOF) {
            switch (c) {
            case '\n':
                while (col < 80 - 1) {
                    putc(' ', stdout);
                    col++;
                }
                putc('\\', stdout);
                putc('\n', stdout);
                col = 0;
                break;
            default:
                putc(c, stdout);
            }
            c = getc(input_file);
            col++;
        }
    }
}