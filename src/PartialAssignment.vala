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


public class PartialAssignment {
    Gee.HashMap<Literal, bool> assignments;
    
    public PartialAssignment() {
        assignments = new Gee.HashMap<Literal, bool>(
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
                
                if (!(a is Literal) || !(b is Literal)) {
                    return false;
                }
                
                Literal lit1 = (Literal)a;
                Literal lit2 = (Literal)b;
                
                return lit1.equals(lit2);
            },
            (a, b) => {
                if (a == null || b == null) {
                    return false;
                }
                
                if (!(a is bool) || !(b is bool)) {
                    return false;
                }
                
                return (bool)a == (bool)b;
            }
        );
    }
    
    /**
     * Assign given Literal given boolean value.
    **/
    public new void assign(Literal literal, bool boolean) {
        assignments.set(literal, boolean);
    }
    
    /**
     * Remove assignment for given Literal.
    **/
    public new void unassign(Literal literal) {
        assignments.unset(literal);
    }
    
    /**
     * Return boolean assignment for given Literal.
    **/
    public new bool get_assignment(Literal literal) {
        return assignments.get(literal);
    }
    
    /**
     * Returns whether given Literal has an assigned boolean value.
    **/
    public bool has_assignment(Literal literal) {
        return assignments.has_key(literal);
    }
    
    /**
     * Represent this PartialAssignment as a string.
    **/
    public string to_string() {
        string str = "";
        
        Gee.MapIterator<Literal, bool> iterator = assignments.map_iterator();
        Literal hash_key;
        bool hash_value;
        while (iterator.next() != false) {
            hash_key = (Literal)iterator.get_key();
            hash_value = (bool)iterator.get_value();
            
            str += "%s=%s ".printf(hash_key.get_name(), hash_value ? "true" : "false");
        }
        
        return str;
    }
}
