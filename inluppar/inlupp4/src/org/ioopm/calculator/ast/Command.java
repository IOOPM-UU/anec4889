package org.ioopm.calculator.ast;

public abstract class Command extends SymbolicExpression {
    @Override
    public int getPriority() {
        throw new RuntimeException("Has no getPriority()");
    }

    @Override
    public boolean isCommand() {
        return true;
    }
}