package org.ioopm.calculator.ast;

import java.util.ArrayList;

public class Sequence extends SymbolicExpression {
    ArrayList<SymbolicExpression> body = new ArrayList<>();

    public void addSequence(SymbolicExpression line) {
        this.body.add(line);
    }

    public void prependSequence(SymbolicExpression line) {
        this.body.add(0, line);
    }

    public void popSequence() {
        this.body.remove(0);
    }

    public int getSize() {
        return this.body.size();
    }

    public ArrayList<SymbolicExpression> getBody() {
        return this.body;
    }

    @Override
    public String toString() {
        return body.toString();
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