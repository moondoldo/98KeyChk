#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <sys/timeb.h>

#define KEY_STATE_GROUPS 16
#define TEXT_ROW_BYTES 160
#define REVERSE_ATTRIBUTE 0x04
#define KEYBOARD_BUFFER_BEEP_DISABLE 0x20
#define PROGRAM_TITLE "PC-98キーボードチェック 98KeyChk Ver0.1 by MoonDoldo"

struct key_display {
    unsigned char code;
    unsigned char row;
    unsigned char column;
    unsigned char width;
};

/* {キーコード, 表示行(0始まり), 表示開始桁(0始まり), 反転表示する桁数} */
static struct key_display key_displays[] = {
    {0x60, 0,  0, 4}, /* STOP */
    {0x61, 0,  5, 4}, /* COPY */
    {0x62, 0, 10, 2}, /* F1 */
    {0x63, 0, 13, 2}, /* F2 */
    {0x64, 0, 16, 2}, /* F3 */
    {0x65, 0, 19, 2}, /* F4 */
    {0x66, 0, 22, 2}, /* F5 */
    {0x67, 0, 25, 2}, /* F6 */
    {0x68, 0, 28, 2}, /* F7 */
    {0x69, 0, 31, 2}, /* F8 */
    {0x6a, 0, 34, 2}, /* F9 */
    {0x6b, 0, 37, 3}, /* F10 */
    {0x52, 0, 42, 2}, /* v1 */
    {0x53, 0, 45, 2}, /* v2 */
    {0x54, 0, 48, 2}, /* v3 */
    {0x55, 0, 51, 2}, /* v4 */
    {0x56, 0, 54, 2}, /* v5 */

    {0x00, 1,  1, 3}, /* ESC */
    {0x01, 1,  7, 1}, /* 1 */
    {0x02, 1,  9, 1}, /* 2 */
    {0x03, 1, 11, 1}, /* 3 */
    {0x04, 1, 13, 1}, /* 4 */
    {0x05, 1, 15, 1}, /* 5 */
    {0x06, 1, 17, 1}, /* 6 */
    {0x07, 1, 19, 1}, /* 7 */
    {0x08, 1, 21, 1}, /* 8 */
    {0x09, 1, 23, 1}, /* 9 */
    {0x0a, 1, 25, 1}, /* 0 */
    {0x0b, 1, 27, 1}, /* - */
    {0x0c, 1, 29, 1}, /* ^ */
    {0x0d, 1, 31, 1}, /* \ */
    {0x0e, 1, 33, 2}, /* BS */
    {0x38, 1, 36, 3}, /* INS */
    {0x39, 1, 40, 3}, /* DEL */
    {0x3e, 1, 49, 1}, /* C */
    {0x3f, 1, 51, 1}, /* H */
    {0x40, 1, 53, 1}, /* - */
    {0x41, 1, 55, 1}, /* / */

    {0x0f, 2,  1, 3}, /* TAB */
    {0x10, 2,  8, 1}, /* Q */
    {0x11, 2, 10, 1}, /* W */
    {0x12, 2, 12, 1}, /* E */
    {0x13, 2, 14, 1}, /* R */
    {0x14, 2, 16, 1}, /* T */
    {0x15, 2, 18, 1}, /* Y */
    {0x16, 2, 20, 1}, /* U */
    {0x17, 2, 22, 1}, /* I */
    {0x18, 2, 24, 1}, /* O */
    {0x19, 2, 26, 1}, /* P */
    {0x1a, 2, 28, 1}, /* @ */
    {0x1b, 2, 30, 1}, /* [ */
    {0x1c, 2, 33, 3}, /* RET */
    {0x36, 2, 38, 3}, /* RUP */
    {0x37, 2, 42, 5}, /* RDOWN */
    {0x42, 2, 49, 1}, /* 7 */
    {0x43, 2, 51, 1}, /* 8 */
    {0x44, 2, 53, 1}, /* 9 */
    {0x45, 2, 55, 1}, /* * */

    {0x74, 3,  0, 4}, /* CTRL */
    {0x71, 3,  5, 3}, /* CAP */
    {0x1d, 3,  9, 1}, /* A */
    {0x1e, 3, 11, 1}, /* S */
    {0x1f, 3, 13, 1}, /* D */
    {0x20, 3, 15, 1}, /* F */
    {0x21, 3, 17, 1}, /* G */
    {0x22, 3, 19, 1}, /* H */
    {0x23, 3, 21, 1}, /* J */
    {0x24, 3, 23, 1}, /* K */
    {0x25, 3, 25, 1}, /* L */
    {0x26, 3, 27, 1}, /* ; */
    {0x27, 3, 29, 1}, /* : */
    {0x28, 3, 31, 1}, /* ] */
    {0x3a, 3, 41, 2}, /* ↑ */
    {0x46, 3, 49, 1}, /* 4 */
    {0x47, 3, 51, 1}, /* 5 */
    {0x48, 3, 53, 1}, /* 6 */
    {0x49, 3, 55, 1}, /* + */

