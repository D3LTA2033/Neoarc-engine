# NeoArc – Fixes Applied

## Input
- **One key per frame**: `input_poll()` reads once then calls `fc_term_drain()` so the buffer is cleared and key repeat does not spam.
- **KEY_ESCAPE**: Added and handled in keymap; games quit on ESC or Q.
- **Keymap**: Handles `\033[...` extended sequences (e.g. `\033[1;2A`) by skipping digits/semicolons before the final A/B/C/D.
- **Include path**: input.c uses `../frecli/include/fc/term.h` from `engine/input/`.

## Cursor & Terminal
- **Raw only in games**: `fc_term_init()` no longer sets raw mode; only saves state. `fc_term_raw(true)` is used when entering a game, `fc_term_raw(false)` when leaving so the menu stays in cooked mode and `fgets()` works.
- **Cursor**: Cursor is hidden with `fc_term_cursor(0)` when entering a game and shown again with `fc_term_cursor(1)` on exit.
- **Alternate screen**: `fc_term_altscreen(1)` on game enter, `fc_term_altscreen(0)` on exit so the main scrollback is unchanged.
- **Clear/flush each frame**: Games call `fc_term_clear()` then `renderer_clear()` / draw / `renderer_flush()` every frame.

## Game Loops
- **Order**: Input → Update (on timer for snake) → Render → Sleep. Snake advances only when the timer fires; key sets direction and is applied on the next tick.
- **Snake**: No double tick on key; `last_key` is updated each frame and passed into `snake_tick()` only when the timer fires. Tail logic: when not eating, the oldest tail segment is removed (shift left then `tail_len--`).
- **Tetris/Platformer**: Real loops: poll key, quit on Q/ESC, clear/draw/flush, `timer_sleep_ms(50)`.

## FreCLI
- **Init/shutdown**: One init at startup, one shutdown at exit. Raw mode is toggled per game.
- **Linking**: Only `engine/frecli/lib/libfrecli.a` is linked; no FreCLI source in application targets.
- **fc_term_drain()**: Added. POSIX: `select()` with zero timeout then `read()` in a loop. Windows: `GetNumberOfConsoleInputEvents` + `ReadConsoleInput` to drain events.
- **fc_term_raw(bool)**: Added so menu stays cooked and games use raw.

## Timer
- **POSIX**: `timer_sleep_ms()` uses `nanosleep()` instead of `usleep()`.
- **Windows**: Unchanged, uses `Sleep()`.

## Memory & Stability
- **main.c**: No use-after-free; score is the return value of `snake_run()` (and tetris/platformer) before destroy.
- **renderer_destroy**: Sets `r->buf = NULL` after `free(r->buf)` before `free(r)`.
- **sprite_destroy**: Sets `s->data = NULL` after free.
- **Game run helpers**: Each allocates renderer and game, runs, then destroys both on all paths (including early failure).

## Cross-Platform
- **term.c**: `#if defined(_WIN32) || defined(_WIN64)` for Windows (handles, `ReadConsole`, `PeekConsoleInput`/`ReadConsoleInput` for drain); else POSIX (termios, `select`, `read`).
- **timer.c**: Windows `Sleep()`; POSIX `nanosleep()`.
- **logger.c**: `fopen`/`fprintf`/`fclose` are portable.

## CLI & Menu
- **Menu**: Six options (Snake, Tetris, Platformer, Learn, Help, Quit). `menu_run()` returns 1–6; main runs the chosen game or command and continues the menu loop.
- **CLI**: `./build/neoarc`, `./build/neoarc snake`, `./build/neoarc tetris`, `./build/neoarc platformer` all work; games use raw mode, altscreen, hide cursor, and exit cleanly.

## Build
- **Makefile**: Builds `engine/frecli/lib/libfrecli.a`, then links engine/games/CLI into `build/neoarc`. `make clean` removes all object files, the executable, and the static library.
- **Layout**: Unchanged; engine, games, cli, include, assets directories as before.
