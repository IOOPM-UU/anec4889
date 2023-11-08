package org.ioopm.calculator.ast;

public class NamedConstant extends Atom {
    private String name;
    private double value;

    public NamedConstant(String name, double value) {
        this.name = name;
        this.value = value;
    }

    @Override
    public boolean isNamedConstant() {
        return true;
    }

    @Override
    public String getNamedConstant() {
        return this.name;
    }

    @Override
    public double getValue() {
        return value;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        return new NamedConstant(this.name, this.value);
    }

    public boolean equals(Object other) {
        if (other instanceof NamedConstant) {
            return this.equals((NamedConstant) other);
        } else {
            return false;
        }
    }

    public boolean equals(NamedConstant other) {
        return this.value == other.value && this.name == other.name;
    }

    public String toString() {
        return String.valueOf(this.name);
    }
}