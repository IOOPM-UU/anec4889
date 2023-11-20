package org.ioopm.calculator.ast;

public class Sin extends Unary {
    SymbolicExpression argument;

    public Sin(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public String getName() {
        return "Sin";
    }
}