
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
