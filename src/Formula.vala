
public class Formula {
    private Gee.HashSet<Clause> clauses;
    private FormulaContext context;
    
    public Formula(owned Gee.HashSet<Clause> clauses, FormulaContext context) {
        this.clauses = (owned)clauses;
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
        foreach (Clause clause in clauses) {
            builder.append(clause.to_string());
            if (i != clauses.size - 1) {
                builder.append_c(Constants.CLAUSE_DELIMITER);
            }
            
            i++;
        }
        
        builder.append_c(Constants.FORMULA_END);
        return builder.str;
    }
    
    private Literal[]? get_next_literals(PartialAssignment pa, out bool[] best_assignments) {
        // Create an array of all literals (set and unset)
        // (if it doesn't exist already)
        Literal[] all_literals = context.get_all_literals();
        
        if (all_literals == null) {
            // Create HashSet of Literals
            Gee.HashSet<Literal> hash_set;
            hash_set = new Gee.HashSet<Literal>(
                (a) => {
                    if (a == null) {
                        return 0;
                    }
                    
                    if (!(a is Literal)) {
                        return 0;
                    }
                    
                    return ((Literal)a).hash();
                },
                (a, b) => {
                    if (a == null || b == null) {
                        return false;
                    }
                    
                    if (!(a is Literal)) {
                        return false;
                    }
                    if (!(a is Literal)) {
                        return false;
                    }
                    
                    return ((Literal)a).equals((Literal)b);
                }
            );
            
            foreach (Clause cl in clauses) {
                foreach (Literal lit in cl.get_all_literals()) {
                    if (hash_set.contains(lit)) {
                        continue;
                    } else {
                        hash_set.add(lit);
                    }
                }
            }
            
            Literal[] literals = hash_set.to_array();
            
            // Save array of Literals, so we don't have to create it again.
            context.set_all_literals(literals);
            all_literals = literals;
        }
        
        // If One-Literal-Clauses exists return all its Literals
        Gee.ArrayList<Literal> literals_to_set = new Gee.ArrayList<Literal>();
        Gee.ArrayList<bool> value_to_set = new Gee.ArrayList<bool>();
        
        foreach (Clause cl in clauses) {
            if (cl.is_OneLiteralClause()) {
                #if VERBOSE_DPLL
                    stdout.printf("  Unassigned literal from One-Literal-Clause: %s\n", cl.get_first_literal().get_name());
                #endif
                
                // If this Literal appears Negated in its One-Literal-Clause
                // we should assign it false and not true.
                Literal l = cl.get_first_literal();
                literals_to_set.add(l);
                
                if (l.is_negated()) {
                    value_to_set.add(false);
                } else {
                    value_to_set.add(true);
                }
            }
        }
        
        if (literals_to_set.size != 0) {
            best_assignments = value_to_set.to_array();
            return literals_to_set.to_array();
        }
        
        #if VERBOSE_DPLL
        {
            // Create a list of Literals that are unset in this PartialAssignment
            List<Literal> available_literals = new List<Literal>();
            foreach (Literal literal in all_literals) {
                if (!pa.has_assignment(literal)) {
                    available_literals.append(literal);
                }
            }
            
            // Print all literals (set and unset)
            {
                string literals_str = "All literals: (";
                
                for (int i = 0; i < all_literals.length; i++) {
                    literals_str += all_literals[i].get_name();
                    if (i != all_literals.length -1) {
                        literals_str += ",";
                    }
                }
                
                stdout.printf("  %s)\n", literals_str);
            }
            
            // Print all available Literals
            {
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
        }
        #endif
        
        // Otherwise just return the first available (unset) Literal
        foreach (Literal literal in all_literals) {
            if (!pa.has_assignment(literal)) {
                #if VERBOSE_DPLL
                    stdout.printf("  Picked unassigned literal: %s\n",
                                  available_literals.first().data.get_name());
                #endif
                
                best_assignments = { true };
                return { literal };
            }
        }
        
        // No literal left
        best_assignments = { };
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
        
        GLib.List<Clause> true_clauses = new GLib.List<Clause>();
        foreach (Clause clause in clauses) {
            switch (clause.evaluate(pa)) {
                case ClauseStatus.TRUE:
                    #if VERBOSE_DPLL
                        stdout.printf("  Clause %s is true, removing ...\n", clause.to_string());
                    #endif
                    true_clauses.append(clause);
                    break;
                case ClauseStatus.FALSE:
                    #if VERBOSE_DPLL
                        stdout.printf("  Clause %s is false, going back ...\n", clause.to_string());
                    #endif
                    return false;
                case ClauseStatus.UNDECIDED:
                    break;
            }
        }
        foreach (Clause clause in true_clauses) {
            clauses.remove(clause);
        }
        
        #if VERBOSE_DPLL
            stdout.printf("Formula:\t%s\n", this.to_string());
            stdout.printf("Assignments:\t%s\n", pa.to_string());
        #endif
        
        // Check if current assignment made formula true
        if (clauses.size == 0) {
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
        bool[] assignments;
        Literal[] unassigned_literals = get_next_literals(pa, out assignments);
        
        // If we don't find one it means every Literal has a value
        // in the current assignment, but the it doesn't make the Formula
        // true.
        // So we return false and try another assignment (or fail).
        if (unassigned_literals == null) {
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
            Gee.HashSet<Clause> saved_clauses = new Gee.HashSet<Clause>(
                null,
                null
            );
            foreach (Clause cl in clauses) {
                saved_clauses.add(cl.clone());
            }
            
            // Set selected Literal to selected assignment
            for (int p = 0; p < unassigned_literals.length; p++) {
                pa.assign(unassigned_literals[p], assignments[p]);
            }
            
            if (dpll_rec(pa)) {
                return true;
            }
            
            // If assignment wasn't correct restore state of Clauses from
            // before, set the Literal to the non-preferred assignment
            // and rerun.
            clauses = (owned)saved_clauses;
            
            for (int p = 0; p < assignments.length; p++) {
                assignments[p] = !assignments[p];
            }
        }
        
        
        // If neither makes the Formula true, we will restore the state
        // of assignment from before false and return false.
        //
        // The algorithm will rerun with another variable on an upper level.
        // (or it will just return false if the Formula is unsatisfiable)
        
        foreach (Literal unassigned_literal in unassigned_literals) {
            pa.unassign(unassigned_literal);
        }
        
        return false;
    }
}
