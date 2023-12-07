package org.ioopm.calculator.ast;

import java.util.ArrayList;

public class FunctionCall extends SymbolicExpression {
    String functionName;
    ArrayList<SymbolicExpression> parameters;

    public FunctionCall(String functionName, ArrayList<SymbolicExpression> parameters) {
        this.functionName = functionName;
        this.parameters = parameters;
    }

    @Override
    public String getName() {
        return this.functionName;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public int getPriority() {
        throw new RuntimeException("Has no getPriority()");
    }
}