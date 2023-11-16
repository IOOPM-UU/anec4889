package org.ioopm.calculator.ast;

public class Log extends Unary {
    private SymbolicExpression argument;

    public Log(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.log(arg.getValue()));
        } else {
            return new Log(arg);
        }
    }

    @Override
    public String getName() {
        return "Log";
    }

    public String toString() {
        return super.toString();
    }
}