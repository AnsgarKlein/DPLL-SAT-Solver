
public static int main(string[] args) {
    // Print help and exit if started with "--help" or "-h"
    foreach (string arg in args) {
        if (arg == "-h" || arg == "--help") {
            stdout.printf("Usage:\n");
            stdout.printf("  %s [OPTIONS...] < FILE\n", args[0]);
            stdout.printf("\n");
            stdout.printf("\n");
            stdout.printf("Options:\n");
            stdout.printf("  -h --help\t\tPrint this help\n");
            stdout.printf("  -c --cnf\t\tIndicate that the given formula is in CNF format.\n");
            stdout.printf("  \t\t\tYou can customize this format using:\n");
            stdout.printf("    --cstart CHAR\tuse CHAR as starting character of a clause\n");
            stdout.printf("  \t\t\tin a formula in CNF form.\n");
            stdout.printf("    --cend CHAR\t\tuse CHAR as ending character of a clause\n");
            stdout.printf("  \t\t\tin a formula in CNF form.\n");
            stdout.printf("    --cdel CHAR\t\tuse CHAR as delimiting character inside clauses\n");
            stdout.printf("  \t\t\tof a formula in CNF form.\n");
            stdout.printf("    --cneg CHAR\t\tuse CHAR as negating character in front of literals\n");
            stdout.printf("  \t\t\tinside clauses in a formula in CNF form.\n");
            stdout.printf("  -d --dimacs\t\tIndicate that the given formula is in DIMACS format.\n");
            stdout.printf("\n");
            stdout.printf("\n");
            stdout.printf("Examples:\n");
            stdout.printf("  echo \"{A, B}, {-A, -B}, {-C}, {E, D}\" | %s --cnf\n", args[0]);
            stdout.printf("  \t\t\tIf you don't give detailed options '{’ and ’}’ are\n");
            stdout.printf("  \t\t\tthe default characters for starting and ending a\n");
            stdout.printf("  \t\t\tclause.\n");
            stdout.printf("  \t\t\t',' delimits literals and '-' is used for negating.\n");
            stdout.printf("\n");
            stdout.printf("  echo \"{A, B}; {-A, -B}  {-C}{E, D}\" | %s --cnf\n", args[0]);
            stdout.printf("  \t\t\tEverything between clauses is ignored.\n");
            stdout.printf("\n");
            stdout.printf("  echo \"(A v B) ^ (-A v -B) ^ (-C) ^ (E v D)\" | \\\n");
            stdout.printf("  %s --cnf --cstart \"(\" --cend \")\" --cdel \"v\"\n", args[0]);
            stdout.printf("  \t\t\tYou can easily customize the details of the CNF form\n");
            stdout.printf("  \t\t\tif your formula is not represented as a set but as\n");
            stdout.printf("  \t\t\tas a conjunction of disjunctions.\n");
            return 0;
        }
    }
    
    // Check if inputs are sane
    {
        string[] optionals = {"--cstart", "--cend", "--cdel", "--cneg"};
        foreach (string str in optionals) {
            if (str in args && !("-c" in args || "--cnf" in args)) {
                stdout.printf("%s needs --cnf\n", str);
                return 1;
            }
        }
    }
    
    // Apply options
    for (int i = 0; i < args.length; i++) {
        if (args[i] == "--cstart") {
            if (args.length == i + 1) {
                stderr.printf("--cstart needs a parameter!\n");
                return 1;
            }
            
            if (args[i+1].char_count() > 1) {
                stderr.printf("--cstart parameter can only be one character!\n");
                return 1;
            }
            
            Parser.CNF.CLAUSE_START = args[i+1].get_char(0);
        } else if (args[i] == "--cend") {
            if (args.length == i + 1) {
                stderr.printf("--cend needs a parameter!\n");
                return 1;
            }
            
            if (args[i+1].char_count() > 1) {
                stderr.printf("--cend parameter can only be one character!\n");
                return 1;
            }
            
            Parser.CNF.CLAUSE_END = args[i+1].get_char(0);
        } else if (args[i] == "--cdel") {
            if (args.length == i + 1) {
                stderr.printf("--cdel needs a parameter!\n");
                return 1;
            }
            
            if (args[i+1].char_count() > 1) {
                stderr.printf("--cdel parameter can only be one character!\n");
                return 1;
            }
            
            Parser.CNF.LITERAL_DELIMITER = args[i+1].get_char(0);
        } else if (args[i] == "--cneg") {
            if (args.length == i + 1) {
                stderr.printf("--cneg needs a parameter!\n");
                return 1;
            }
            
            if (args[i+1].char_count() > 1) {
                stderr.printf("--cneg parameter can only be one character!\n");
                return 1;
            }
            
            Parser.CNF.NEGATE_CHAR = args[i+1].get_char(0);
        }
    }
    
    // Read formula from stdin
    string formula_str;
    {
        GLib.StringBuilder string_builder = new GLib.StringBuilder();
        char[] buffer = new char[1024];
        while (!stdin.eof()) {
            string chunk = stdin.gets(buffer);
            if (chunk != null) {
                string_builder.append(chunk);
            }
        }
        formula_str = string_builder.str;
    }
    
    // Decide whether formula is in dimacs format or in cnf format.
    Formula formula = null;
    {
        bool dimacs_format = false;
        bool cnf_format = false;
        
        // if format is set on command line we'll use that
        if ("--dimacs" in args || "-d" in args) {
            dimacs_format = true;
        } else if ("--cnf" in args || "-c" in args) {
            cnf_format = true;
        }
        
        // if format is not set we'll try to guess it.
        if (!dimacs_format && !cnf_format) {
            if (formula_str.contains(Parser.CNF.CLAUSE_START.to_string())) {
                cnf_format = true;
            }
        }
        
        if (cnf_format) {
            formula = Parser.CNF.parse_formula(formula_str);
        } else {
            formula = Parser.DIMACS.parse_formula(formula_str.split("\n"));
        }
    }
    
    if (formula == null) {
        stderr.printf("Error\n");
        return 1;
    }
    
    bool satisfiable = formula.dpll();
    if (!satisfiable) {
        stdout.printf("Formula is not satisfiable\n");
        return 0;
    } else {
        stdout.printf(formula.get_context().get_solution().to_string() +"\n");
        return 0;
    }
    
}
