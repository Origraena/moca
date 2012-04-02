package moca.comparators;

import java.util.Comparator;

public class IntegerComparator implements Comparator<Integer> {

	public int compare(Integer i, Integer j) {
		if (i.intValue() < j.intValue())
			return -1;
		else if (i.intValue() == j.intValue())
			return 0;
		else
			return 1;
	}

};

