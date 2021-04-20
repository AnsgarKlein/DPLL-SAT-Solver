# DPLL-SAT-Solver

Simple implementation of the
[Davis–Putnam–Logemann–Loveland (DPLL)](https://en.wikipedia.org/wiki/DPLL_algorithm)
algorithm for solving the satisfiability problem of propositional logic written
in C99 without external dependencies.


## Requirements

No dependencies except a C compiler (clang and gcc are tested)


## Building

### Setup build directory

Create build directory for out-of-tree build:

```
mkdir build
cd build
```

### Generate build files

CMake generates files for another build system that will then
be used for the actual build.

Generate default CMake build files:

```
cmake ..
```

Generate Makefile:

```
cmake -G 'Unix Makefiles' ..
```

Generate Ninja files:

```
cmake -G 'Ninja' ..
```

### Configure build

Specify C compiler:

```
cmake -DCMAKE_C_COMPILER=clang ..
```

Create release binary:

```
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Create binary with debug information:

```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Build

Build using Makefile:

```
make
```

Build using Ninja:

```
ninja
```


## Running

Run solver on DIMACS file:

```
build/dpll --dimacs < input.txt
```

Run solver on plain text formula:

```
echo "{A, B}, {-A, -B}, {-C}, {E, D}" | build/dpll --cnf
```

Run solver on plain text formula with custom format:

```
echo "(A v B) ^ (-A v -B) ^ (-C) ^ (E v D)" | build/dpll --cnf --cstart '(' --cend ')' --cdel 'v' --cneg '-'
```


## License

This software is released under the
[GPL v.3](https://www.gnu.org/licenses/gpl-3.0.html).
