#include <assert.h>
#include <string.h>
#include "libsapling/cc/terminal.h"

int main(void)
{
    // build terminal
    terminal_t terminal = new_terminal(0, "main");

    assert(terminal__id(terminal) == 0);
    assert(!strcmp(terminal__lexeme(terminal), "main"));
}
