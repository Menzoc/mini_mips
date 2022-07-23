[[_TOC_]]

# Minimal MIPS project
This project aims to create a minimal MIPS processor emulator using C++20.  
It is composed in 2 parts, an assembly to binary converter/compiler and the processor emulator.

### Assembly to binary converter
This part use a simple assembly pseudo language and convert it to a binary file readable by the processor emulator.

```
   ______                       ______
  |       \                    |       \
  |        |                   |        |
  |        |  --> asm2bin -->  |        |
  |        |                   |        |
  |________|                   |________|
   assembly                      binary
     file                         file
```

The pseudo assembly language support a list of determined instructions and the use of LABELS to an instruction index.

Labels are followed by a colon (`:`) and may include capital letters, lower case letters, number or the underscore character.  
Labels cannot start with a number and a number cannot start with the character `R` or `r` followed only by numbers. They are also case-sensitive which means that `MY_LABEL` and `my_label` are 2 different labels.  
Valid label examples are `MY_LABEL:`, `MY_LABEL: add r1,r2,r3`, `_LABEL1:`

Supported instructions are the following :
- `r` is a register name (e.g. r0, r1, r2...)
- `o` is a register name or an integer value (e.g. 12, -45...)
- `a` is an integer value

| syntax        | instruction                     | effect                                                                                                                                                                                                                                            |
|---------------|---------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| add r1,o,r2   | integer addition                | r2 receive r1+o                                                                                                                                                                                                                                   |
| sub r1,o,r2   | integer substraction            | r2 receive r1-o                                                                                                                                                                                                                                   |
| mult r1,o,r2  | integer multiplication          | r2 receive r1*o                                                                                                                                                                                                                                   |
| div r1,o,r2   | integer division                | r2 receive r1/o                                                                                                                                                                                                                                   |
| and r1,o,r2   | bitwise and                     | r2 receive r1 "and" r2                                                                                                                                                                                                                            |
| or r1,o,r2    | bitwise or                      | r2 receive r1 "or" r2                                                                                                                                                                                                                             |
| xor r1,o,r2   | bitwise xor                     | r2 receive r1 "xor" r2                                                                                                                                                                                                                            |
| shl r1,o,r2   | bitwise shift to the right      | r2 receive r1 shifted of o bits to the right                                                                                                                                                                                                      |
| shr r1,o,r2   | bitwise shift to the left       | r2 receive r1 shifted of o bits to the left                                                                                                                                                                                                       |
| slt r1,o,r2   | inferior test                   | r2 receive 1 if r1<o, 0 else                                                                                                                                                                                                                      |
| sle r1,o,r2   | inferior or equal test          | r2 receive 1 if r1<=o, 0 else                                                                                                                                                                                                                     |
| seq r1,o,r2   | equal test                      | r2 receive 1 if r1=o, 0 else                                                                                                                                                                                                                      |
| load r1,o,r2  | memory read                     | r2 receive the content written at the r1+o address                                                                                                                                                                                                |
| store r1,o,r2 | memory write                    | r2 content is written at the r1+o address                                                                                                                                                                                                         |
| jmp o,r       | jump to an instruction          | jump to the instruction at address o et save the address of the next instruction in r                                                                                                                                                             |
| braz r,a      | jump to an instruction if 0     | jump to the instruction at address a only if r = 0                                                                                                                                                                                                |
| branz r,a     | jump to an instruction if not 0 | jump to the instruction at address a only if r != 0                                                                                                                                                                                               |
| scall n       | system call                     | custom command: (0 -> print to stdout the value stored in register  `R1`, 1 -> ask the user to print a number in the command line and store it in register `R1`, 2 -> update the 100x100 px bitmap screen that use the 10000 first memory values) |
| stop          | stop the execution              | end of the program                                                                                                                                                                                                                                |

### Processor emulator
This part use the binary file «compiled» by the `asm2bin` tool and execute it on an emulated minimal MIPS processor.

This processor support the use a register memory of a fixed size defined by the `register_memory` constant in the `emulator_globals.h` header.    
It can also use a «raw» memory of a fixed size defined by the `data_memory` constant in the same header. This memory can be either directly modified or modified through the use of a cache represented by the `MemoryProcessor` class.

The first 10000 memory values are used to display a bitmap screen of 100x100 pixels. Note that for readability reasons, the screen is scaled to 800x800 pixels.

The emulator show a theoretical latency that can be used to have an idea on how well the emulated program is performing. 

# Folder Architecture
The project use the following architecture :
```
├─ assembly_files       // List of assembly like programs that can be used as examples
└─ src                  // Project sources folder
    ├─ demo             // List of demonstration executables 
    ├─ asm2bin          // Source for the assembly to binary converter
    ├─ helpers          // List of helpers functions and global variables
    └─ emulator         // Source for the minimal mips emulator
```

# Requirements

> We recommend running the project under a linux architecture as it as not been tested under windows.

To run this project the following packages are needed :
 - SDL2 version 2.0

# Compiling project

### Compilation requirements
To compile the project the following tools and packages are needed :
- cmake (version 3.16.3 or greater)
- gcc/g++ (version 11.1.0 or greater)
- libsdl2

To install them under an ubuntu/debian environment use :
```
sudo apt install cmake libsdl2-dev gcc-11 g++-11
```

>As gcc-11 (or greater) packages are not directly available under Ubuntu 20.04 it has to be compiled manually.   
Another solution is to add the following PPA to the apt source list:    
`sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test`

### Compilation step by step
We recommend to use Clion integrated environnement to build the project, but it can be done from the following command lines. 

__Create and move to the build folder :__
```
mkdir build
cd build
```
__Configure project :__
- For an x86 architecture
    ```
    cmake env WORKING_DIR=./ ..
    ```
- For a 64bits arm architecture
    ```
    cmake --DCMAKE_C_COMPILER=/usr/bin/aarch64-linux-gnu-gcc-11 -DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++-11 env WORKING_DIR=./ ..
    ```

__Compile :__
- For all targets
    ```
    make
    ```
- For a specific targets
   ```
   make <target_name>
   ``` 

> target binaries are available in the `build/bin` folder

# Using project

For each subproject, a demo executable and a command line tool are compiled.

### Assembly to binary converter
- The `ASM2BIN_DEMO` executable can be used to test the assembly to binary converter on the assembly file `assembly_files/assembly_file_1.asm`.
- The `asm2bin` command line tool can be used to convert any given assembly file to binary.   
  > Use `./asm2bin -h` to print tool option and usage help

### Processor emulator

- The `EMULATOR_DEMO` executable can be used to test the emulator on the binary file `assembly_files/assembly_file_1.o`.
  > Note that this binary file is not directly present in the git repository, it has to be generated using either the `ASM2BIN_DEMO` binary or the `asm2bin` tool. 
- The `emulator` command line tool can be used to emulate any given binary file.   
  > Use `./emulator -h` to print tool option and usage help
