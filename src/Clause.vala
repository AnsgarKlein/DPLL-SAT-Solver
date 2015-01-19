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
 * Represents the possible status of a Clause.
 * 
 * TRUE - Clause is satisfied
 * FALSE - Clause is unsatisfied
 * UNDECIDED - Clause is not yet satisfied or unsatisfied
**/
public enum ClauseStatus {
    TRUE,
    FALSE,
    UNDECIDED
}

/**
 * Represents a Clause of propositional logic in conjunctive normal form.
 * 
 * A Clause contains Literals.
 * For a Clause to be true at least one contained Literal has to be true.
**/
public class Clause {
    private Literal[] literals;
    private ClauseStatus clause_status;
    
    public Clause(Literal[] literals) {
        this.literals = literals;
        this.clause_status = ClauseStatus.UNDECIDED;
    }
    
    /**
     * Copy this object.
     * Note: Contained Literals don't get copied!
    **/
    public Clause clone() {
        Literal[] cloned_literals = new Literal[literals.length];
        for (int i = 0; i < literals.length; i++) {
            cloned_literals[i] = literals[i];
        }
        
        return new Clause(cloned_literals);
    }
    
    /**
     * Represent this Clause as a string.
    **/
    public string to_string(bool color) {
        GLib.StringBuilder builder = new GLib.StringBuilder();
        
        // Add symbol indicating the start of the clause
        if (clause_status == ClauseStatus.TRUE && color) {
            builder.append(Constants.COLOR_PREFIX_TRUE);
        } else if (clause_status == ClauseStatus.FALSE && color) {
            builder.append(Constants.COLOR_PREFIX_FALSE);
        }
        
        builder.append_c(Constants.CLAUSE_START);
        
        if (clause_status != ClauseStatus.UNDECIDED && color) {
            builder.append(Constants.COLOR_SUFFIX);
        }
        
        // Add content of clause
        {
            int i = 0;
            foreach (Literal lit in literals) {
                builder.append(lit.to_string(color));
                if (i != literals.length - 1) {
                    builder.append_c(Constants.LITERAL_DELIMITER);
                }
                
                i++;
            }
        }
        
        // Add symbol indicating the end of the clause
        if (clause_status == ClauseStatus.TRUE && color) {
            builder.append(Constants.COLOR_PREFIX_TRUE);
        } else if (clause_status == ClauseStatus.FALSE && color) {
            builder.append(Constants.COLOR_PREFIX_FALSE);
        }
        
        builder.append_c(Constants.CLAUSE_END);
        
        if (clause_status != ClauseStatus.UNDECIDED && color) {
            builder.append(Constants.COLOR_SUFFIX);
        }
        
        
        
        
        
        return builder.str;
    }
    
    /**
     * Decides wheter this clause is a unit clause (only contains one single
     * unassigned Literal).
     * 
     * If this clause is a unit clause its only unassigned Literal is returned.
     * If this clause is not a unit clause null is returned.
    **/
    public Literal? is_unit_clause() {
        // If all but one Literal are assigned return true.
        // If there are more than one unassigned Literal return false.
        bool found_unassigned_literal = false;
        Literal? only_literal = null;
        
        foreach (Literal literal in literals) {
            if (literal.get_literal().get_assignment() == LiteralAssignment.UNSET) {
                // If we found an unassigned Literal previously there are
                // multiple unassigned Literals in this Clause.
                if (found_unassigned_literal) {
                    return null;
                }
                
                found_unassigned_literal = true;
                only_literal = literal;
            }
        }
        
        return only_literal;
    }
    
    /**
     * Evaluates this Clause according to logical rules.
     * 
     * Returns the status of this Clause after evaluating all its Literals.
    **/
    public ClauseStatus evaluate() {
        // Check all Literals in this Clause
        foreach (Literal lit in literals) {
            bool assignment = false;
            
            switch (lit.get_literal().get_assignment()) {
            case LiteralAssignment.UNSET:
                continue;
            case LiteralAssignment.TRUE:
                assignment = true;
                break;
            case LiteralAssignment.FALSE:
                assignment = false;
                break;
            }
            
            // If any Literal is true this Clause is true.
            if (assignment == !lit.is_negated()) {
                this.clause_status = ClauseStatus.TRUE;
                return this.clause_status;
            }
            
            // If a Literal is false:
            if (assignment == lit.is_negated()) {
                // If a Literal is false but there are other Literals in this
                // Clause that are true or not set everything is fine.
                bool all_literals_false = true;
                foreach (Literal l in literals) {
                    LiteralAssignment assign = l.get_literal().get_assignment();
                    
                    if (assign == LiteralAssignment.UNSET ||
                        assign == LiteralAssignment.FALSE && l.is_negated() ||
                        assign == LiteralAssignment.TRUE && !l.is_negated()) {
                        
                        all_literals_false = false;
                        break;
                    }
                }
                
                // If there is only one Literal left in this Clause and it
                // is false this Clause is false.
                if (all_literals_false) {
                    this.clause_status = ClauseStatus.FALSE;
                    return this.clause_status;
                }
            }
        }
        
        // If this Clause is neither true nor false it is undecided.
        this.clause_status = ClauseStatus.UNDECIDED;
        return this.clause_status;
    }
    
    public ClauseStatus get_status() {
        return clause_status;
    }
}
