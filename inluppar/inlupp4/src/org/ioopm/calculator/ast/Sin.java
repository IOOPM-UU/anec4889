package org.ioopm.calculator.ast;

public class Sin extends Unary {
    private SymbolicExpression argument;

    public Sin(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.sin(arg.getValue()));
        } else {
            return new Sin(arg);
        }
    }

    @Override
    public String getName() {
        return "Sin";
    }

    public String toString() {
        return super.toString();
    }
}