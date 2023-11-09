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

    @Override
    public SymbolicExpression eval(Environment vars) {
        if (vars.containsKey(this)) {
            if (vars.get(this).isConstant()) {
                return new Constant(vars.get(this).getValue());
            }
            return vars.get(this);
        }
        return new Variable(this.identifier);
    }

    public String toString() {
        return String.valueOf(identifier);
    }
}