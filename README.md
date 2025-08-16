

https://github.com/user-attachments/assets/3a1d5d18-25c6-448e-a70f-811eda509c1f



# NastyTetris
Making tetris without worrying too much about the quality of the code and structure of it more for a learning experience.

## Install
Download game from [itch.io](https://aheathfield.itch.io/nastytetris)

## How To Play
- Use mouse to click play button
- Left and Right arrow keys to move side to side
- Down arrow key to fast fall
- Up arrow key to rotate
- Space to hold a shape
- Type with any key if you got a highscore

## Bugs
- I know that there is some bugs in this game, it was mainly to learn so I won't be fixing any bugs

## Assets
- I made all assets.

## Tools Used (all free and open source)
- Neovim (for code)
- GIMP (for art)
- LMMS (for sfx and music)

### To build run this in root directory (only tested on Linux):
```
cmake -S . -B build
cmake --build build
```

### For a windows build (via mingw)
```
cmake -S . -B build_win -DCMAKE_TOOLCHAIN_FILE=mingw_toolchain.cmake
cmake --build build_win
```
