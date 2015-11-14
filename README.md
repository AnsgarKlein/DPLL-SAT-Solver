# DPLL-SAT-Solver

Simple implementation of the
[Davis–Putnam–Logemann–Loveland (DPLL)](https://en.wikipedia.org/wiki/DPLL_algorithm)
algorithm for solving the satisfiability problem of propositional logic written
in C99 without external dependencies.


## Requirements

No dependencies except a C compiler (clang and gcc are tested)


## Building

Build default build using make:

```
make
```

Specify compiler:

```
CC=clang make
```

Build optimized binary:

```
make optimized
```

Build binary with debug information:

```
make debug
```

Build binary with verbose output:

```
VERBOSE_DPLL=1 make
```


## Running

Run solver on plain text formula:

```
echo "{A, B}, {-A, -B}, {-C}, {E, D}" | ./dpll --cnf
```


## License

This software is released under the
[GPL v.3](https://www.gnu.org/licenses/gpl-3.0.html).