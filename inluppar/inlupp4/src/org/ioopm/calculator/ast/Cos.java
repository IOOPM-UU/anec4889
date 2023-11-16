package org.ioopm.calculator.ast;

public class Cos extends Unary {
    private SymbolicExpression argument;

    public Cos(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.cos(arg.getValue()));
        } else {
            return new Cos(arg);
        }
    }

    @Override
    public String getName() {
        return "Cos";
    }

    public String toString() {
        return super.toString();
    }
}