package org.ioopm.calculator.ast;

import java.util.ArrayList;

public class ReassignmentChecker implements Visitor {
    private ArrayList<String> assignments = new ArrayList<>();

    public boolean check(SymbolicExpression topLevel) {
        try {
            topLevel.accept(this);
        } catch (IllegalExpressionException e) {
            System.err.println(e.getMessage());
            assignments.clear();
            return false;
        }
        assignments.clear();
        return true;
    }

    @Override
    public SymbolicExpression visit(Addition a) {
        a.lhs.accept(this);
        a.rhs.accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Assignment a) {
        a.lhs.accept(this);
        a.rhs.accept(this);
        if (assignments.contains(a.rhs.getVariable())) {
            throw new IllegalExpressionException("Error, the variable " + a.rhs.getVariable() + " is reassigned");
        }
        assignments.add(a.rhs.getVariable());
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

    @Override
    public SymbolicExpression visit(FunctionDeclaration n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(End n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Sequence n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(FunctionCall n) {
        return null;
    }
}