    {0x70, 4,  1, 5}, /* SHIFT */
    {0x29, 4, 10, 1}, /* Z */
    {0x2a, 4, 12, 1}, /* X */
    {0x2b, 4, 14, 1}, /* C */
    {0x2c, 4, 16, 1}, /* V */
    {0x2d, 4, 18, 1}, /* B */
    {0x2e, 4, 20, 1}, /* N */
    {0x2f, 4, 22, 1}, /* M */
    {0x30, 4, 24, 1}, /* , */
    {0x31, 4, 26, 1}, /* . */
    {0x32, 4, 28, 1}, /* / */
    {0x33, 4, 30, 1}, /* _ */
    {0x7d, 4, 32, 5}, /* SHIFT */
    {0x3b, 4, 39, 2}, /* ← */
    {0x3c, 4, 43, 2}, /* → */
    {0x4a, 4, 49, 1}, /* 1 */
    {0x4b, 4, 51, 1}, /* 2 */
    {0x4c, 4, 53, 1}, /* 3 */
    {0x4d, 4, 55, 1}, /* = */

    {0x72, 5,  2, 4}, /* KANA */
    {0x77, 5,  7, 3}, /* WIN(L) */
    {0x73, 5, 11, 4}, /* GRPH */
    {0x51, 5, 16, 4}, /* NFER */
    {0x34, 5, 21, 5}, /* SPACE */
    {0x35, 5, 27, 4}, /* XFER */
    {0x78, 5, 32, 3}, /* WIN(R) */
    {0x79, 5, 36, 3}, /* APP */
    {0x3d, 5, 41, 2}, /* ↓ */
    {0x4e, 5, 49, 1}, /* 0 */
    {0x4f, 5, 51, 1}, /* , */
    {0x50, 5, 53, 1}, /* . */
    {0x1c, 5, 55, 1}  /* R */
};

static unsigned key_display_count =
    sizeof(key_displays) / sizeof(key_displays[0]);

static unsigned char interrupt_return[] = {0xcf}; /* IRET */
static void (far *old_copy_handler)();
static void (far *old_stop_handler)();
static unsigned char old_keyboard_buffer_beep_state;

/* 起動中または終了中の注意画面を表示し、読めるように2秒待つ。 */
static void show_wait_screen(void)
{
    struct timeb start_time;
    struct timeb current_time;
    long elapsed_milliseconds;

    system("CLS");
    puts(PROGRAM_TITLE);
    puts("");
    puts("キーボードには触れずお待ちください");

    /* 待機前に、タイトルと注意文を確実に画面へ出力する。 */
    fflush(stdout);

    /* 待機を始めた時刻を、秒とミリ秒で取得する。 */
    ftime(&start_time);
    elapsed_milliseconds = 0;
    while (elapsed_milliseconds < 2000L)
    {
        ftime(&current_time);

        /* 秒の差をミリ秒へ換算し、1秒未満の差を加えて経過時間を求める。 */
        elapsed_milliseconds =
            (current_time.time - start_time.time) * 1000L
            + (long)current_time.millitm - (long)start_time.millitm;
    }
}

/* 終了後にタイトルだけを画面へ残す。 */
static void show_title_screen(void)
{
    system("CLS");
    puts(PROGRAM_TITLE);
}

/* 起動画面を消去し、キーボードの配置をテキストで表示する。 */
static void display_keyboard(int extended_keys)
{
    system("CLS");

    puts("STOP COPY F1 F2 F3 F4 F5 F6 F7 F8 F9 F10  v1 v2 v3 v4 v5");
    puts(" ESC   1 2 3 4 5 6 7 8 9 0 - ^ \\ BS INS DEL      C H - /");
    puts(" TAB    Q W E R T Y U I O P @ [  RET  RUP RDOWN  7 8 9 *");
    puts("CTRL CAP A S D F G H J K L ; : ]         ↑      4 5 6 +");
    if (extended_keys)
    {
        puts(" SHIFT    Z X C V B N M , . / _ SHIFT  ←  →    1 2 3 =");
        puts("  KANA WIN GRPH NFER SPACE XFER WIN APP  ↓      0 , . R");
    }
    else
    {
        puts(" SHIFT    Z X C V B N M , . / _        ←  →    1 2 3 =");
        puts("  KANA     GRPH NFER SPACE XFER          ↓      0 , . R");
    }
    puts("");
    if (extended_keys)
    {
        puts("WIN/APP Mode [終了:CTRL+C]");
    }
    else
    {
        puts("Normal Mode (WIN/APP Mode は /W 付き起動) [終了:CTRL+C]");
    }
}

