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


public class Literal {
    private bool negated;
    private string name;
    
    public Literal(string name, bool negated) {
        this.name = name;
        this.negated = negated;
    }
    
    /**
     * Returns the name of the Literal.
    **/
    public string get_name() {
        return name;
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
            return "%c%s".printf(Constants.NEGATE_CHAR, name);
        } else {
            return name;
        }
    }
    
    /**
     * Returns whether a Literal is equal to this Literal.
     * 
     * Note: Two Literals are equal, if their names are equal.
     *       It does NOT matter whether they are negated or not!
    **/
    public bool equals(Literal lit) {
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
     *       It does NOT matter whether they are negated or not!
    **/
    public uint hash() {
        return name.hash();
    }
}
