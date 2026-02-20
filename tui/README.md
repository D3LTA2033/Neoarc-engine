# NeoArc

C-based terminal game and learning engine using FreCLI for TUI.  
Games: [NeoArc-engine (GitHub)](https://github.com/D3LTA2033/Neoarc-engine/tree/main/games)

## Build

```
make
```

Output: `build/neoarc`

## Run

```
./build/neoarc
./build/neoarc snake
./build/neoarc tetris
./build/neoarc platformer
```

## CLI

- **Search** – Search games (from repo list); pick by number.
- **Play** – Submenu: Snake, Tetris, Platformer.
- **Settings** – FPS cap (30/60/120/Uncapped), box theme (ASCII, Double, Round, Rounded).
- **Credits** – @emexos (Co-Founder, TUI/frecli), @mcs.s (Founder, backend & systems).
- Version **v1.0.0** shown at bottom of main menu.

Config: `~/.config/neoarc/config` (Linux/macOS) or `%APPDATA%\NeoArc\config` (Windows).

## Structure

- engine/ FreCLI, renderer, input, physics, utils
- games/normal/ snake, tetris, platformer
- learning/ tutorials, challenges, progress
- cli/ main, commands, menu, config, box
