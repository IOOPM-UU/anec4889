package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Constants;

public class Assignment extends Binary {
    private SymbolicExpression lhs;
    private SymbolicExpression rhs;

    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
        this.lhs = lhs;
        this.rhs = rhs;
    }

    @Override
    public int getPriority() {
        return 25;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lhs = this.lhs.eval(vars);
        if (Constants.namedConstants.containsKey(this.rhs.getVariable())) {
            throw new IllegalExpressionException("Error: cannot redefine named constant");
        } else if (rhs.isVariable()) {
            vars.put(new Variable(rhs.getVariable()), lhs);
            return rhs.eval(vars);
        } else {
            return new Assignment(lhs, rhs);
        }
    }

    @Override
    public String getName() {
        return "=";
    }

    public String toString() {
        return super.toString();
    }
}