
public class Clause {
    private GLib.List<Literal> literals;
    private bool solved;
    
    public Clause(GLib.List<Literal> literals) {
        this.literals = new GLib.List<Literal>();
        foreach (Literal lit in literals) {
            this.literals.append(lit);
        }
        
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
        
        return new Clause(cloned_list);
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
     * Returns whether this clause is equivalent to true.
    **/
    public bool is_true() {
        return solved;
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
     * Returns whether a given Literal appears in this Clause.
    **/
    public bool contains_literal(Literal literal) {
        foreach (Literal l in literals) {
            if (l.equals(literal)) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * Evaluates this Clause according to logical rules by applying assignments
     * from given PartialAssignment.
     * 
     * Returns null if this clause is either true or false.
     * (Which one can be determined by calling is_true())
     * 
     * If this clause is neither true nor false it returns a simplified
     * version of itself.
    **/
    public Clause? evaluate(PartialAssignment pa) {
        GLib.List<Literal> false_literals = new GLib.List<Literal>();
        
        foreach (Literal lit in literals) {
            if (!pa.has_assignment(lit)) {
                continue;
            }
            
            bool assignment = pa.get_assignment(lit);
            
            // If any Literal is true, we will return null to
            // signal that this clause is true.
            if (assignment == !lit.is_negated()) {
                this.solved = true;
                return null;
            }
            
            // If a Literal is false:
            if (assignment == lit.is_negated()) {
                // If there is only one Literal left in this clause and it
                // is false, we will return null to signal that this clause
                // is false.
                if (literals.length() == 1) {
                    this.solved = false;
                    return null;
                }
                
                // If a Literal is false but is not the only Literal in this
                // CLause we will remove the Literal from this Clause.
                false_literals.append(lit);
            }
        }
        
        foreach (Literal lit in false_literals) {
            literals.remove(lit);
        }
        
        // If this Clause is still not true or false, we will
        // return the object itself.
        // (It now has less Literals than before)
        return this;
    }
}
