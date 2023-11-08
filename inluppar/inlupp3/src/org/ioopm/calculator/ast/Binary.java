package org.ioopm.calculator.ast;

public abstract class Binary extends SymbolicExpression {
    private SymbolicExpression lhs = null;
    private SymbolicExpression rhs = null;

    Binary(SymbolicExpression lhs, SymbolicExpression rhs) {
        this.lhs = lhs;
        this.rhs = rhs;
    }

    public boolean equals(Object other) {
        if (other instanceof Binary) {
            return this.equals((Binary) other);
        } else {
            return false;
        }
    }

    public boolean equals(Binary other) {
        return this.lhs == other.lhs && this.rhs == other.rhs && this.getName() == other.getName();
    }

    public String toString() {
        if (this.lhs.getPriority() < this.getPriority())
            return "(" + this.lhs.toString() + ") " + this.getName() + " " + this.rhs.toString();
        return this.lhs.toString() + " " + this.getName() + " " + this.rhs.toString();
    }
}