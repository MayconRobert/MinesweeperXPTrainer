# Windows XP Minesweeper Trainer

A Command-Line Interface Trainer/Cheater created for studying the internal behavior of the classic Windows XP Minesweeper game. This project was built as an educational exercise to explore Windows application internals, memory manipulation, and reverse engineering techniques in a controlled environment.

## Deployment

To compile this project, simply run the following to compile to 32 Bits. If you will play on a 64 Bits machine, feel free to remove the `-m32` flag

```bash
  gcc -m32 main.c utils.c cheats.c -o MinesweeperHack
```

or

```bash
  gcc -m32 *.c -o MinesweeperHack
```

## Features

- Pause and Resume Timer
- Show Mines
- Block and Unblock Mines

## FAQ

#### Disclaimer

This Minesweeper Trainer was tested on the original Windows XP Minesweeper game and the following executable downloaded from [Web Archive](https://archive.org/details/Mircosoft-Minesweeper-2001-XP). Other versions were not tested and may not work properly
