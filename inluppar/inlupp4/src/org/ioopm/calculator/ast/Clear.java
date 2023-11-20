package org.ioopm.calculator.ast;

public class Clear extends Command {
    @Override
    public SymbolicExpression eval(Environment vars) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    private static final Clear theInstance = new Clear();

    private Clear() {
    }

    public static Clear instance() {
        return theInstance;
    }
}