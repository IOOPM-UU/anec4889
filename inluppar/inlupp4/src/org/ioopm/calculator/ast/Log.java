package org.ioopm.calculator.ast;

public class Log extends Unary {
    SymbolicExpression argument;

    public Log(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public String getName() {
        return "Log";
    }

    public String toString() {
        return super.toString();
    }
}