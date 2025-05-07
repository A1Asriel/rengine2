## Настройка

### Ubuntu

```bash
$ sudo apt install build-essentials cmake libsdl2-dev libgl1-mesa-dev libopengl-del gcovr
```

Для кросс-компиляции:

```bash
$ sudo apt install gcc-mingw-w64
```

### Windows

Установить [CMake](https://cmake.org/), [MinGW](https://github.com/niXman/mingw-builds-binaries/releases/tag/15.1.0-rt_v12-rev0) и [Ninja](https://ninja-build.org/) и добавить пути к их исполняемым файлам в `PATH`, если необходимо.

## Сборка под Linux

> [!WARNING]
> Перед первой сборкой небходимо создать папку `build`.

### Обычная сборка

```bash
cmake build && cmake --build build --target rengine && build/rengine
```

### Покрытие тестов

```bash
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCOVERAGE:BOOL=true build && cmake --build build --target coverage
```

### Профилирование

```bash
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DPROFILING:BOOL=true build && cmake --build build --target profile
```

## Сборка под Windows

> [!WARNING]
> Перед первой сборкой необходимо создать папку `build`, скачать [заголовочные файлы SDL2 для MingGW](https://github.com/libsdl-org/SDL/releases/download/release-2.32.4/SDL2-devel-2.32.4-mingw.zip) и скопировать папку `x86_64-w64-mingw32` из архива в `sdl2-mingw` в корне проекта.

### Обычная сборка

```cmd
cmake -DCMAKE_TOOLCHAIN_FILE:FILEPATH=./cmake/mingw-w64-x86_64.cmake build && cmake --build build --target rengine && build\rengine.exe
```

### Покрытие тестов

```cmd
cmake -DCMAKE_TOOLCHAIN_FILE:FILEPATH=./cmake/mingw-w64-x86_64.cmake -DCOVERAGE build && cmake --build build --target coverage
```

### Профилирование

```cmd
cmake -DCMAKE_TOOLCHAIN_FILE:FILEPATH=./cmake/mingw-w64-x86_64.cmake -DPROFILING build && cmake --build build --target profile
```
