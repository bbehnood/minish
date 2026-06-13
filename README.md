# Minish

Minish is a lightweight Unix shell implementation written in C. It provides a simple interactive command-line environment with support for command execution, built-in commands, and pipelines.

## Features

* Execute external commands
* Built-in commands:

  * `cd`
  * `pwd`
  * `exit`
* Command pipelines (`|`)
* Output redirection (`<`, `>`, `>>`)
* Interactive command prompt
* Line editing and history support via Readline

## Requirements

* C compiler (`clang` or `gcc`)
* CMake
* Readline development library

### Installing Readline

#### Ubuntu / Debian

```bash
sudo apt install libreadline-dev
```

#### Fedora

```bash
sudo dnf install readline-devel
```

#### Arch Linux

```bash
sudo pacman -S readline
```

#### macOS (Homebrew)

```bash
brew install readline
```

## Building

Create a build directory and configure the project with CMake:

```bash
mkdir build

cmake -S . -B build \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_BUILD_TYPE=Release
```

Alternatively, use GCC:

```bash
cmake -S . -B build \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_BUILD_TYPE=Release
```

Build the project:

```bash
cmake --build build
```

## Running

From the build directory:

```bash
./minish
```

## Usage Examples

### Execute Commands

```bash
minish$ ls
minish$ echo hello
```

### Change Directory

```bash
minish$ cd ~/projects
minish$ pwd
```

### Pipelines

```bash
minish$ ls -la | grep ".c"
```

### Exit the Shell

```bash
minish$ exit
```

## Project Goals

Minish is a learning-oriented shell project focused on understanding:

* Process creation with `fork`
* Program execution with `exec`
* Inter-process communication through pipes
* Command parsing
* Unix process and file descriptor management

## Repository

GitHub: https://github.com/bbehnood/minish

## License

Distributed under the Apache License 2.0. See the `LICENSE` file for details.

