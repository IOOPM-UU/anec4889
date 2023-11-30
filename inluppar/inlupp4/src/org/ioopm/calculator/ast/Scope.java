package org.ioopm.calculator.ast;

public class Scope extends Unary {
    SymbolicExpression arg;

    public Scope(SymbolicExpression arg) {
        super(arg);
        this.arg = arg;
    }

    @Override
    public String getName() {
        return super.getName();
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}