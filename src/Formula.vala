/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/


public class Formula {
    private Gee.HashSet<Clause> clauses;
    private GenericLiteral[] all_literals;
    
    public Formula(owned Gee.HashSet<Clause> clauses, GenericLiteral[] all_literals) {
        this.clauses = (owned)clauses;
        this.all_literals = all_literals;
    }
    
    
    public string get_solution() {
        string str = "";
        
        foreach (GenericLiteral literal in all_literals) {
            string name = literal.get_name();
            
            switch (literal.get_assignment()) {
                case LiteralAssignment.TRUE:
                    str += "%s=true ".printf(name);
                    break;
                case LiteralAssignment.FALSE:
                    str += "%s=false ".printf(name);
                    break;
                case LiteralAssignment.UNSET:
                    str += "%s=? ".printf(name);
                    break;
            }
        }
        
        return str;
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
    
    private GenericLiteral[]? get_next_literals(out bool[] best_assignments) {
        // If One-Literal-Clauses exists return all its Literals
        Gee.ArrayList<GenericLiteral> literals_to_set = new Gee.ArrayList<GenericLiteral>();
        Gee.ArrayList<bool> value_to_set = new Gee.ArrayList<bool>();
        
        foreach (Clause cl in clauses) {
            if (cl.is_OneLiteralClause()) {
                #if VERBOSE_DPLL
                    stdout.printf("  Unassigned literal from One-Literal-Clause: %s\n", cl.get_only_literal().get_literal().get_name());
                #endif
                
                // If this Literal appears Negated in its One-Literal-Clause
                // we should assign it false and not true.
                Literal l = cl.get_only_literal();
                literals_to_set.add(l.get_literal());
                
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
            List<GenericLiteral> available_literals = new List<GenericLiteral>();
            foreach (GenericLiteral literal in all_literals) {
                if (literal.get_assignment() == LiteralAssignment.UNSET) {
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
        foreach (GenericLiteral literal in all_literals) {
            if (literal.get_assignment() == LiteralAssignment.UNSET) {
                #if VERBOSE_DPLL
                    stdout.printf("  Picked unassigned literal: %s\n",
                                  literal.get_name());
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
        #if VERBOSE_DPLL
            stdout.printf("\n\n\n");
            stdout.printf("Formula:\t%s\n", this.to_string());
            stdout.printf("Assignments:\t%s\n", get_context().get_solution());
        #endif
        
        // Evaluate current assignment
        #if VERBOSE_DPLL
            stdout.printf("  evaluating ...\n");
        #endif
        
        GLib.List<Clause> true_clauses = new GLib.List<Clause>();
        foreach (Clause clause in clauses) {
            switch (clause.evaluate()) {
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
            stdout.printf("Assignments:\t%s\n", get_context().get_solution());
        #endif
        
        // Check if current assignment made formula true
        if (clauses.size == 0) {
            #if VERBOSE_DPLL
                stdout.printf("  0 Clauses -> formula satisfied\n");
            #endif
            
            return true;
        }
        
        // Try to find another another Literal which hasn't been
        // assigned a value.
        //
        // Normally we first try to assign true to Literals, but sometimes
        // (a negated Literal in a One-Literal-Clause) it's smarter to
        // assign false first.
        bool[] assignments;
        GenericLiteral[] unassigned_literals = get_next_literals(out assignments);
        
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
                for (int p = 0; p < unassigned_literals.length; p++) {
                    stdout.printf("  Trying %s=%s ...\n",
                        unassigned_literals[p].get_name(),
                        assignments[p] ? "true" : "false"
                    );
                }
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
                unassigned_literals[p].assign(assignments[p]);
            }
            
            if (dpll()) {
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
        foreach (GenericLiteral unassigned_literal in unassigned_literals) {
            unassigned_literal.unassign();
        }
        
        // The algorithm will rerun with another variable on an upper level.
        // (or it will just return false if the Formula is unsatisfiable)
        return false;
    }
}
