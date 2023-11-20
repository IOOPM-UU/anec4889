package org.ioopm.calculator.ast;

import java.util.HashMap;
import java.util.TreeSet;

public class Environment extends HashMap<Variable, SymbolicExpression> {
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Variables: ");
        TreeSet<Variable> vars = new TreeSet<>(this.keySet());
        for (Variable v : vars) {
            sb.append("\n");
            sb.append(v.getVariable());
            sb.append(" = ");
            sb.append(this.get(v));
        }
        return sb.toString();
    }
}