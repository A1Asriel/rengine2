## Настройка

### Ubuntu

```bash
$ sudo apt install build-essentials cmake libsdl2-dev libgl1-mesa-dev libopengl-del
```

Для кросс-компиляции:

```bash
$ sudo apt install gcc-mingw-w64
```

### Windows

Установить [CMake](https://cmake.org/), [MinGW](https://github.com/niXman/mingw-builds-binaries/releases/tag/15.1.0-rt_v12-rev0) и [Ninja](https://ninja-build.org/) и добавить пути к их исполняемым файлам в `PATH`, если необходимо.

## Сборка под Linux

```bash
$ mkdir build && cmake . -B./build && cd build && make
```

## Сборка под Windows

1. Необходимо скачать [заголовочные файлы SDL2 для MingGW](https://github.com/libsdl-org/SDL/releases/download/release-2.32.4/SDL2-devel-2.32.4-mingw.zip) и скопировать папку `x86_64-w64-mingw32` в `sdl2-mingw` в корне проекта.
2. В Windows:  
   ```cmd
   mkdir build && cmake . -Bbuild -DCMAKE_TOOLCHAIN_FILE:FILEPATH=cmake\mingw-w64-x86_64.cmake -G Ninja && cmake --build build
   ```
   В Linux:  
   ```bash
   $ mkdir build && cmake . -B./build -DCMAKE_TOOLCHAIN_FILE:FILEPATH=./cmake/mingw-w64-x86_64.cmake && cd build && make
   ```
