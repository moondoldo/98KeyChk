#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#define KEY_STATE_GROUPS 16
#define TEXT_ROW_BYTES 160
#define REVERSE_ATTRIBUTE 0x04

struct key_display {
    unsigned char code;
    unsigned char row;
    unsigned char column;
    unsigned char width;
};

static struct key_display key_displays[] = {
    {0x60, 0,  0, 4}, {0x61, 0,  5, 4},
    {0x62, 0, 10, 2}, {0x63, 0, 13, 2},
    {0x64, 0, 16, 2}, {0x65, 0, 19, 2},
    {0x66, 0, 22, 2}, {0x67, 0, 25, 2},
    {0x68, 0, 28, 2}, {0x69, 0, 31, 2},
    {0x6a, 0, 34, 2}, {0x6b, 0, 37, 3},
    {0x52, 0, 42, 2}, {0x53, 0, 45, 2},
    {0x54, 0, 48, 2}, {0x55, 0, 51, 2},
    {0x56, 0, 54, 2},

    {0x00, 1,  1, 3},
    {0x01, 1,  7, 1}, {0x02, 1,  9, 1},
    {0x03, 1, 11, 1}, {0x04, 1, 13, 1},
    {0x05, 1, 15, 1}, {0x06, 1, 17, 1},
    {0x07, 1, 19, 1}, {0x08, 1, 21, 1},
    {0x09, 1, 23, 1}, {0x0a, 1, 25, 1},
    {0x0b, 1, 27, 1}, {0x0c, 1, 29, 1},
    {0x0d, 1, 31, 1}, {0x0e, 1, 33, 2},
    {0x38, 1, 36, 3}, {0x39, 1, 40, 3},
    {0x3e, 1, 49, 1}, {0x3f, 1, 51, 1},
    {0x40, 1, 53, 1}, {0x41, 1, 55, 1},

    {0x0f, 2,  1, 3},
    {0x10, 2,  8, 1}, {0x11, 2, 10, 1},
    {0x12, 2, 12, 1}, {0x13, 2, 14, 1},
    {0x14, 2, 16, 1}, {0x15, 2, 18, 1},
    {0x16, 2, 20, 1}, {0x17, 2, 22, 1},
    {0x18, 2, 24, 1}, {0x19, 2, 26, 1},
    {0x1a, 2, 28, 1}, {0x1b, 2, 30, 1},
    {0x1c, 2, 33, 3}, {0x36, 2, 38, 3},
    {0x37, 2, 42, 5},
    {0x42, 2, 49, 1}, {0x43, 2, 51, 1},
    {0x44, 2, 53, 1}, {0x45, 2, 55, 1},

    {0x74, 3,  0, 4}, {0x71, 3,  5, 3},
    {0x1d, 3,  9, 1}, {0x1e, 3, 11, 1},
    {0x1f, 3, 13, 1}, {0x20, 3, 15, 1},
    {0x21, 3, 17, 1}, {0x22, 3, 19, 1},
    {0x23, 3, 21, 1}, {0x24, 3, 23, 1},
    {0x25, 3, 25, 1}, {0x26, 3, 27, 1},
    {0x27, 3, 29, 1}, {0x28, 3, 31, 1},
    {0x3a, 3, 41, 2},
    {0x46, 3, 49, 1}, {0x47, 3, 51, 1},
    {0x48, 3, 53, 1}, {0x49, 3, 55, 1},

    {0x70, 4,  1, 5},
    {0x29, 4, 10, 1}, {0x2a, 4, 12, 1},
    {0x2b, 4, 14, 1}, {0x2c, 4, 16, 1},
    {0x2d, 4, 18, 1}, {0x2e, 4, 20, 1},
    {0x2f, 4, 22, 1}, {0x30, 4, 24, 1},
    {0x31, 4, 26, 1}, {0x32, 4, 28, 1},
    {0x33, 4, 30, 1}, {0x7d, 4, 32, 5},
    {0x3b, 4, 38, 4}, {0x3c, 4, 43, 5},
    {0x4a, 4, 49, 1}, {0x4b, 4, 51, 1},
    {0x4c, 4, 53, 1}, {0x4d, 4, 55, 1},

