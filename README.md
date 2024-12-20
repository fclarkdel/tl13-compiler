# TL13 Compiler

## Description

I used flex, bison, and C to create a minimal compiler for the TL13 language. I also created a simple test suite to
verify the correctness of the implementation.

## Scope

This implementation does not do everything a compiler could do, this was a learning exercise and is not meant to be used for compiling anything that will be put into production.

It is capable of catching syntax errors, doing some type checking, verifying the range of numerical literals, and detecting if a variable has been declared already or not at all.

## Platform

This project was developed on an x86-64 system running Ubuntu 22.04.5. I have only ever run it on this system, so I only
know for sure it works on this platform.

## Usage

To build the compiler run ```sh build.sh```, the binary will be in the build directory as ```tl13```.

To run the test suite run ```sh test.sh```.

To silence the output of the test suite and only show the pass fail results run ```sh test.sh --silent```.
