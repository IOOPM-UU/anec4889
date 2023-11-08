package org.ioopm.calculator.ast;

public abstract class Unary extends SymbolicExpression {
    private SymbolicExpression argument = null;

    Unary(SymbolicExpression argument) {
        this.argument = argument;
    }

    @Override
    public int getPriority() {
        return 150;
    }

    public boolean equals(Object other) {
        if (other instanceof Unary) {
            return this.equals((Unary) other);
        } else {
            return false;
        }
    }

    public boolean equals(Unary other) {
        return this.argument == other.argument && this.getName() == other.getName();
    }

    public String toString() {
        return this.getName() + "(" + this.argument.toString() + ")";
    }
}