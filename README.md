# Concurrent Batch File Processor

A C program that executes a command on multiple files concurrently using fork and exec system calls.

## Overview

This program allows you to run a command on multiple files in parallel. It creates child processes for each file and executes the specified command, then waits for all processes to complete.

## Build

gcc ./src/main.c -o batchrun

## How to Use

```bash
./src/batchrun <command> <file1> <file2> <file3> ...
```

Example:
```bash
./src/batchrun wc tests/Text\ File.txt tests/Text\ File\ \(1\).txt
```

## Limitations

- Maximum 10 files can be processed at once (total arguments limited to 12)
- Error handling could be improved for edge cases
- Could add a MakeFile in future