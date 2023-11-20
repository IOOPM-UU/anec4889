package org.ioopm.calculator.ast;

public class Negation extends Unary {
    private SymbolicExpression argument;

    public Negation(SymbolicExpression argument) {
        super(argument);
        this.argument = argument;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public int getPriority() {
        return 100;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(arg.getValue() * (-1));
        } else {
            return new Negation(arg);
        }
    }

    @Override
    public String getName() {
        return "-";
    }

    public String toString() {
        return super.toString();
    }
}