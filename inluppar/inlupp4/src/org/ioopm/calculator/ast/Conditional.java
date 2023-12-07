package org.ioopm.calculator.ast;

public class Conditional extends SymbolicExpression {
    SymbolicExpression idLhs;
    String op;
    SymbolicExpression idRhs;
    SymbolicExpression scopeLhs;
    SymbolicExpression scopeRhs;

    public Conditional(SymbolicExpression idLhs, String op, SymbolicExpression idRhs,
            SymbolicExpression scopeLhs,
            SymbolicExpression scopeRhs) {
        this.idLhs = idLhs;
        this.idRhs = idRhs;
        this.op = op;
        this.scopeLhs = scopeLhs;
        this.scopeRhs = scopeRhs;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public int getPriority() {
        throw new RuntimeException("Has no getPriority()");
    }
}