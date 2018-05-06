# Peg solitaire

## Compiling and running

You will need CMake and a compiler toolkit.

In order to build `moofoopeg` just run:

```
cmake -B_build -H. -DCMAKE_CXX_FLAGS="-stdlib=libc++"
cmake --build _build
```

Then you will need to copy the media files to the `_build` directory. And you are done!

Run the game with `_build/moofoopeg`.

Use arrow keys and space to play the game.

