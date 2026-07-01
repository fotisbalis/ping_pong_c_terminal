# Ping Pong

A simple terminal-based Pong-style game written in C with `ncurses`.

## What the Game Does

This project runs in the terminal and offers two game modes:

- `Singleplayer`: control the left paddle and keep the ball in play against the right wall.
- `Multiplayer`: two local players control one paddle each.

The game also stores the highest all-time single-player score in `highest_score.txt`.

## Controls

- `W` / `S`: move the left paddle up and down
- `Up Arrow` / `Down Arrow`: move the right paddle in multiplayer
- `Q`: quit

## Requirements

You need a C development environment with:

- `gcc`
- `make`
- `ncurses` development libraries

This project is intended for a Unix-like terminal environment such as:

- Linux
- macOS
- WSL on Windows

## How to Build

From the project folder, run:

```sh
make
```

This uses:

```sh
gcc ping_pong.c -std=c11 -o ping_pong -lncurses
```

## How to Run

After building, start the game with:

```sh
./ping_pong
```

When the game starts, choose:

- `S` for single-player
- `M` for multiplayer
- `Q` to quit

## Clean Build Output

To remove the compiled executable, run:

```sh
make clean
```

## Notes

- The game reads and writes `highest_score.txt`, so keep that file in the same directory as the executable.
- If you are on Windows, the easiest way to run this project is usually through WSL, since `ncurses` is not a standard native Windows library.
