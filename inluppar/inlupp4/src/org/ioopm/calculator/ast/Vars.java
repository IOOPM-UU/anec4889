package org.ioopm.calculator.ast;

public class Vars extends Command {
    @Override
    public SymbolicExpression eval(Environment vars) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    private static final Vars theInstance = new Vars();

    private Vars() {
    }

    public static Vars instance() {
        return theInstance;
    }
}