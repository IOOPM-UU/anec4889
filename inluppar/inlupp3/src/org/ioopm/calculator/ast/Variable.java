package org.ioopm.calculator.ast;

public class Variable extends Atom {
    private String identifier;

    public Variable(String identifier) {
        this.identifier = identifier;
    }

    @Override
    public boolean isVariable() {
        return true;
    }

    @Override
    public int hashCode() {
        return this.getVariable().hashCode();
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        if (vars.containsKey(this)) {
            return new Constant(vars.get(this).getValue());
        }
        return new Variable(this.identifier);
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
        return this.identifier == other.identifier;
    }

    public String toString() {
        return String.valueOf(identifier);
    }
}