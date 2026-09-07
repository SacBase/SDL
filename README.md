# SaC SDL3 Module

## Note

Due to a bug in the private heap manager, requires the `-noPHM` flag.

## About

This is a SaC module that wraps around the [SDL3](https://www.libsdl.org/)
library and can be used for visualisation or graphical interfaces.

## Build Instructions

You'll need to have installed `sac2c` and its standard library.
You'll also need to have at least `pthreads` and the `SDL3` C-library (`libsdl3-dev libsdl3-image-dev libsdl3-ttf-dev`).

The project uses `cmake-common`. Upon cloning this repo it is important initialize submodules.

```bash
git submodule update --init --recursive
make
```

## Variables that can be passed to CMake

When running CMake it is possible to pass the following variables:
 * `-DTARGETS=x;y;z`: build for targets x, y and z.

   *Default:* `seq;seq_checks;mt_pth`
 * `-DSAC2C_EXEC=/path/to/sac2c`: specify `sac2c` executable directly. Otherwise CMake will try to find it on your path.
 * `-DLINKSETSIZE=n`: set `-linksetsize n` when calling `sac2c`.
   This option is responsible for the number of C functions that are put in a single C file when compiling a SaC program.
   The rule of thumb:
    * `0` is the fastest time-wise but potentially results in a large memory consumption.
    * `1` reduces the memory consumption to minimum, buy significantly increases compilation time.

      *Default: 500*
