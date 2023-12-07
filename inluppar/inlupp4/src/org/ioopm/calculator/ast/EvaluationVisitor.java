package org.ioopm.calculator.ast;

import java.util.ArrayList;

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
        if (n.rhs.isUnary()) {
            throw new IllegalExpressionException("Error: cannot assign unary operator " + n.rhs.getName());
        }
        this.vars.put(new Variable(n.rhs.getVariable()), lhs);
        if (lhs.isConstant()) {
            return new Constant(lhs.getValue());
        } else {
            return lhs;
        }
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
            SymbolicExpression exp = vars.get(n);
            return exp.accept(this);
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

    @Override
    public SymbolicExpression visit(Scope n) {
        Environment temp = this.vars;
        this.vars = (Environment) temp.clone();
        SymbolicExpression arg = n.arg.accept(this);
        this.vars = temp;
        return arg;
    }

    @Override
    public SymbolicExpression visit(Conditional n) {
        boolean ifElse = false;
        SymbolicExpression lhs = n.idLhs.accept(this);
        SymbolicExpression rhs = n.idRhs.accept(this);
        SymbolicExpression taken = n.scopeLhs;
        SymbolicExpression notTaken = n.scopeRhs;
        if (lhs.isConstant() && rhs.isConstant()) {
            if (n.op.equals("<")) {
                ifElse = lhs.getValue() < rhs.getValue();
            } else if (n.op.equals(">")) {
                ifElse = lhs.getValue() > rhs.getValue();
            } else if (n.op.equals("<=")) {
                ifElse = lhs.getValue() <= rhs.getValue();
            } else if (n.op.equals(">=")) {
                ifElse = lhs.getValue() >= rhs.getValue();
            } else {
                ifElse = lhs.getValue() == rhs.getValue();
            }
            if (ifElse) {
                return new Scope(taken).accept(this);
            } else {
                return new Scope(notTaken).accept(this);
            }
        } else {
            return new Conditional(lhs, n.op, rhs, lhs, rhs);
        }
    }

    @Override
    public SymbolicExpression visit(FunctionDeclaration n) {
        vars.put(new Variable(n.functionName), n);
        return null;
    }

    @Override
    public SymbolicExpression visit(End n) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    @Override
    public SymbolicExpression visit(Sequence n) {
        SymbolicExpression functionReturn = null;
        for (SymbolicExpression p : n.body) {
            functionReturn = p.accept(this);
        }
        return functionReturn;
    }

    @Override
    public SymbolicExpression visit(FunctionCall n) {
        FunctionDeclaration function = (FunctionDeclaration) vars.get(new Variable(n.functionName));
        if (n.parameters.size() != function.parameters.size()) {
            throw new IllegalExpressionException("Parameters don't match");
        }

        ArrayList<SymbolicExpression> valueParameters = n.parameters;
        ArrayList<SymbolicExpression> nameParameters = function.parameters;

        Environment temp = this.vars;
        this.vars = (Environment) temp.clone();

        for (int i = 0; i < valueParameters.size(); i++) {
            SymbolicExpression con = valueParameters.get(i).accept(this);
            Variable var = (Variable) nameParameters.get(i);
            vars.put(var, con);
        }
        SymbolicExpression functionResult = function.body.accept(this);

        this.vars = temp;
        return functionResult;
    }
}