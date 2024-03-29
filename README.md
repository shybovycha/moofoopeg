# Peg solitaire

![screenshot](screenshot.png)

## Compiling and running

You will need [CMake](https://cmake.org/), [vcpkg](https://github.com/microsoft/vcpkg) and a compiler toolkit.

In order to build `moofoopeg` just run:

```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_PATH/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

Then you will need to copy the media files to the `build` directory. And you are done!

Run the game with `build/moofoopeg`.

## Playing the game

The goal of the game is to remove all the pegs from the board.
The peg is removed once you "jump" over it with another peg.
A "jump" is the move when you take a peg and move it two spaces in one direction into an empty spot,
bypassing another peg:

```
    /-- the peg you are jumping over
    |
    v
[o] o _
 ^    ^
 |    |--- empty spot
 |
 \- current peg
```

result:

```
_ _ [o]
^ ^
| |
 v
 |---- empty spots
```

Use arrow keys to move the cursor and <kbd>Space</kbd> to select the peg or the new space for it (confirm the move).
