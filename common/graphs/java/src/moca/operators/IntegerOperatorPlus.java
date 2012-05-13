package moca.operators;

public class IntegerOperatorPlus implements OperatorPlus1T<Integer> {

	public Integer exec(Integer b, Integer c) {
		return new Integer(b+c);
	}

};

