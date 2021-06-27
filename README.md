# DPLL-SAT-Solver

Simple implementation of the
[Davis–Putnam–Logemann–Loveland (DPLL)](https://en.wikipedia.org/wiki/DPLL_algorithm)
algorithm for solving the satisfiability problem of propositional logic.


## Requirements

No dependencies except default vala libraries glib, gio, gee.


## Building

Build default build using make:

```
make
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
