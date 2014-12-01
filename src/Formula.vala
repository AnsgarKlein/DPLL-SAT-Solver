
public class Formula {
    private GLib.List<IClause> clauses;
    private FormulaContext context;
    
    public Formula(GLib.List<IClause> clauses, FormulaContext context) {
        this.clauses = new GLib.List<IClause>();
        foreach (IClause clause in clauses) {
            this.clauses.append(clause.clone());
        }
        
        this.context = context;
    }
    
    /**
     * Returns the context of this Formula.
    **/
    public FormulaContext get_context() {
        return context;
    }
    
    /**
     * Represent this Formula as a string.
    **/
    public string to_string() {
        GLib.StringBuilder builder = new GLib.StringBuilder();
        builder.append_c(Constants.FORMULA_START);
        
        int i = 0;
        foreach (IClause clause in clauses) {
            builder.append(clause.to_string());
            if (i != clauses.length() - 1) {
                builder.append_c(Constants.CLAUSE_DELIMITER);
            }
            
            i++;
        }
        
        builder.append_c(Constants.FORMULA_END);
        return builder.str;
    }
    
    /**
     * Returns the number of occurrences of Literal in this Formula.
     * 
     * Note: A Literal that appears multiple times in the same Clause is
     *       only counted as one !
    **/
    private int get_literal_occurrences(Literal literal) {
        int occurrences = 0;
        
        // We add 1 if the Literal appears in a clause even though it could
        // appear multiple times.
        foreach (IClause cl in clauses) {
            if (cl.contains_literal(literal)) {
                occurrences++;
            }
        }
        
        return occurrences;
    }
    
    private Literal? get_next_literal(PartialAssignment pa, ref bool best_assignment) {
        // Create an array of all literals (set and unset)
        // (if it doesn't exist already)
        Literal[] all_literals = context.get_all_literals();
        
        if (all_literals == null) {
            // Create HashSet of tuples (Literal, occurences).
            Gee.HashSet<LiteralOccurrenceTuple> tuple_set;
            tuple_set = new Gee.HashSet<LiteralOccurrenceTuple>(
                (a) => {
                    if (a == null) {
                        return 0;
                    }
                    
                    if (!(a is LiteralOccurrenceTuple)) {
                        return 0;
                    }
                    
                    return ((LiteralOccurrenceTuple)a).get_literal().hash();
                },
                (a, b) => {
                    if (a == null || b == null) {
                        return false;
                    }
                    
                    if (!(a is LiteralOccurrenceTuple)) {
                        return false;
                    }
                    if (!(a is LiteralOccurrenceTuple)) {
                        return false;
                    }
                    
                    Literal lit1 = ((LiteralOccurrenceTuple)a).get_literal();
                    Literal lit2 = ((LiteralOccurrenceTuple)b).get_literal();
                    
                    return lit1.equals(lit2);
                }
            );
            
            foreach (IClause cl in clauses) {
                foreach (Literal lit in cl.get_all_literals()) {
                    if (tuple_set.contains(new LiteralOccurrenceTuple(lit, 0))) {
                        continue;
                    } else {
                        int occurrences = get_literal_occurrences(lit);
                        tuple_set.add(new LiteralOccurrenceTuple(lit, occurrences));
                    }
                }
            }
            
            // Create array from HashSet
            LiteralOccurrenceTuple[] tuple_array = tuple_set.to_array();
            
            // Sort array of tuples by occurrences of Literal
            // using (bad) BubbleSort (but this doesn't really matter).
            {
                int n = tuple_array.length;
                bool changed = false;
                do {
                    changed = false;
                    for (int i = 0; i < n - 1; i++) {
                        if (LiteralOccurrenceTuple.compare(tuple_array[i], tuple_array[i+1]) > 0) {
                            LiteralOccurrenceTuple tmp = tuple_array[i];
                            tuple_array[i] = tuple_array[i+1];
                            tuple_array[i+1] = tmp;
                            
                            changed = true;
                        }
                    }
                } while (changed);
            }
            
            // Create Literal[] from LiteralOccurrenceTuple[]
            Literal[] literals;
            {
                int length = tuple_array.length;
                literals = new Literal[length];
                for (int i = 0; i < length; i++) {
                    literals[i] = tuple_array[i].get_literal();
                }
            }
            
            // Save sorted array of Literals, so we don't have to
            // create it again.
            context.set_all_literals(literals);
            all_literals = literals;
        }
        
        // If a One-Literal-Clause exists return its Literal
        foreach (IClause cl in clauses) {
            if (cl.is_OneLiteralClause()) {
                #if VERBOSE_DPLL
                    stdout.printf("  Unassigned literal from One-Literal-Clause: %s\n", cl.get_first_literal().get_name());
                #endif
                
                // If this Literal appears Negated in its One-Literal-Clause
                // we should assign it false and not true.
                Literal l = cl.get_first_literal();
                if (l.is_negated()) {
                    best_assignment = false;
                }
                
                return l;
            }
        }
        
        // Create a list of Literals that are unset in this PartialAssignment
        List<Literal> available_literals = new List<Literal>();
        foreach (Literal literal in all_literals) {
            if (!pa.has_assignment(literal)) {
                available_literals.append(literal);
            }
        }
        
        #if VERBOSE_DPLL
        {
            // Print all literals (set and unset)
            string literals_str = "All literals: (";
            
            for (int i = 0; i < all_literals.length; i++) {
                literals_str += all_literals[i].get_name();
                if (i != all_literals.length -1) {
                    literals_str += ",";
                }
            }
            
            stdout.printf("  %s)\n", literals_str);
        }
        #endif
        
        #if VERBOSE_DPLL
        {
            // Print all available Literals
            string literals_str = "Available literals: (";
            
            int i = 0;
            foreach (Literal lit in available_literals) {
                literals_str += lit.get_name();
                if (i != available_literals.length() - 1) {
                    literals_str += ",";
                }
                
                i++;
            }
            
            stdout.printf("  %s)\n", literals_str);
        }
        #endif
        
        // Otherwise just return the first available (unset) Literal
        if (available_literals.length() != 0) {
            #if VERBOSE_DPLL
                stdout.printf("  Picked unassigned literal: %s\n",
                              available_literals.first().data.get_name());
            #endif
            
            return available_literals.first().data;
        }
        
        // No literal left
        return null;
    }
    
