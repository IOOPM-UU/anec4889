package org.ioopm.calculator.ast;

public class NamedConstantChecker implements Visitor {

    public boolean check(SymbolicExpression topLevel) {
        try {
            topLevel.accept(this);
        } catch (IllegalExpressionException e) {
            System.err.println(e.getMessage());
            return false;
        }
        return true;
    }

    public SymbolicExpression visit(Addition a) {
        a.lhs.accept(this);
        a.rhs.accept(this);
        return null;
    }

    // When we hit an assignment, make sure to check!
    public SymbolicExpression visit(Assignment a) {
        a.lhs.accept(this);
        a.rhs.accept(this);
        if (a.rhs.isNamedConstant()) {
            throw new IllegalExpressionException("Error, assignments to named constants: \n" + a);
        }
        return null;
    }

    @Override
    public SymbolicExpression visit(Clear n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Constant n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Cos n) {
        n.argument.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Division n) {
        n.lhs.accept(this);
        n.rhs.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Exp n) {
        n.argument.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Log n) {
        n.argument.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Multiplication n) {
        n.lhs.accept(this);
        n.rhs.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Negation n) {
        n.argument.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Quit n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Sin n) {
        n.argument.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Subtraction n) {
        n.lhs.accept(this);
        n.rhs.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Variable n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Vars n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(NamedConstant n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Scope n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Conditional n) {
        return null;
    }
}