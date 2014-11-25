
public static void main(string[] args) {
    string[] sats = {
        "{ A,B}, {-A, -B},{ -C }, {lit, D}",
        "{A}, {-A}, {C}, {literalname}, {C}",
        "{q, -p, -r, s}, {-q, -r, s}, {r}, {-p, -s}, {-p, r}",
        "{-p, q, -r, s}, {-q, -r, s}, {r}, {-p, -s}, {-p, r}",
        "{A,B,C}, {-A,C,D}, {-A,C,-D}, {-A, -C, D}, {-A, -C, -D}, {A, B, -C}, {A, -B, C}",
        "{s, -q}, {-p, q, s}, {p}, {r, -s}, {-p, -r, -s}",
        "{-q, s}, {-p, q, s}, {p}, {r, -s}, {-p, -r, -s}"
    };
    
    foreach (string sat in sats) {
        do_dpll(sat, true);
    }
    
    {
        double seconds = 0;
        Timer timer = new Timer();
        
        for (int i = 0; i < 1000; i++) {
            foreach (string sat in sats) {
                Formula formula = CNFParser.parse_CNF_formula(sat);
                formula.dpll();
            }
        }
        
        timer.stop();
        seconds = timer.elapsed();
        stdout.printf("took %ss\n", seconds.to_string());
    }
    
    

}

public static void do_dpll(string cnf, bool print_formula) {
    Formula formula = CNFParser.parse_CNF_formula(cnf);
    bool satisfiable = formula.dpll();
    
    string str = "";
    
    if (print_formula) {
        str += formula.to_string();
        str += "\n";
    } else {
        str += "<formula>\n";
    }
    
    if (satisfiable) {
        str += "is satfisfiable\n";
        str += "Satisfying interpretation:\n";
        str += "  %s\n".printf(formula.get_context().get_solution().to_string());
    } else {
        str += "is not satfisfiable\n";
    }
    
    stdout.printf(str +"\n");
}
