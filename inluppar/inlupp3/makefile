all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java

run:
	java -cp classes org.ioopm.calculator.Calculator

comptest:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Test.java

test: comptest
	java -cp classes org.ioopm.calculator.Test

clean:
	rm -rf classes