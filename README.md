# Brainfu Interpreter

Brainfu is an optimizing bytecode interpreter for the esoteric programming language
Brainfuck, it has an interactive REPL mode, error reporting (work in progress) it can
also run files.

## Usage

Get the repository, create a directory and build it.

```shell script
git clone https://github.com/ambertide/brainfuck.git
cd brainfuck
mkdir build && cd build
cmake .. && make all
```

You can now use the `./brainfu` file to play with the language. When called without any
arguments, this will open an interactive prompt, when called with a file, it will attempt
to execute the file.

## Language
which simulates a basic Turing machine with an *infinite* tape and a pointer
on the tape. The tape consists of cells, initially valued at 0.

It has 6 commands and 1 macro for modifying the values of the cell in tape, moving the pointer, 
looping in the code itself and input/output.

| Command | Meaning |
| :-----: | ------- |
|   \>    | Move pointer to the right. |
|   \<    | Move pointer to the left.  |
|   +     | Increment the value of the cell pointer is pointing at. |
|   -     | Decrement the value of the cell pointer is pointing at. |
|   [     | Jump forward to the command after the matching `]` character if the cell pointer value is  0. |
|   ]     | Jump back to the command after the matching `[` character if the cell pointer value is not zero. |
|   .     | Output the ASCII value of the character at cell pointer to the `stdout` |
|   ,     | Get a character from `stdin` and write its value to the cell pointer† |
|   !     | A special macro, characters entered before `!` are taken as input†. |

† Feature not yet implemented.

#### Notes

* If the cell pointer is at 0. `<` does nothing.
* Cell values are between 0 and 256, they wrap around each other.
* In the REPL mode, if you enter `d`, the debug mode will be unlocked, this will allow 
the user to see the emitted optimized bytecode.
* Input taken from `!` is exhausted by `,` before `stdin` is used.

## Implementation Details

### Bytecodes

Understanding underlying bytecodes might be helpful, there are five bytecodes. Each take
one operand.

| Op Code | Operand | Description |
|  :---:  |  :---:  | ----------- |
| OP_MOVE | X (int) | Move the pointer on tape by `X`.
| OP_CHANGE | X (int) | Change the value of the cell pointed by `X`. May overflow.
| OP_JUMP | X (int) | Jump to the command at `X + 1` proivded that either (1) If `A` is current address and `X < A` check if cell pointed is not 0 (2) otherwise check if 0.
| OP_INPUT | 0       | Take input, first from an internal stack then `stdin`. Write it to cell pointed.
| OP_OUTPUT| 0       | Write the value at cell pointed to `stdout`.