/*
 * PC-98キーボードI/Oのステータス(43h)を待つ。
 * キーボードから応答がない場合も停止し続けないよう、タイムアウトを設ける。
 */
static int wait_keyboard_status(unsigned char mask)
{
    unsigned timeout;

    timeout = 65535U;
    while (timeout != 0)
    {
        if ((inp(0x43) & mask) != 0)
        {
            return 1;
        }
        timeout--;
    }
    return 0;
}

/*
 * データポート(41h)へ1バイト送信し、キーボードからのACK(FAh)を確認する。
 */
static int send_keyboard_byte(unsigned char value)
{
    /* TxRDY: キーボードへ送信できるまで待つ。 */
    if (!wait_keyboard_status(0x01))
    {
        return 0;
    }

    outp(0x41, value);

    /* RxRDY: キーボードから応答を受信できるまで待つ。 */
    if (!wait_keyboard_status(0x02))
    {
        return 0;
    }

    return inp(0x41) == 0xfa;
}

/*
 * 95hコマンドで拡張キーの動作モードを設定する。
 * modeの03hはWIN/APPキー有効、00hは通常状態を表す。
 * ACKがIRQ1のBIOS処理に先取りされないよう、送受信中だけ割り込みを禁止する。
 */
static void set_extended_key_mode(unsigned char mode)
{
    /* 現在のフラグを保存し、割り込みを一時的に禁止する。 */
    _asm_c("\n\tPUSHF\n\tCLI");

    /* キーボードインターフェースの送信を有効にする。 */
    outp(0x43, 0x17);
    if (!send_keyboard_byte(0x95))
    {
        goto finish;
    }
    if (!send_keyboard_byte(mode))
    {
        goto finish;
    }
    /* TxEMPTY: 最後のデータが送信し終わるまで待つ。 */
    if (!wait_keyboard_status(0x04))
    {
        goto finish;
    }

finish:
    /* 送信を無効にし、受信可能な通常状態へ戻してエラーをリセットする。 */
    outp(0x43, 0x16);
    /* 保存したフラグを復元し、割り込み状態を処理前に戻す。 */
    _asm_c("\n\tPOPF");
}

/*
 * STOPとCOPYは、それぞれINT 06hとINT 05hを発生させる。
 * キー状態の監視中だけIRETする処理へ差し替え、通常の割り込み処理を抑止する。
 */
static void hook_special_keys(void)
{
    struct SREGS segments;
    void (far *handler)();

    segread(&segments);
    handler = (void (far *)())MK_FP(
        segments.ds,
        (unsigned)interrupt_return
    );
    old_copy_handler = _dos_getvect(0x05);
    old_stop_handler = _dos_getvect(0x06);
    _dos_setvect(0x05, handler);
    _dos_setvect(0x06, handler);
}

/* STOP/COPYの割り込み処理を起動前の状態へ戻す。 */
static void restore_special_keys(void)
{
    _dos_setvect(0x05, old_copy_handler);
    _dos_setvect(0x06, old_stop_handler);
}

/*
 * キーを押し続けた時にキーバッファーが溢れて鳴るBEEPを抑止する。
 * BIOSフラグ(0000:0500h)のD5を変更する前に、元の状態を保存する。
 */
static void disable_keyboard_buffer_beep(void)
{
    unsigned char far *bios_flag;

    bios_flag = (unsigned char far *)MK_FP(0x0000, 0x0500);
    old_keyboard_buffer_beep_state =
        *bios_flag & KEYBOARD_BUFFER_BEEP_DISABLE;
    *bios_flag |= KEYBOARD_BUFFER_BEEP_DISABLE;
}

/* 他のBIOSフラグには触れず、BEEP抑止のD5だけを元の状態へ戻す。 */
static void restore_keyboard_buffer_beep(void)
{
    unsigned char far *bios_flag;

    bios_flag = (unsigned char far *)MK_FP(0x0000, 0x0500);
    if (old_keyboard_buffer_beep_state)
    {
        *bios_flag |= KEYBOARD_BUFFER_BEEP_DISABLE;
    }
    else
    {
        *bios_flag &= ~KEYBOARD_BUFFER_BEEP_DISABLE;
    }
}

