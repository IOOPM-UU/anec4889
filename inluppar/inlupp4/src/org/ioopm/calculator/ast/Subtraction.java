package org.ioopm.calculator.ast;

public class Subtraction extends Binary {
    private SymbolicExpression lhs;
    private SymbolicExpression rhs;

    public Subtraction(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
        this.lhs = lhs;
        this.rhs = rhs;
    }

    @Override
    public int getPriority() {
        return 50;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lhs = this.lhs.eval(vars);
        SymbolicExpression rhs = this.rhs.eval(vars);
        if (lhs.isConstant() && rhs.isConstant()) {
            return new Constant(lhs.getValue() - rhs.getValue());
        } else {
            return new Subtraction(lhs, rhs);
        }
    }

    @Override
    public String getName() {
        return "-";
    }
}