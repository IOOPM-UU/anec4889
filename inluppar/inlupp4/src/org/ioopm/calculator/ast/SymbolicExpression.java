package org.ioopm.calculator.ast;

public abstract class SymbolicExpression {

    public abstract SymbolicExpression accept(Visitor v);

    public boolean isConstant() {
        return false;
    }

    public boolean isUnary() {
        return false;
    }

    public boolean isVariable() {
        return false;
    }

    public boolean isNamedConstant() {
        return false;
    }

    public boolean isCommand() {
        return false;
    }

    public double getValue() {
        throw new RuntimeException("getValue() called on a non constant");
    }

    public String getVariable() {
        throw new RuntimeException("getVariable() called on a non variable");
    }

    public String getNamedConstant() {
        throw new RuntimeException("getNamedConstant() called on a non variable");
    }

    public String getName() {
        throw new RuntimeException("getName() called on expression with no operator");
    }

    public abstract int getPriority();
}