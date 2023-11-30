package org.ioopm.calculator.ast;

public class Conditional {
    SymbolicExpression idLhs;
    SymbolicExpression op;
    SymbolicExpression idRhs;
    SymbolicExpression scopeLhs;
    SymbolicExpression scopeRhs;

    public Conditional(SymbolicExpression idLhs, SymbolicExpression op, SymbolicExpression idRhs,
            SymbolicExpression scopeLhs,
            SymbolicExpression scopeRhs) {
        this.idLhs = idLhs;
        this.idRhs = idRhs;
        this.op = op;
        this.scopeLhs = scopeLhs;
        this.scopeRhs = scopeRhs;
    }

    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}