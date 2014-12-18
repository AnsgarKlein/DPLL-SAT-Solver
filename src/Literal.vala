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
 * Represents the possible assignments of a Literal.
 * 
 * TRUE - Literal is true
 * FALSE - Literal is false
 * UNSET - Literal has no assignment
**/
public enum LiteralAssignment {
    TRUE,
    FALSE,
    UNSET
}

/**
 * Wrapper class for GenericLiteral, which adds the possibility of the
 * contained GenericLiteral being negated.
**/
public class Literal {
    private GenericLiteral literal;
    private bool negated;
    
    public Literal(GenericLiteral literal, bool negated) {
        this.literal = literal;
        this.negated = negated;
    }
    
    /**
     * Returns the GenericLiteral this object is wrapping.
    **/
    public GenericLiteral get_literal() {
        return literal;
    }
    
    /**
     * Returns whether the Literal appears negated (~A) or not (A).
    **/
    public bool is_negated() {
        return negated;
    }
    
    /**
     * Represent this Literal as a string.
    **/
    public string to_string() {
        if (negated) {
            return "%c%s".printf(Constants.NEGATE_CHAR, literal.to_string());
        } else {
            return literal.to_string();
        }
    }
}

/**
 * Represents a Literal that can have an assignment.
**/
public class GenericLiteral {
    private string name;
    private LiteralAssignment assignment;
    private int occurrences;
    
    public GenericLiteral(string name) {
        this.name = name;
        this.assignment = LiteralAssignment.UNSET;
        this.occurrences = 0;
    }
    
    /**
     * Increases the number of Clauses this Literal occurs in by one.
    **/
    public void increase_occurrences() {
        occurrences++;
    }
    
    /**
     * Returns the number of Clauses this Literal occurs in.
    **/
    public int get_occurrences() {
        return occurrences;
    }
    
    /**
     * Returns the name of this Literal.
    **/
    public string get_name() {
        return name;
    }
    
    /**
     * Assign this Literal the given boolean value.
    **/
    public void assign(bool b) {
        assignment = b ? LiteralAssignment.TRUE : LiteralAssignment.FALSE;
    }
    
    /**
     * Remove assignment for this Literal.
    **/
    public void unassign() {
        assignment = LiteralAssignment.UNSET;
    }
    
    /**
     * Return assignment for this Literal.
    **/
    public LiteralAssignment get_assignment() {
        return assignment;
    }
    
    /**
     * Represent this Literal as a string.
    **/
    public string to_string() {
        return name;
    }
    
    /**
     * Returns whether a Literal is equal to this Literal.
     * 
     * Note: Two Literals are equal, if their names are equal.
     *       It does NOT matter whether they have the same assignment or not!
    **/
    public bool equals(GenericLiteral lit) {
        if (lit == this) {
            return true;
        }
        
        if (this.name == lit.get_name()) {
            return true;
        }
        
        return false;
    }
    
    /**
     * Returns an unambiguous hash for this Literal.
     * 
     * Note: Two Literals are equal (share the same hash), if their names
     *       are equal.
     *       It does NOT matter whether they have the same assignment or not!
    **/
    public uint hash() {
        return name.hash();
    }
}
