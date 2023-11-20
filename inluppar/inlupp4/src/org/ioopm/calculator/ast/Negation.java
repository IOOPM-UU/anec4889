package org.ioopm.calculator.ast;

public class Negation extends Unary {
    SymbolicExpression argument;

    public Negation(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public int getPriority() {
        return 100;
    }

    @Override
    public String getName() {
        return "-";
    }

    public String toString() {
        return super.toString();
    }
}