package org.ioopm.calculator;

import java.io.IOException;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.SyntaxErrorException;

public abstract class Calculator {

    public static void main(String[] args) throws IOException {
        final CalculatorParser parser = new CalculatorParser();
        final Environment vars = new Environment();
        SymbolicExpression result;
        int expressions = 0;
        int fullyEvaluated = 0;
        Variable ans = new Variable("ans");

        System.out.println("Welcome to the parser \n");

        while (true) {
            try {
                String input = System.console().readLine("Please enter an expression: ");

                SymbolicExpression ss = parser.parse(input, vars);
                if (ss.isCommand()) {
                    if (ss instanceof Quit) {
                        System.out.println("Thank you for using the parser \n");
                        System.out.println("Total expressions: " + expressions);
                        System.out.println("Fully evaluated expressions: " + fullyEvaluated);
                        break;
                    } else if (ss instanceof Vars) {
                        vars.forEach((k, v) -> System.out.println(k + " = " + v));
                    } else if (ss instanceof Clear) {
                        vars.clear();
                        System.out.println("Variables cleared");
                    }
                } else {
                    result = ss.eval(vars);
                    new Assignment(result, ans).eval(vars);
                    System.out.println(result + "\n");
                    expressions++;
                    if (result.isConstant())
                        fullyEvaluated++;
                }
            } catch (IllegalExpressionException d) {
                System.out.println(d.getMessage());
                continue;
            } catch (IOException d) {
                System.out.println(d.getMessage());
                continue;
            } catch (SyntaxErrorException d) {
                System.out.println(d.getMessage());
                continue;
            } catch (RuntimeException d) {
                System.out.println(d.getMessage());
                continue;
            }
        }
    }
}