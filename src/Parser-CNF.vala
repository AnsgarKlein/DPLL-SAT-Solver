
namespace Parser {
    namespace CNF {
        public static unichar CLAUSE_START = '{';
        public static unichar CLAUSE_END = '}';
        public static unichar LITERAL_DELIMITER = ',';
        public static unichar NEGATE_CHAR = '-';
        
        private static Formula parse_formula(string str) {
            char[] formula = str.to_utf8();
            
            // Create List of clauses
            Gee.HashSet<Clause> clauses = new Gee.HashSet<Clause>(null, null);
            // Search for starting point of clause
            for (int i = 0; i < formula.length; i++) {
                if (formula[i] == CLAUSE_START) {
                    string clause_str = "";
                    
                    // Search end of clause
                    int p;
                    
                    for (p = i+1; p < formula.length; p++) {
                        if (formula[p] != CLAUSE_END) {
                            clause_str += formula[p].to_string();
                        } else {
                            break;
                        }
                    }
                    
                    // Create and add clause to list of clauses
                    GLib.List<Literal> literals = parse_clause(clause_str);
                    Clause new_clause = new Clause((owned)literals);
                    clauses.add(new_clause);
                    
                    i = p;
                }
            }
            
            // Create formula from list of clauses
            return new Formula((owned)clauses, new FormulaContext());
        }
        
        private static GLib.List<Literal> parse_clause(string str) {
            GLib.List<Literal> literals = new GLib.List<Literal>();
            
            char[] clause = str.to_utf8();
            string lit = "";
            for (int i = 0; i < clause.length; i++) {
                if (clause[i] != LITERAL_DELIMITER) {
                    lit += clause[i].to_string();
                } else {
                    literals.append(parse_literal(lit));
                    lit = "";
                }
            }
            literals.append(parse_literal(lit));
            
            return literals;
        }
        
        private static Literal parse_literal(string str) {
            string name = "";
            bool negated = false;
            
            char[] arr = str.to_utf8();
            for (int i = 0; i < arr.length; i++) {
                if (arr[i] == NEGATE_CHAR) {
                    negated = !negated;
                } else if (arr[i] == ' ') {
                    //skip
                } else {
                    name = str.substring(i, -1).strip();
                    break;
                }
            }
            
            return new Literal(name, negated);
        }
    }
}
