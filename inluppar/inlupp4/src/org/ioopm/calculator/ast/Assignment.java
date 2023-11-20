package org.ioopm.calculator.ast;

public class Assignment extends Binary {
    SymbolicExpression lhs;
    SymbolicExpression rhs;

    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
        this.lhs = lhs;
        this.rhs = rhs;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public int getPriority() {
        return 25;
    }

    @Override
    public String getName() {
        return "=";
    }
}