SAC SDL Module
==============

About
-----

This is a SaC module that wraps around the [SDL](https://www.libsdl.org/)
(version 1) library and can be used for visualisation or graphical
interfaces.

Build Instructions
------------------

You'll need to have installed `sac2c` and have a copy of the Stdlib
installed as well. You'll also need to have `SDL` version 1 C-library and most
likely operational `pthreads`. NB. The SDL version 2 library is not, at present,
supported.

```sh
git submodule update --init --recursive
mkdir build
cd build
cmake <OPTIONS> ..
make -j4
```

Variables that can be passed to CMake
-------------------------------------

When running CMake it is possible to pass the following variables:
  * `-DTARGETS=x;y;z` --- build stdlib for targets x, y and z. (defaults are `seq; mt_pth`)
  * `-DSAC2C_EXEC=/path/to/sac2c` --- specify `sac2c` executable directly. Otherwise CMake will
    try to find `sac2c` on yout PATH.
  * `-DLINKSETSIZE=n` --- set `-linksetsize n` when calling `sac2c`.  This option is responsible
    for the number of C functions that are put in a single C file when compiling a SaC program.
    The rule of thumb:
    * value `0` is the fastest time-wise but potentially results in a large memory consumption
    * value `1` reduces the memory consumption to minimum, buy significantly increases compilation time.

      *Default value: 500.*
