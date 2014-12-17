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
    namespace DIMACS {
        private static Formula? parse_formula(string[] lines) {
            int literals_count;
            int clauses_count;
            
            // Skip comments
            int i = 0;
            for (i = 0; i < lines.length; i++) {
                if (lines[i].slice(0, 1) == "c") {
                } else {
                    break;
                }
            }
            
            // Parse problem line
            string[] params = lines[i].split(" ");
            
            if (params.length != 4 || params[0] != "p" || params[1] != "cnf") {
                stderr.printf("Error - missing/malformed problem line\n");
                return null;
            }
            
            literals_count = int.parse(params[2]);
            clauses_count = int.parse(params[3]);
            
            if (literals_count == 0) {
                stderr.printf("Error - number of literals is 0 according to problem line!\n");
                stderr.printf(" - this can't be right\n");
                return null;
            }
            
            if (clauses_count == 0) {
                stderr.printf("Error - number of clauses is 0 according to problem line!\n");
                stderr.printf(" - this can't be right\n");
                return null;
            }
            
            i++;
            
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
            
            // Parse Clauses
            int clauses_found = 0;
            Gee.HashSet<Clause> clauses_set = new Gee.HashSet<Clause>(null, null);
            for (; i < lines.length; i++) {
                // Ignore it if the last line is empty
                if (i == lines.length - 1) {
                    if (lines[i] == "") {
                        continue;
                    }
                }
                
                Clause clause = parse_clause(lines[i], all_literals);
                
                if (clause == null) {
                    return null;
                }
                
                clauses_found++;
                clauses_set.add(clause);
            }
            
            // Check if given number of clauses was correct
            if (clauses_found != clauses_count) {
                stderr.printf("Error - number of clauses given in problem line is not correct!\n");
                return null;
            }
            
            //TODO: Check for number of Literals
            
            return new Formula(clauses_set, new FormulaContext());
        }
        
        private static Clause? parse_clause(string line, Gee.HashSet<GenericLiteral> all_literals) {
            Gee.LinkedList<Literal> literals = new Gee.LinkedList<Literal>();
            
            // Empty lines are not permitted
            if (line.strip() == "") {
                stderr.printf("Error - empty lines are not permitted!\n");
                return null;
            }
            
            string[] literals_array = line.split(" ");
            for (int i = 0; i < literals_array.length; i++) {
                int literal_name_i = int.parse(literals_array[i]);
                
                // The last element of each line has to be 0
                if (i == literals_array.length - 1) {
                    if (literal_name_i != 0) {
                        stderr.printf("Error - Last element of line was not 0!\n");
                        return null;
                    }
                    break;
                }
                
                // 0 is only allowed to be the last element of each line.
                if (literal_name_i == 0 && i != literals_array.length - 1) {
                    stderr.printf("Error - Found Literal 0, which is not permitted!\n");
                    return null;
                }
                
                // Parse Literal
                bool negated = false;
                string name;
                
                if (literal_name_i < 0) {
                    negated = true;
                    literal_name_i = literal_name_i.abs();
                }
                name = literal_name_i.to_string();
                
                // Create Literal
                bool already_contained = false;
                
                GenericLiteral new_literal = new GenericLiteral(name);
                Gee.Iterator<GenericLiteral> iterator = all_literals.iterator();
                while (iterator.next() != false) {
                    GenericLiteral contained_literal = iterator.get();
                    
                    if (contained_literal.hash() == new_literal.hash()) {
                        already_contained = true;
                        literals.add(new Literal(contained_literal, negated));
                    }
                }
                
                if (!already_contained) {
                    all_literals.add(new_literal);
                    literals.add(new Literal(new_literal, negated));
                }
            }
            
            return new Clause(literals.to_array());
        }
    }
}
