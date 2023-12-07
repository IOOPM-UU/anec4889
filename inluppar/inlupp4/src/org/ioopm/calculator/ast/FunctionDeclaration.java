package org.ioopm.calculator.ast;

import java.util.ArrayList;

public class FunctionDeclaration extends SymbolicExpression {
    String functionName;
    ArrayList<SymbolicExpression> parameters = new ArrayList<>();
    Sequence body = null;

    public FunctionDeclaration(String functionName, ArrayList<SymbolicExpression> parameters) {
        this.functionName = functionName;
        this.parameters = parameters;
    }

    public void addBody(Sequence body) {
        this.body = body;
    }

    @Override
    public String getName() {
        return this.functionName;
    }

    @Override
    public boolean isFunctionDeclaration() {
        return true;
    }

    @Override
    public String toString() {
        String str = "Name: " + functionName + "\n";
        str += "Parameters:" + parameters.toString() + "\n";
        str += "Body: " + body.toString();
        return str;
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