gbsim is an emulator for the [Gamebuino](http://gamebuino.com/).  It's based on [simavr](https://gitorious.org/simavr/).

Usage
======

    $ gbsim <elf-file>

To get the .elf file from Arduino, go to File/Preferences, then check "Show verbose output during compiling".  Then click "Verify", and the .elf file should be shown in the output window.

Status
------

The graphics and input partially works.  Notably the [3D dungeon demo](http://gamebuino.com/wiki/index.php?title=3D_dungeon_rendering) doesn't work.  It probably uses more CPU time than it needs to.

[gdb](https://www.gnu.org/software/gdb/) support would be nice, and simavr already supports it.

What doesn't work: Everything else, including:

 - Sound
 - I&sup2;C
 - SD card
 - EEPROM
 - gdb
 - Screen brightness
 - Battery
 - Backlight

Keys
------

 - `W`, `S`, `A`, `D`: Up, down, left, right
 - `K`: A button
 - `L`: B button
 - `R`: C button
 - `ESC`: Quit

Building
======

You'll need [SDL 2](http://libsdl.org/).

simavr probably wants OpenGL, Glut and avr-gcc to build, but shouldn't be necessary.

Linux
------
`make` should work fine.  The binary will be in `gbsim/[your-platform]`.  It dynamically links simavr, which could be annoying if you try moving it.

Others
------

Maybe look at [simavr's instructions](https://gitorious.org/simavr/pages/GetStarted).  On Windows, [MinGW](http://mingw.org/) and [MSYS](http://mingw.org/wiki/MSYS) is probably your best bet; failing that, try [Cygwin](http://cygwin.com/).
