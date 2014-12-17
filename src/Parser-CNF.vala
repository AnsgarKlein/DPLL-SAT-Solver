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


namespace Parser {
    namespace CNF {
        public static unichar CLAUSE_START = '{';
        public static unichar CLAUSE_END = '}';
        public static unichar LITERAL_DELIMITER = ',';
        public static unichar NEGATE_CHAR = '-';
        
        private static Formula parse_formula(string str) {
            char[] formula = str.to_utf8();
            
            // Create List of ALL Literals
            Gee.HashSet<GenericLiteral> all_literals = new Gee.HashSet<GenericLiteral>(
                (a) => {
                    if (a == null) {
                        return 0;
                    }
                    
                    if (!(a is GenericLiteral)) {
                        return 0;
                    }
                    
                    return ((GenericLiteral)a).hash();
                },
                (a, b) => {
                    if (a == null || b == null) {
                        return false;
                    }
                    
                    if (!(a is GenericLiteral) || !(b is GenericLiteral)) {
                        return false;
                    }
                    
                    if (a == b) {
                        return true;
                    }
                    
                    return ((GenericLiteral)a).equals((GenericLiteral)b);
                }
            );
            
            // Create List of Clauses
            Gee.HashSet<Clause> clauses = new Gee.HashSet<Clause>(null, null);
            // Search for starting point of clause
            for (int i = 0; i < formula.length; i++) {
                if (formula[i] == CLAUSE_START) {
                    string clause_str = "";
                    
                    // Search end of Clause
                    int p;
                    
                    for (p = i+1; p < formula.length; p++) {
                        if (formula[p] != CLAUSE_END) {
                            clause_str += formula[p].to_string();
                        } else {
                            break;
                        }
                    }
                    
                    // Create and add Clause to list of Clauses
                    Literal[] literals = parse_clause(clause_str, all_literals);
                    Clause new_clause = new Clause(literals);
                    clauses.add(new_clause);
                    
                    i = p;
                }
            }
            
            // Create formula from list of clauses
            return new Formula((owned)clauses, all_literals.to_array());
        }
        
        private static Literal[] parse_clause(string str, Gee.HashSet<GenericLiteral> all_literals) {
            Gee.LinkedList<Literal> clause_literals = new Gee.LinkedList<Literal>();
            
            char[] clause = str.to_utf8();
            string lit = "";
            for (int i = 0; i < clause.length; i++) {
                if (clause[i] != LITERAL_DELIMITER) {
                    lit += clause[i].to_string();
                } else {
                    clause_literals.add(parse_literal(lit, all_literals));
                    lit = "";
                }
            }
            clause_literals.add(parse_literal(lit, all_literals));
            
            return clause_literals.to_array();
        }
        
        private static Literal parse_literal(string str, Gee.HashSet<GenericLiteral> all_literals) {
            string name = "";
            bool negated = false;
            
            char[] arr = str.to_utf8();
            for (int i = 0; i < arr.length; i++) {
                if (arr[i] == NEGATE_CHAR) {
                    negated = !negated;
                } else if (arr[i] == ' ') {
                    //skip
                } else {
                    name = str.substring(i, -1).strip();
                    break;
                }
            }
            
            GenericLiteral new_literal = new GenericLiteral(name);
            Gee.Iterator<GenericLiteral> iterator = all_literals.iterator();
            while (iterator.next() != false) {
                GenericLiteral contained_literal = iterator.get();
                
                if (contained_literal.hash() == new_literal.hash()) {
                    return new Literal(contained_literal, negated);
                }
            }
            
            all_literals.add(new_literal);
            return new Literal(new_literal, negated);
        }
    }
}
