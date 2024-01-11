package org.ioopm.calculator;

import java.io.IOException;
import java.util.Scanner;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.SyntaxErrorException;

public abstract class Calculator {

    public static void main(String[] args) throws IOException {
        final CalculatorParser parser = new CalculatorParser();
        final Environment vars = new Environment();
        final EvaluationVisitor evaluator = new EvaluationVisitor();
        final NamedConstantChecker namedChecker = new NamedConstantChecker();
        final ReassignmentChecker reassignChecker = new ReassignmentChecker();
        SymbolicExpression result;
        int expressions = 0;
        int fullyEvaluated = 0;
        boolean parserMode = false;
        Variable ans = new Variable("ans");
        Sequence body = new Sequence();
        FunctionDeclaration function = new FunctionDeclaration(null, null);

        System.out.println("Welcome to the parser \n");
        Scanner sc = new Scanner(System.in);

        while (true) {
            try {
                if (!parserMode) {
                    System.out.print("Please enter an expression: ");
                    String input = sc.nextLine();
                    SymbolicExpression ss = parser.parse(input, vars);
                    if (ss.isFunctionDeclaration()) {
                        body = new Sequence();
                        function = (FunctionDeclaration) ss;
                        vars.put(new Variable(function.getName()), function);
                        System.out.println("Entering function mode\n");
                        parserMode = true;
                    } else {
                        if (ss.isCommand()) {
                            if (ss instanceof Quit) {
                                System.out.println("Thank you for using the parser \n");
                                System.out.println("Total expressions: " + expressions);
                                System.out.println("Fully evaluated expressions: " + fullyEvaluated);
                                break;
                            } else if (ss instanceof Vars) {
                                System.out.println(vars.toString());
                            } else if (ss instanceof Clear) {
                                vars.clear();
                                System.out.println("Variables cleared");
                            }
                        } else {
                            if (namedChecker.check(ss) && reassignChecker.check(ss)) {
                                result = evaluator.evaluate(ss, vars);
                                evaluator.evaluate(new Assignment(result, ans), vars);
                                System.out.println(result + "\n");
                                expressions++;
                                if (result.isConstant())
                                    fullyEvaluated++;
                            }
                        }
                    }
                } else {
                    // Function mode
                    System.out.print("Please enter a function statement, or type 'end': ");
                    String input = sc.nextLine();
                    SymbolicExpression ss = parser.parse(input, vars);

                    if (ss instanceof End && function != null) {
                        function.addBody(body);
                        parserMode = false;
                        evaluator.evaluate(function, vars);
                    } else if (ss.isFunctionDeclaration()) {
                        throw new IllegalExpressionException("Nested function definitions are not permitted");
                    } else {
                        body.addSequence(ss);
                    }
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
        sc.close();
    }
}