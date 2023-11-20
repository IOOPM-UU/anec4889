package org.ioopm.calculator.ast;

public class Addition extends Binary {
    SymbolicExpression lhs;
    SymbolicExpression rhs;

    public Addition(SymbolicExpression lhs, SymbolicExpression rhs) {
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
        return 50;
    }

    @Override
    public String getName() {
        return "+";
    }

    public String toString() {
        return super.toString();
    }
}