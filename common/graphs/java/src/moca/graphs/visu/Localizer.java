package moca.graphs.visu;

import java.util.NoSuchElementException;
import moca.graphs.Point;
import moca.graphs.Graph;

public interface Localizer {

	public void process(Graph graph);
	Point position(int vertexID) throws NoSuchElementException;
	int x(int vertexID) throws NoSuchElementException;
	int y(int vertexID) throws NoSuchElementException;

};

