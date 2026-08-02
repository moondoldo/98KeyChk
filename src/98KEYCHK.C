#include <stdio.h>

static void display_keyboard(void)
{
    puts("STOP COPY F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 (v1 v2 v3 v4 v5)");
    puts(" ESC   1 2 3 4 5 6 7 8 9 0 - ^ \\ BS INS DEL      C H - /");
    puts(" TAB    Q W E R T Y U I O P @ [  RET  RUP RDOWN  7 8 9 *");
    puts("CTRL CAP A S D F G H J K L ; : ]         UP      4 5 6 +");
    puts(" SHIFT    Z X C V B N M , . / _ SHIFT LEFT RIGHT 1 2 3 -");
    puts("  KANA WIN GRPH NFER SPACE XFER WIN APP DOWN     0 , . R");
}

int main(void)
{
    display_keyboard();
    return 0;
}
