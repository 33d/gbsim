[gbsim][5] Windows port
===

Building
---

1. Install [msys2][1] (I used 64 bit), and update it as described on its home page
1. Install these packages (`pacman -S ...`): 
 - mingw-w64-i686-toolchain
 - mingw-w64-i686-libelf
 - mingw-w64-i686-SDL2 
 - mingw-w64-i686-cmake (NOT cmake)
 - make
1. Update the path for mingw: `export PATH=$PATH:/mingw32/bin`
1. `/mingw32/bin/cmake -G 'MSYS Makefiles' .`
1. `make`
1. The binary is in `build/gbsim.exe`

Before distributing the file, run `ldd` on it to confirm that it depends only on Windows files:

    $ ldd build/gbsim.exe
        ntdll.dll => /c/WINDOWS/SYSTEM32/ntdll.dll (0x771b0000)
        KERNEL32.DLL => /c/WINDOWS/SYSTEM32/KERNEL32.DLL (0x76310000)
        KERNELBASE.dll => /c/WINDOWS/SYSTEM32/KERNELBASE.dll (0x76190000)
        GDI32.dll => /c/WINDOWS/SYSTEM32/GDI32.dll (0x74290000)
        USER32.dll => /c/WINDOWS/SYSTEM32/USER32.dll (0x76f00000)

[1]: http://msys2.github.io/
[5]: https://github.com/33d/gbsim
