package org.ioopm.calculator;

import java.io.IOException;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.CalculatorParser;

public abstract class Calculator {

    public static void main(String[] args) throws IOException {
        CalculatorParser parser = new CalculatorParser();
        final Environment vars = new Environment();
        Variable ans = new Variable(null);

        while (true) {
            String input = System.console().readLine();

            SymbolicExpression ss = parser.parse(input, vars);
            if (ss.isCommand()) {
                return;
            }
            Assignment result = new Assignment(ss.eval(vars), ans);
            System.out.println(result.toString());
        }
    }
}