package org.ioopm.calculator.ast;

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
        SymbolicExpression lhs = n.lhs.eval(vars);
        if (n.rhs.isNamedConstant()) {
            throw new IllegalExpressionException("Error: cannot redefine named constant '" + n.rhs + "'");
        } else if (n.rhs.isVariable()) {
            vars.put(new Variable(n.rhs.getVariable()), lhs);
            return n.rhs.eval(vars);
        }
        return new Assignment(lhs, n.rhs);
    }

    @Override
    public SymbolicExpression visit(Clear n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Constant n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Cos n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Division n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Exp n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Log n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Multiplication n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Negation n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Quit n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Sin n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Subtraction n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Variable n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(Vars n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }

    @Override
    public SymbolicExpression visit(NamedConstant n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }
}