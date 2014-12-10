
/**namespace Tests {
    private struct CNFSolutionTuple {
        string cnf;
        bool satisfiable;
    }
    
    private struct DimacsSolutionTuple {
        string cnf;
        bool satisfiable;
    }
    
    private const CNFSolutionTuple[] cnfs = {
        { "{ A,B}, {-A, -B},{ -C }, {lit, D}", true },
        { "{A}, {-A}, {C}, {literalname}, {C}", false },
        { "{q, -p, -r, s}, {-q, -r, s}, {r}, {-p, -s}, {-p, r}", true },
        { "{-p, q, -r, s}, {-q, -r, s}, {r}, {-p, -s}, {-p, r}", true },
        { "{A,B,C}, {-A,C,D}, {-A,C,-D}, {-A, -C, D}, {-A, -C, -D}, {A, B, -C}, {A, -B, C}", true },
        { "{s, -q}, {-p, q, s}, {p}, {r, -s}, {-p, -r, -s}", false },
        { "{-q, s}, {-p, q, s}, {p}, {r, -s}, {-p, -r, -s}", false }
    };
    
    private const DimacsSolutionTuple[] dimacs = {
        { "c foo\nc bar\np cnf 5 4\n"
            +"1 2 0\n"
            +"-1 -2 0\n"
            +"3 0\n"
            +"4 5 0", true },
        { "c foo\nc bar\np cnf 3 5\n"
            +"1 0\n"
            +"-1 0\n"
            +"2 0\n"
            +"3 0\n"
            +"2 0", false },
        { "c foo\nc bar\np cnf 4 5\n"
            +"2 -1 -3 4 0\n"
            +"-2 -3 4 0\n"
            +"3 0\n"
            +"-1 -4 0\n"
            +"-1 3 0", true },
        { "c foo\nc bar\np cnf 4 5\n"
            +"-1 2 -3 4 0\n"
            +"-2 -3 4 0\n"
            +"3 0\n"
            +"-1 -4 0\n"
            +"-1 3 0", true },
        { "c foo\nc bar\np cnf 4 7\n"
            +"1 2 3 0\n"
            +"-1 3 4 0\n"
            +"-1 3 -4 0\n"
            +"-1 -3 4 0\n"
            +"-1 -3 -4 0\n"
            +"1 2 -3 0\n"
            +"1 -2 3 0" , true },
        { "c foo\nc bar\np cnf 4 5\n"
            +"4 -2 0\n"
            +"-1 2 4 0\n"
            +"1 0\n"
            +"3 -4 0\n"
            +"-1 -3 -4 0"
            , false },
        { "c foo\nc bar\np cnf 4 5\n"
            +"-2 4 0\n"
            +"-1 2 4 0\n"
            +"1 0\n"
            +"3 -4 0\n"
            +"-1 -3 -4 0"
            , false }
    };
    
    private static void test_all() {
        // Test correctness
        if (!correctness_test()) {
            stdout.printf("Implementation is not correct!\n");
            return;
        } else {
            stdout.printf("Passed correctness test...\n");
        }
        
        // Test performance
        double performance = performance_test(1000);
        stdout.printf("Performance Test: %fs\n", performance);
    }
    
    private static double performance_test(uint repetitions) {
        // Extract tests
        GLib.List<string> strings = new GLib.List<string>();
        foreach (CNFSolutionTuple tup in cnfs) {
            strings.append(tup.cnf);
        }
        
        // Start testing
        Timer timer = new Timer();
        
        for (int i = 0; i < repetitions; i++) {
            foreach (string str in strings) {
                Formula formula = Parser.CNF.parse_formula(str);
                formula.dpll();
            }
        }
        
        timer.stop();
        return timer.elapsed();
    }
    
    private static bool correctness_test() {
        foreach (CNFSolutionTuple tup in cnfs) {
            if (test_cnf(tup) == false) {
                return false;
            }
        }
        
        foreach (DimacsSolutionTuple tup in dimacs) {
            if (test_dimacs(tup) == false) {
                return false;
            }
        }
        
        return true;
    }
    
    private static bool test_cnf(CNFSolutionTuple tup) {
        Formula formula = Parser.CNF.parse_formula(tup.cnf);
        
        return tup.satisfiable == formula.dpll();
    }
    
    private static bool test_dimacs(DimacsSolutionTuple tup) {
        Formula formula = Parser.Dimacs.parse_formula(tup.cnf.split("\n"));
        
        return tup.satisfiable == formula.dpll();
    }
}**/