/* BIOSのキーセンスで、全キーグループの押下状態を取得する。 */
static void read_key_states(unsigned char states[])
{
    union REGS in_regs;
    union REGS out_regs;
    int group;

    memset(&in_regs, 0, sizeof(in_regs));
    for (group = 0; group < KEY_STATE_GROUPS; group++)
    {
        in_regs.h.ah = 0x04;
        in_regs.h.al = (unsigned char)group;
        int86(0x18, &in_regs, &out_regs);
        states[group] = out_regs.h.ah;
    }
}

/* 取得済みのキー状態から、指定したキーが押されているか判定する。 */
static int is_key_pressed(unsigned char states[], unsigned char code)
{
    return (states[code >> 3] & (1 << (code & 7))) != 0;
}

/* CTRLとCの両方が離れ、BIOSが解放を処理するまで待つ。 */
static void wait_exit_keys_released(unsigned char states[])
{
    read_key_states(states);
    while (is_key_pressed(states, 0x74)
            || is_key_pressed(states, 0x2b))
    {
        read_key_states(states);
    }
}

/* 指定キーの表示範囲について、テキストVRAMの反転属性を設定・解除する。 */
static void set_key_reverse(struct key_display *key, int reverse)
{
    unsigned char far *attribute_vram;
    unsigned offset;
    int column;

    attribute_vram = (unsigned char far *)MK_FP(0xa200, 0);
    for (column = 0; column < key->width; column++)
    {
        offset = key->row * TEXT_ROW_BYTES
            + (key->column + column) * 2;
        if (reverse)
        {
            attribute_vram[offset] |= REVERSE_ATTRIBUTE;
        }
        else
        {
            attribute_vram[offset] &= ~REVERSE_ATTRIBUTE;
        }
    }
}

/* 取得したキー状態に合わせて、すべてのキー表示を更新する。 */
static void update_key_display(unsigned char states[])
{
    unsigned index;

    for (index = 0; index < key_display_count; index++)
    {
        set_key_reverse(&key_displays[index],
            is_key_pressed(states, key_displays[index].code));
    }
}

/* すべてのキー表示から反転属性を解除する。 */
static void clear_key_display(void)
{
    unsigned index;

    for (index = 0; index < key_display_count; index++)
    {
        set_key_reverse(&key_displays[index], 0);
    }
}

static void clear_keyboard_buffer(void)
{
    union REGS in_regs;
    union REGS out_regs;

    /* 監視中に蓄積したキー入力を、終了後のDOSへ渡さないよう消去する。 */
    memset(&in_regs, 0, sizeof(in_regs));
    in_regs.h.ah = 0x0c;
    in_regs.h.al = 0x06;
    in_regs.h.dl = 0xff;
    intdos(&in_regs, &out_regs);
}

int main(int argc, char *argv[])
{
    unsigned char states[KEY_STATE_GROUPS];
    int extended_keys;

    /* 初期値は0(通常モード)。WIN/APPキーは使用しない。 */
    extended_keys = 0;
    if (argc > 1)
    {
        /* /Wまたは/wが指定された場合だけ1(WIN/APPモード)にする。 */
        if (strcmp(argv[1], "/W") == 0
                || strcmp(argv[1], "/w") == 0)
        {
            extended_keys = 1;
        }
    }

    show_wait_screen();
    display_keyboard(extended_keys);

    /* /W指定時はWIN/APPキーを有効にする。 */
    if (extended_keys)
    {
        /*
         * EXE起動に使ったRETが離されるまで、拡張モードの設定を開始しない。
         * RETの解放コードをコマンドの応答として読み捨てる競合を防ぐ。
         */
        read_key_states(states);
        while (is_key_pressed(states, 0x1c))
        {
            read_key_states(states);
        }
        set_extended_key_mode(0x03);
    }

    disable_keyboard_buffer_beep();
    hook_special_keys();

    /* CTRL+Cが押されるまで、キー状態の取得と表示の更新を繰り返す。 */
    for (;;)
    {
        read_key_states(states);
        update_key_display(states);
        if (is_key_pressed(states, 0x74)
                && is_key_pressed(states, 0x2b))
        {
            break;
        }
    }

    /* 終了時に残っている、すべてのキーの反転表示を解除する。 */
    clear_key_display();

    /* CTRL+Cが離れてから、DOSに中断されないよう入力を消去する。 */
    wait_exit_keys_released(states);
    clear_keyboard_buffer();
    show_wait_screen();

    /* 2秒の待機中にキーが押された場合も、離されるまで待つ。 */
    wait_exit_keys_released(states);

    /* /W指定時はWIN/APPキーを無効にし、通常モードへ戻す。 */
    if (extended_keys)
    {
        set_extended_key_mode(0x00);
    }

    clear_keyboard_buffer();
    restore_special_keys();
    restore_keyboard_buffer_beep();
    show_title_screen();
    return 0;
}
