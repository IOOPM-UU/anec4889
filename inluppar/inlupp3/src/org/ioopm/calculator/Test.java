package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.Constants;

public class Test {

    public void testPrinting(String expected, SymbolicExpression e) {
        if (expected.equals("" + e)) {
            System.out.println("Passed: " + e);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + e + "'");
        }
    }

    public void testEvaluating(SymbolicExpression expected, SymbolicExpression e, Environment vars) {
        SymbolicExpression r = e.eval(vars);
        if (r.equals(expected)) {
            System.out.println("Passed: " + e);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + e + "'");
        }
    }

    public static void main(String[] args) {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Multiplication m1 = new Multiplication(a, c2);
        // (5 + x) * 2

        Subtraction s1 = new Subtraction(m1, v);
        Division d1 = new Division(s1, c1);
        // (5 + x) * 2 - x

        Constant c3 = new Constant(3);
        Variable v2 = new Variable("y");
        Subtraction s2 = new Subtraction(c3, v2);
        Division d2 = new Division(s2, c2);
        Multiplication m2 = new Multiplication(d2, c1);
        Addition a2 = new Addition(m2, v);
        // (3 - y) / 2 * 5 + x

        Constant c5 = new Constant(6);
        Variable v4 = new Variable("x");
        Addition a4 = new Addition(c5, v4);
        Subtraction s4 = new Subtraction(a4, c2);
        Multiplication m4 = new Multiplication(s4, v4);
        Division d4 = new Division(m4, c1);
        // (6 + x - 2) * x / 5

        Sin t1 = new Sin(new Variable("t"));
        Cos t2 = new Cos(new Constant(3));
        Addition t3 = new Addition(t1, t2);

        Test test1 = new Test();

        test1.testPrinting("(5.0 + x) * 2.0", m1);
        test1.testPrinting("((5.0 + x) * 2.0 - x) / 5.0", d1);
        test1.testPrinting("(3.0 - y) / 2.0 * 5.0 + x", a2);
        test1.testPrinting("(6.0 + x - 2.0) * x / 5.0", d4);
        test1.testPrinting("sin(t) + cos(3.0)", t3);

        SymbolicExpression eval1 = new Addition(new Constant(5), new Constant(37));
        SymbolicExpression eval2 = new Constant(42);

        test1.testEvaluating(eval2, eval1, new Environment());

        Environment env = new Environment();

        Assignment ass = new Assignment(new Constant(8), v);
        ass.eval(env);
        Constant d = new Constant(8);
        test1.testEvaluating(d, ass, env);

        Assignment f1 = new Assignment(new Constant(0), new Variable("k"));
        f1.eval(env);
        Assignment f = new Assignment(new Constant(0), new Variable("pi"));
        f.eval(env);

    }
}