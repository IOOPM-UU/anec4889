package org.ioopm.calculator.ast;

public class Exp extends Unary {
    private SymbolicExpression argument;

    public Exp(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.exp(arg.getValue()));
        } else {
            return new Exp(arg);
        }
    }

    @Override
    public String getName() {
        return "Exp";
    }

    public String toString() {
        return super.toString();
    }
}