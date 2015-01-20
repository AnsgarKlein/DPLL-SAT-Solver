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


/**
 * Represents a formula of propositional logic in conjunctive normal form.
 * 
 * A Formula contains Clauses, which contain Literals.
 * For a Formula to be true all contained Clauses have to be true.
**/
public class Formula {
    private Gee.HashSet<Clause> clauses;
    private GenericLiteral[] all_literals;
    
    public Formula(owned Gee.HashSet<Clause> clauses, GenericLiteral[] all_literals) {
        this.clauses = (owned)clauses;
        
        // Sort array of all Literals
        {
            GLib.CompareDataFunc cmp = (a, b) => {
                int ai = ((GenericLiteral)a).get_occurrences();
                int bi = ((GenericLiteral)b).get_occurrences();
                
                return (int)(ai < bi) - (int)(ai > bi);
            };
            
            int n = all_literals.length;
            bool changed = false;
            do {
                changed = false;
                for (int i = 0; i < n - 1; i++) {
                    if (cmp(all_literals[i], all_literals[i + 1]) > 0) {
                        GenericLiteral tmp = all_literals[i];
                        all_literals[i] = all_literals[i + 1];
                        all_literals[i + 1] = tmp;
                        
                        changed = true;
                    }
                }
            } while (changed);
        }
        
        this.all_literals = all_literals;
    }
    
    /**
     * Represent this Formula as a string.
    **/
    public string to_string(bool color) {
        GLib.StringBuilder builder = new GLib.StringBuilder();
        builder.append_c(Constants.FORMULA_START);
        
        int i = 0;
        foreach (Clause clause in clauses) {
            builder.append(clause.to_string(color));
            if (i != clauses.size - 1) {
                builder.append_c(Constants.CLAUSE_DELIMITER);
            }
            
            i++;
        }
        
        builder.append_c(Constants.FORMULA_END);
        return builder.str;
    }
    
    /**
     * Represent current assignments of Literals as a string.
     * 
     * If print_all is false Literals that are not set will not be included
     * in the string.
    **/
    public string to_assignment_string(bool color, bool print_all) {
        string str = "";
        
        foreach (GenericLiteral literal in all_literals) {
            if (print_all || literal.get_assignment() != LiteralAssignment.UNSET) {
                str += literal.to_assignment_string(color);
                str += " ";
            }
        }
        
        return str;
    }
    
    /**
     * Select Literal(s) (that are currently unassigned) from
     * One-Literal-Clauses to assign a value to.
     * 
     * best_assignments contains assignments for the corresponding Literals,
     * which will most likely satisfy the Formula.
     * 
     * Returns null if no unassigned Literals from One-Literal-Clauses are left.
    **/
    private GenericLiteral[]? unit_propagate(out bool[] best_assignments) {
        // If One-Literal-Clauses exists return their Literals,
        // with the appropriate assignments.
        
        Gee.ArrayList<GenericLiteral> propagated_literals;
        propagated_literals = new Gee.ArrayList<GenericLiteral>();
        Gee.ArrayList<bool> value_to_set = new Gee.ArrayList<bool>();
        
        foreach (Clause cl in clauses) {
            Literal? only_literal = cl.is_unit_clause();
            if (only_literal != null) {
                #if VERBOSE_DPLL
                    stdout.printf("  Unassigned literal from One-Literal-Clause: %s\n", only_literal.get_literal().get_name());
                #endif
                
                propagated_literals.add(only_literal.get_literal());
                
                // If this Literal appears Negated in its One-Literal-Clause
                // we should assign it false and not true.
                if (only_literal.is_negated()) {
                    value_to_set.add(false);
                } else {
                    value_to_set.add(true);
                }
            }
        }
        
        if (propagated_literals.size != 0) {
            best_assignments = value_to_set.to_array();
            return propagated_literals.to_array();
        }
        
        best_assignments = { };
        return null;
    }
    
    /**
     * Select next Literal (that is currently unassigned) to assign a
     * value to.
     * 
     * best_assignment contains assignment for the corresponding Literal,
     * which will most likely satisfy the Formula.
     * 
     * Returns null if no unassigned Literals are left.
    **/
    private GenericLiteral[]? choose_literal(out bool best_assignment) {
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
                foreach (GenericLiteral lit in available_literals) {
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
                
                best_assignment = true;
                return { literal };
            }
        }
        
        // No literal left
        best_assignment = true;
        return null;
    }
    
    /**
     * Returns true if Formula is satisfiable, false otherwise.
    **/
    public bool dpll() {
        #if VERBOSE_DPLL
            stdout.printf("\n\n\n");
            stdout.printf("Formula:\t%s\n", this.to_string(Constants.COLOR_ENABLED));
            stdout.printf("Assignments:\t%s\n", to_assignment_string(Constants.COLOR_ENABLED, false));
        #endif
        
        {
            // Evaluate current assignment
            #if VERBOSE_DPLL
                stdout.printf("  evaluating ...\n");
            #endif
            
            GLib.List<Clause> true_clauses = new GLib.List<Clause>();
            foreach (Clause clause in clauses) {
                switch (clause.evaluate()) {
                    case ClauseStatus.TRUE:
                        #if VERBOSE_DPLL
                            stdout.printf("  Clause %s is true, removing ...\n",
                                          clause.to_string(Constants.COLOR_ENABLED));
                        #endif
                        true_clauses.append(clause);
                        break;
                    case ClauseStatus.FALSE:
                        #if VERBOSE_DPLL
                            stdout.printf("  Clause %s is false, going back ...\n",
                                          clause.to_string(Constants.COLOR_ENABLED));
                        #endif
                        return false;
                    case ClauseStatus.UNDECIDED:
                        break;
                }
            }
            foreach (Clause clause in true_clauses) {
                clauses.remove(clause);
            }
        }
        
        #if VERBOSE_DPLL
            stdout.printf("Formula:\t%s\n", this.to_string(Constants.COLOR_ENABLED));
            stdout.printf("Assignments:\t%s\n", to_assignment_string(Constants.COLOR_ENABLED, false));
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
        GenericLiteral[]? unassigned_literals = unit_propagate(out assignments);
        
        if (unassigned_literals == null) {
            bool assignment;
            unassigned_literals = choose_literal(out assignment);
            assignments = { assignment };
        }
        
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
            
            #if VERBOSE_DPLL
                for (int p = 0; p < unassigned_literals.length; p++) {
                    stdout.printf("  Trying %s\n",
                                  unassigned_literals[p].to_assignment_string(Constants.COLOR_ENABLED));
                }
            #endif
            
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
