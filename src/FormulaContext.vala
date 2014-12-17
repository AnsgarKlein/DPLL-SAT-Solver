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


public class FormulaContext {
    private GenericLiteral[] all_literals;
    
    public FormulaContext() {
        all_literals = null;
    }
    
    public GenericLiteral[]? get_all_literals() {
        return all_literals;
    }
    
    public void set_all_literals(GenericLiteral[] all_literals) {
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
}
