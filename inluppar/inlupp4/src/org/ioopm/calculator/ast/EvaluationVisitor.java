package org.ioopm.calculator.ast;

//import static org.junit.jupiter.api.Assertions.assertEquals;

public class EvaluationVisitor implements Visitor {
    private Environment vars = null;

    public SymbolicExpression evaluate(SymbolicExpression topLevel, Environment env) {
        this.vars = env;
        return topLevel.accept(this);
    }

    public SymbolicExpression visit(Addition n) {
        SymbolicExpression left = n.lhs.accept(this);
        SymbolicExpression right = n.rhs.accept(this);
        if (left.isConstant() && right.isConstant()) {
            return new Constant(left.getValue() + right.getValue());
        } else {
            return new Addition(left, right);
        }
    }

    public SymbolicExpression visit(Assignment n) {
        SymbolicExpression lhs = n.lhs.accept(this);
        if (n.rhs.isNamedConstant()) {
            throw new IllegalExpressionException("Error: cannot redefine named constant '" + n.rhs + "'");
        } else if (n.rhs.isVariable()) {
            vars.put(new Variable(n.rhs.getVariable()), lhs);
            return n.rhs.accept(this);
        }
        return new Assignment(lhs, n.rhs);
    }

    @Override
    public SymbolicExpression visit(Clear n) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    @Override
    public SymbolicExpression visit(Constant n) {
        return new Constant(n.getValue());
    }

    @Override
    public SymbolicExpression visit(Cos n) {
        SymbolicExpression arg = n.argument.accept(this);
        if (arg.isConstant()) {
            return new Constant(Math.cos(arg.getValue()));
        } else {
            return new Cos(arg);
        }
    }

    @Override
    public SymbolicExpression visit(Division n) {
        SymbolicExpression lhs = n.lhs.accept(this);
        SymbolicExpression rhs = n.rhs.accept(this);
        if (lhs.isConstant() && rhs.isConstant()) {
            return new Constant(lhs.getValue() / rhs.getValue());
        } else {
            return new Division(lhs, rhs);
        }
    }

    @Override
    public SymbolicExpression visit(Exp n) {
        SymbolicExpression arg = n.argument.accept(this);
        if (arg.isConstant()) {
            return new Constant(Math.exp(arg.getValue()));
        } else {
            return new Exp(arg);
        }
    }

    @Override
    public SymbolicExpression visit(Log n) {
        SymbolicExpression arg = n.argument.accept(this);
        if (arg.isConstant()) {
            return new Constant(Math.log(arg.getValue()));
        } else {
            return new Log(arg);
        }
    }

    @Override
    public SymbolicExpression visit(Multiplication n) {
        SymbolicExpression lhs = n.lhs.accept(this);
        SymbolicExpression rhs = n.rhs.accept(this);
        if (lhs.isConstant() && rhs.isConstant()) {
            return new Constant(lhs.getValue() * rhs.getValue());
        } else {
            return new Multiplication(lhs, rhs);
        }
    }

    @Override
    public SymbolicExpression visit(Negation n) {
        SymbolicExpression arg = n.argument.accept(this);
        if (arg.isConstant()) {
            return new Constant(arg.getValue() * (-1));
        } else {
            return new Negation(arg);
        }
    }

    @Override
    public SymbolicExpression visit(Quit n) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    @Override
    public SymbolicExpression visit(Sin n) {
        SymbolicExpression arg = n.argument.accept(this);
        if (arg.isConstant()) {
            return new Constant(Math.sin(arg.getValue()));
        } else {
            return new Sin(arg);
        }
    }

    @Override
    public SymbolicExpression visit(Subtraction n) {
        SymbolicExpression lhs = n.lhs.accept(this);
        SymbolicExpression rhs = n.rhs.accept(this);
        if (lhs.isConstant() && rhs.isConstant()) {
            return new Constant(lhs.getValue() - rhs.getValue());
        } else {
            return new Subtraction(lhs, rhs);
        }
    }

    @Override
    public SymbolicExpression visit(Variable n) {
        if (vars.containsKey(n)) {
            if (vars.get(n).isConstant()) {
                return new Constant(vars.get(n).getValue());
            }
            return vars.get(n);
        }
        return new Variable(n.identifier);
    }

    @Override
    public SymbolicExpression visit(Vars n) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    @Override
    public SymbolicExpression visit(NamedConstant n) {
        return new Constant(n.getValue());
    }
}