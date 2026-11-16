# strace

A minimal strace clone built with ptrace, for x86-64 Linux.

Each step is the same program compiled with a different `LEVEL`:

| Binary    | Output                                        |
|-----------|-----------------------------------------------|
| strace_0  | syscall number                                |
| strace_1  | syscall name                                  |
| strace_2  | name and return value (hex)                   |
| strace_3  | name, raw hex parameters, return value        |
| strace_4  | adds `char *` strings                         |
| strace_5  | adds integer parameters and return values     |
| strace_6  | adds NULL and mmap/open/access flags          |
| strace_7  | adds read/write buffers                       |
| strace_8  | adds `struct stat` (st_mode, st_size)         |

## Build

    make strace_N      # N from 0 to 8
    make               # builds all

## Usage

    ./strace_N /full/path/to/command [args...]
