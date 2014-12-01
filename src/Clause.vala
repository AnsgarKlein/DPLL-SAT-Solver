
public enum ClauseStatus {
    TRUE,
    FALSE,
    UNDECIDED
}

public class Clause {
    private GLib.List<Literal> literals;
    private bool solved;
    
    public Clause(owned GLib.List<Literal> literals) {
        this.literals = (owned)literals;
        this.solved = false;
    }
    
    /**
     * Copy this object.
     * Note: Contained Literals don't get copied!
    **/
    public Clause clone() {
        GLib.List<Literal> cloned_list = new GLib.List<Literal>();
        foreach (Literal l in literals) {
            cloned_list.append(l);
        }
        
        return new Clause((owned)cloned_list);
    }
    
    /**
     * Represent this Clause as a string.
    **/
    public string to_string() {
        GLib.StringBuilder builder = new GLib.StringBuilder();
        builder.append_c(Constants.CLAUSE_START);
        
        int i = 0;
        foreach (Literal lit in literals) {
            builder.append(lit.to_string());
            if (i != literals.length() - 1) {
                builder.append_c(Constants.LITERAL_DELIMITER);
            }
            
            i++;
        }
        
        builder.append_c(Constants.CLAUSE_END);
        return builder.str;
    }
    
    /**
     * Returns whether this clause does only contain one literal
     * (One-Literal-Clause).
    **/
    public bool is_OneLiteralClause() {
        if (literals.length() == 1) {
            return true;
        } else {
            return false;
        }
    }
    
    public Literal? get_first_literal() {
        return literals.first().data;
    }
    
    /**
     * Returns array of all Literals that occur in this Clause.
    **/
    public Literal[] get_all_literals() {
        Literal[] lits = new Literal[literals.length()];
        
        int i = 0;
        foreach (Literal lit in literals) {
            lits[i] = lit;
            i++;
        }
        
        return lits;
    }
    
    /**
     * Evaluates this Clause according to logical rules by applying assignments
     * from given PartialAssignment.
     * 
     * This function will modify the content of this Clause!
     * (Remove false Literals)
     * 
     * Returns the status of this Clause when given PartialAssignment
     * is applied.
    **/
    public ClauseStatus evaluate(PartialAssignment pa) {
        GLib.List<Literal> false_literals = new GLib.List<Literal>();
        
        foreach (Literal lit in literals) {
            if (!pa.has_assignment(lit)) {
                continue;
            }
            
            bool assignment = pa.get_assignment(lit);
            
            // If any Literal is true this Clause is true.
            if (assignment == !lit.is_negated()) {
                return ClauseStatus.TRUE;
            }
            
            // If a Literal is false:
            if (assignment == lit.is_negated()) {
                // If there is only one Literal left in this Clause and it
                // is false this Clause is false.
                if (literals.length() == 1) {
                    return ClauseStatus.FALSE;
                }
                
                // If a Literal is false but is not the only Literal in this
                // Clause we will remove the Literal from this Clause.
                false_literals.append(lit);
            }
        }
        
        foreach (Literal lit in false_literals) {
            literals.remove(lit);
        }
        
        return ClauseStatus.UNDECIDED;
    }
}
