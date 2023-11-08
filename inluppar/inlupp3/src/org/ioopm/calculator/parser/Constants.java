package org.ioopm.calculator.parser;

import java.util.HashMap;

import org.ioopm.calculator.ast.*;

public class Constants {
    public static final HashMap<String, NamedConstant> namedConstants = new HashMap<>();

    static {
        Constants.namedConstants.put("pi", new NamedConstant("pi", Math.PI));
        Constants.namedConstants.put("e", new NamedConstant("e", Math.E));
        Constants.namedConstants.put("g", new NamedConstant("g", 9.82));
        Constants.namedConstants.put("sqrt2", new NamedConstant("sqrt2", Math.sqrt(2)));
        Constants.namedConstants.put("Answer", new NamedConstant("Answer", 42.0));
        Constants.namedConstants.put("L", new NamedConstant("L", 6.022140857 * Math.pow(10, 23)));
    }
}