    /**
     * Returns true if Formula is satisfiable, false otherwise.
    **/
    public bool dpll() {
        PartialAssignment pa = new PartialAssignment();
        return dpll_rec(pa);
    }
    
    /**
     * Returns true if Formula is satisfiable, false otherwise.
    **/
    private bool dpll_rec(PartialAssignment pa) {
        #if VERBOSE_DPLL
            stdout.printf("\n\n\n");
            stdout.printf("Formula:\t%s\n", this.to_string());
            stdout.printf("Assignments:\t%s\n", pa.to_string());
        #endif
        
        // Evaluate current assignment
        #if VERBOSE_DPLL
            stdout.printf("  evaluating ...\n");
        #endif
        
        GLib.List<IClause> simplified_clauses = new GLib.List<IClause>();
        foreach (IClause clause in clauses) {
            IClause new_clause = clause.evaluate(pa);
            
            switch (new_clause.get_status()) {
                case ClauseStatus.TRUE:
                    #if VERBOSE_DPLL
                        stdout.printf("  Clause %s is true, removing ...\n", clause.to_string());
                    #endif
                    break;
                case ClauseStatus.FALSE:
                    #if VERBOSE_DPLL
                        stdout.printf("  Clause %s is false, going back ...\n", clause.to_string());
                    #endif
                    return false;
                case ClauseStatus.UNDECIDED:
                    simplified_clauses.prepend(new_clause);
                    break;
            }
        }
        this.clauses = (owned)simplified_clauses;
        
        #if VERBOSE_DPLL
            stdout.printf("Formula:\t%s\n", this.to_string());
            stdout.printf("Assignments:\t%s\n", pa.to_string());
        #endif
        
        // Check if current assignment made formula true
        if (clauses.length() == 0) {
            #if VERBOSE_DPLL
                stdout.printf("  0 Clauses -> formula satisfied\n");
            #endif
            
            context.set_solution(pa);
            return true;
        }
        
        // Try to find another another Literal which hasn't been
        // assigned a value.
        //
        // Normally we first try to assign true to Literals, but sometimes
        // (a negated Literal in a One-Literal-Clause) it's smarter to
        // assign false first.
        bool assignment = true;
        Literal unassigned_lit = get_next_literal(pa, ref assignment);
        
        // If we don't find one it means every Literal has a value
        // in the current assignment, but the it doesn't make the Formula
        // true.
        // So we return false and try another assignment (or fail).
        if (unassigned_lit == null) {
            #if VERBOSE_DPLL
                stdout.printf("  Found no unassigned literal\n");
            #endif
            return false;
        }
        
        // Set the found Literal to the preferred assignment and rerun
        // the algorithm.
        for (int i = 0; i < 2; i++) {
            #if VERBOSE_DPLL
                stdout.printf("  Trying %s=%s ...",
                              unassigned_lit.get_name(),
                              assignment ? "true" : "false");
            #endif
            
            // Save state of Clauses
            GLib.List<IClause> saved_clauses = new GLib.List<IClause>();
            foreach (IClause cl in clauses) {
                saved_clauses.prepend(cl.clone());
            }
            
            // Set selected Literal to selected assignment
            pa.assign(unassigned_lit, assignment);
            if (dpll_rec(pa)) {
                return true;
            }
            
            // If assignment wasn't correct restore state of Clauses from
            // before, set the Literal to the non-preferred assignment
            // and rerun.
            clauses = (owned)saved_clauses;
            
            assignment = !assignment;
        }
        
        
        // If neither makes the Formula true, we will restore the state
        // of assignment from before false and return false.
        //
        // The algorithm will rerun with another variable on an upper level.
        // (or it will just return false if the Formula is unsatisfiable)
        pa.unassign(unassigned_lit);
        return false;
    }
}
