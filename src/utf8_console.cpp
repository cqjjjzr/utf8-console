#include "utf8_console.h"

#ifdef WIN32
void initialize_utf8_console_windows();
#endif

void initialize_utf8_console()
{
#ifdef WIN32
    initialize_utf8_console_windows();
#endif
}