    {0x72, 5,  2, 4}, {0x77, 5,  7, 3},
    {0x73, 5, 11, 4}, {0x51, 5, 16, 4},
    {0x34, 5, 21, 5}, {0x35, 5, 27, 4},
    {0x78, 5, 32, 3}, {0x79, 5, 36, 3},
    {0x3d, 5, 40, 4},
    {0x4e, 5, 49, 1}, {0x4f, 5, 51, 1},
    {0x50, 5, 53, 1}, {0x1c, 5, 55, 1}
};

static unsigned key_display_count =
    sizeof(key_displays) / sizeof(key_displays[0]);

static void display_keyboard(int extended_keys)
{
    puts("STOP COPY F1 F2 F3 F4 F5 F6 F7 F8 F9 F10  v1 v2 v3 v4 v5");
    puts(" ESC   1 2 3 4 5 6 7 8 9 0 - ^ \\ BS INS DEL      C H - /");
    puts(" TAB    Q W E R T Y U I O P @ [  RET  RUP RDOWN  7 8 9 *");
    puts("CTRL CAP A S D F G H J K L ; : ]         UP      4 5 6 +");
    puts(" SHIFT    Z X C V B N M , . / _ SHIFT LEFT RIGHT 1 2 3 =");
    if (extended_keys)
        puts("  KANA WIN GRPH NFER SPACE XFER WIN APP DOWN     0 , . R");
    else
        puts("  KANA     GRPH NFER SPACE XFER         DOWN     0 , . R");
    puts("");
    puts("[\x8f\x49\x97\xb9:CTRL+C]");
}

static void set_extended_key_mode(unsigned char mode)
{
    outp(0x43, 0x17);
    while ((inp(0x43) & 0x01) == 0)
        ;
    outp(0x41, 0x95);
    while ((inp(0x43) & 0x01) == 0)
        ;
    outp(0x41, mode);
    outp(0x43, 0x16);
}

static void read_key_states(unsigned char states[])
{
    union REGS in_regs;
    union REGS out_regs;
    int group;

    memset(&in_regs, 0, sizeof(in_regs));
    for (group = 0; group < KEY_STATE_GROUPS; group++) {
        in_regs.h.ah = 0x04;
        in_regs.h.al = (unsigned char)group;
        int86(0x18, &in_regs, &out_regs);
        states[group] = out_regs.h.ah;
    }
}

static int is_key_pressed(unsigned char states[], unsigned char code)
{
    return (states[code >> 3] & (1 << (code & 7))) != 0;
}

static void set_key_reverse(struct key_display *key, int reverse)
{
    unsigned char far *attribute_vram;
    unsigned offset;
    int column;

    attribute_vram = (unsigned char far *)MK_FP(0xa200, 0);
    for (column = 0; column < key->width; column++) {
        offset = key->row * TEXT_ROW_BYTES
            + (key->column + column) * 2;
        if (reverse)
            attribute_vram[offset] |= REVERSE_ATTRIBUTE;
        else
            attribute_vram[offset] &= ~REVERSE_ATTRIBUTE;
    }
}

static void update_key_display(unsigned char states[])
{
    unsigned index;

    for (index = 0; index < key_display_count; index++) {
        set_key_reverse(&key_displays[index],
            is_key_pressed(states, key_displays[index].code));
    }
}

static void clear_key_display(void)
{
    unsigned index;

    for (index = 0; index < key_display_count; index++)
        set_key_reverse(&key_displays[index], 0);
}

static void clear_keyboard_buffer(void)
{
    while (kbhit())
        getch();
}

int main(int argc, char *argv[])
{
    unsigned char states[KEY_STATE_GROUPS];
    int extended_keys;

    extended_keys = argc > 1
        && (strcmp(argv[1], "/W") == 0
            || strcmp(argv[1], "/w") == 0);

    system("CLS");
    display_keyboard(extended_keys);
    if (extended_keys)
        set_extended_key_mode(0x03);

    for (;;) {
        read_key_states(states);
        update_key_display(states);
        if (is_key_pressed(states, 0x74)
                && is_key_pressed(states, 0x2b))
            break;
    }

    clear_key_display();
    if (extended_keys)
        set_extended_key_mode(0x00);
    clear_keyboard_buffer();
    return 0;
}
