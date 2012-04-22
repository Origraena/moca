package moca.graphs.visu;

import java.util.NoSuchElementException;

public abstract class AbstractLocalizer implements Localizer {

	public int x(int vertexID) throws NoSuchElementException {
		return position(vertexID).x;
	}

	public int y(int vertexID) throws NoSuchElementException {
		return position(vertexID).y;
	}

};

