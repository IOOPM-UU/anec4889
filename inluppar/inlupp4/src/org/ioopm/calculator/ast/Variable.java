package org.ioopm.calculator.ast;

public class Variable extends Atom implements Comparable<Variable> {
    String identifier;

    public Variable(String identifier) {
        this.identifier = identifier;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public boolean isVariable() {
        return true;
    }

    @Override
    public int hashCode() {
        return this.identifier.hashCode();
    }

    @Override
    public String getVariable() {
        return identifier;
    }

    public boolean equals(Object other) {
        if (other instanceof Variable) {
            return this.equals((Variable) other);
        } else {
            return false;
        }
    }

    public boolean equals(Variable other) {
        return this.identifier.equals(other.identifier);
    }

    public String toString() {
        return String.valueOf(identifier);
    }

    @Override
    public int compareTo(Variable other) {
        return this.identifier.compareTo(other.identifier);
    }
}