package org.ioopm.calculator;

import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.SyntaxErrorException;

public class ParserTests {
    static CalculatorParser parser;
    static Environment vars;
    static EvaluationVisitor evaluator;

    @BeforeAll
    public static void initParser() {
        parser = new CalculatorParser();
        vars = new Environment();
        evaluator = new EvaluationVisitor();
    }

    static SymbolicExpression parse(String input) throws IOException {
        return parser.parse(input, vars);
    }

    static void runTest(SymbolicExpression e) throws IOException {
        String input = e.toString();
        SymbolicExpression result = parse(input);
        assertEquals(e, result);
    }

    static void runTestFail(SymbolicExpression e) throws IOException {
        String input = e.toString();
        SymbolicExpression result = parse(input);
        assertNotEquals(e, result);
    }

    static SymbolicExpression con(double d) {
        return new Constant(d);
    }

    static SymbolicExpression var(String s) {
        return new Variable(s);
    }

    static SymbolicExpression add(SymbolicExpression l, SymbolicExpression r) {
        return new Addition(l, r);
    }

    static SymbolicExpression sub(SymbolicExpression l, SymbolicExpression r) {
        return new Subtraction(l, r);
    }

    static SymbolicExpression mul(SymbolicExpression l, SymbolicExpression r) {
        return new Multiplication(l, r);
    }

    static SymbolicExpression div(SymbolicExpression l, SymbolicExpression r) {
        return new Division(l, r);
    }

    static SymbolicExpression neg(SymbolicExpression n) {
        return new Negation(n);
    }

    static SymbolicExpression asg(SymbolicExpression l, SymbolicExpression r) {
        return new Assignment(l, r);
    }

    static SymbolicExpression cos(SymbolicExpression a) {
        return new Cos(a);
    }

    static SymbolicExpression exp(SymbolicExpression a) {
        return new Exp(a);
    }

    static SymbolicExpression log(SymbolicExpression a) {
        return new Log(a);
    }

    @Test
    void testConstant() throws IOException {
        runTest(con(42.0));
        runTest(con(0.0));
        runTest(con(3.14159));
        runTest(neg(con(3.14159)));
    }

    @Test
    void testVariable() throws IOException {
        runTest(var("x"));
        runTest(var("xyz"));
        runTest(var("aVeryLongVariableNameThatNeverSeemsToEnd"));
    }

    @Test
    void testAddition() throws IOException {
        runTest(add(con(1.0), con(2.0)));
        runTest(add(var("x"), con(2.0)));
    }

    @Test
    void testParentheses() throws IOException {
        assertEquals(parse("(1)"), new Constant(1.0));
        assertEquals(parse("((1))"), new Constant(1.0));
        assertEquals(parse("(((1)))"), new Constant(1.0));
        assertEquals(parse("(((1 + 2) + 3) + 4)"),
                add(add(add(con(1), con(2)), con(3)), con(4)));
        assertEquals(parse("(1 + (2 + (3 + 4)))"),
                add(con(1), add(con(2), add(con(3), con(4)))));
        assertThrows(SyntaxErrorException.class, () -> {
            parse("(1 + ");
        });
    }

    @Test
    void testTree() throws IOException {
        SymbolicExpression tree = asg(mul(cos(var("x")), con(5)), var("y"));
        SymbolicExpression tree2 = add(log(con(4)), exp(con(2)));
        runTest(tree);
        runTest(tree2);
    }

    @Test
    void testGetValue() {
        assertEquals(3, con(3).getValue());
        assertEquals(3, evaluator.evaluate(asg(con(3), var("y")), vars).getValue());
        assertThrows(RuntimeException.class, () -> {
            var("x").getValue();
        });
        assertNotEquals(3, con(5).getValue());
    }

    @Test
    void testIsConstant() {
        assertFalse(var("x").isConstant());
        assertTrue(con(3).isConstant());
        assertTrue(evaluator.evaluate(asg(con(3), var("y")), vars).isConstant());
    }

    @Test
    void testGetName() {
        assertEquals("+", add(var("y"), con(3)).getName());
        assertEquals("-", sub(var("y"), con(3)).getName());
        assertEquals("-", sub(var("y"), con(3)).getName());
        assertEquals("=", asg(con(3), var("y")).getName());
        assertThrows(RuntimeException.class, () -> {
            var("x").getName();
        });
    }

    @Test
    void testIsCommand() throws IOException {
        assertTrue(parse("Quit").isCommand());
        assertTrue(parse("Vars").isCommand());
        assertTrue(parse("Clear").isCommand());
        assertFalse(var("d").isCommand());
    }

    @Test
    void testPriority() {
        assertEquals(50, add(con(2), con(1)).getPriority());
        assertEquals(100, mul(con(2), con(1)).getPriority());
    }

    @Test
    void testToString() {
        assertEquals("3.0 + 2.0", add(con(3), con(2)).toString());
        assertEquals("3.0 * 2.0", mul(con(3), con(2)).toString());
        assertEquals("(3.0 - x) / 2.0", div(sub(con(3), var("x")), con(2)).toString());
        assertEquals("Cos(x + 3.0)", cos(add(var("x"), con(3))).toString());
    }

    @Test
    void testToEquals() {
        assertTrue(cos(con(5)).equals(cos(evaluator.evaluate(add(con(3), con(2)), vars))));
        assertTrue(mul(var("x"), con(3)).equals(mul(var("x"), con(3))));
        assertTrue(var("x").equals(var("x")));
        assertFalse(var("x").equals(var("y")));
    }

    @Test
    void testEval() throws IOException {
        assertEquals(add(var("x"), con(3)), evaluator.evaluate(add(var("x"), con(3)), vars));
        assertEquals(con(5), evaluator.evaluate(add(con(2), con(3)), vars));
        assertEquals(con(3), evaluator.evaluate(asg(con(3), var("x")), vars));
        assertThrows(RuntimeException.class, () -> {
            evaluator.evaluate(parse("Quit"), vars);
        });
    }
}