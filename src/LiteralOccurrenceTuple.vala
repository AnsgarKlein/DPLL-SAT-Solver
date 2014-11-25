
private class LiteralOccurrenceTuple {
    private Literal literal;
    
    /**
     * The number of occurrences of a Literal in a corresponding Formula.
    **/
    private int occurrences;
    
    public LiteralOccurrenceTuple(Literal literal, int occurrences) {
        this.literal = literal;
        this.occurrences = occurrences;
    }
    
    /**
     * Returns the corresponding Literal
    **/
    public Literal get_literal() {
        return literal;
    }

    /**
     * Returns a negative integer if the first Literal has more occurrences.
     * Returns 0 if both Literals have the same amount of occurrences.
     * Returns a positive integer if the first value has less occurrences.
    **/
    public static int compare(LiteralOccurrenceTuple a, LiteralOccurrenceTuple b) {
        int ai = a.occurrences;
        int bi = b.occurrences;
        
        return (int)(ai < bi) - (int)(ai > bi);
    }
}
