package org.ioopm.calculator.ast;

public class Constant extends Atom {
    private double value;

    public Constant(double value) {
        this.value = value;
    }

    @Override
    public boolean isConstant() {
        return true;
    }

    @Override
    public double getValue() {
        return value;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        return new Constant(this.value);
    }

    public boolean equals(Object other) {
        if (other instanceof Constant) {
            return this.equals((Constant) other);
        } else {
            return false;
        }
    }

    public boolean equals(Constant other) {
        return this.value == other.value;
    }

    public String toString() {
        return String.valueOf(this.value);
    }
}