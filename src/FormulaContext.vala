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
    private Literal[] all_literals;
    private PartialAssignment solution;
    
    public FormulaContext() {
        all_literals = null;
        solution = null;
    }
    
    public Literal[]? get_all_literals() {
        return all_literals;
    }
    
    public void set_all_literals(Literal[] all_literals) {
        this.all_literals = all_literals;
    }
    
    public void set_solution(PartialAssignment solution) {
        this.solution = solution;
    }
    
    public PartialAssignment? get_solution() {
        return solution;
    }
}
