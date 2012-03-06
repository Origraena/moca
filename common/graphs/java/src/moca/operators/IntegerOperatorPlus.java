package moca.operators;

public class IntegerOperatorPlus implements OperatorPlus<Integer,Integer,Integer> {

	public Integer exec(Integer b, Integer c) {
		return b+c;
	}

};

