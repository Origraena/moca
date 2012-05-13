package moca.operators;

/**
 * Interface which represents the operator+.
 * The exec method must take B and C as parameters, and return their sums as A type.
 */
public interface OperatorPlus<A,B,C> {
	A exec(B b, C c);
};

