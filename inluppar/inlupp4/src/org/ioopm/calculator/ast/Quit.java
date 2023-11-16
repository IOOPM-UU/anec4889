package org.ioopm.calculator.ast;

public class Quit extends Command {
    @Override
    public SymbolicExpression eval(Environment vars) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    private static final Quit theInstance = new Quit();

    private Quit() {
    }

    public static Quit instance() {
        return theInstance;
    }
}