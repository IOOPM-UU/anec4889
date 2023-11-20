package org.ioopm.calculator.ast;

public class Exp extends Unary {
    SymbolicExpression argument;

    public Exp(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public String getName() {
        return "Exp";
    }

    public String toString() {
        return super.toString();
    }
}