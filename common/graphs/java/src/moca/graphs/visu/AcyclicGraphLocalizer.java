package moca.graphs.visu;

import java.util.NoSuchElementException;
import java.util.ArrayList;
import java.util.Iterator;
import moca.graphs.Point;
import moca.graphs.Graph;
import moca.graphs.edges.NeighbourEdge;

public class AcyclicGraphLocalizer extends AbstractLocalizer implements Localizer {

	public AcyclicGraphLocalizer(Graph graph, int width, int height) {
		super();
		_width = width;
		_height = height;
		process(graph);
	}
	public AcyclicGraphLocalizer(int width, int height) {
		super();
		_width = width;
		_height = height;
	}

	public Point position(int vertexID) throws NoSuchElementException {
		if ((vertexID >= _positions.length) || (vertexID < 0))
			throw new NoSuchElementException();
		return _positions[vertexID];
	}

	public void process(Graph graph) {
		final int nbVertices = graph.getNbVertices();
		int i,j,id;
		boolean ended;
		boolean marked[] = new boolean[nbVertices];
		ArrayList<ArrayList<Integer> > positions = new ArrayList<ArrayList<Integer> >();
		Iterator<NeighbourEdge> iterator;
		ArrayList<Integer> previousList;
		int wfoot,hfoot;

		// sources
		positions.add(new ArrayList<Integer>());
		for (i = 0 ; i < nbVertices ; i++) {
			marked[i] = false;
			ended = false;
			for (j = 0 ; (j < nbVertices) && !ended ; j++) {
				if ((i != j) && (graph.isEdge(j,i)))
					ended = true;
			}
			if (j == nbVertices) {
				positions.get(0).add(new Integer(i));
				marked[i] = true;
			}
		}

		// all other vertices
		i = 1;
		previousList = positions.get(i-1);
		while (previousList.size() > 0) {
			positions.add(new ArrayList<Integer>());
			for (Integer previousVertex : previousList) {
				iterator = graph.neighbourIterator(previousVertex);
				while (iterator.hasNext()) {
					id = iterator.next().getIDV();
					if (marked[id] == false) {
						positions.get(i).add(new Integer(id));
						marked[id] = true;
					}
				}
			}
			previousList = positions.get(i);
			i++;
		}
		
		// process positions
		_positions = new Point[nbVertices];
		hfoot = _height / (positions.size() + 1);
		for (i = 0 ; i < positions.size() ; i++) {
			wfoot = _width / (positions.get(i).size() + 1);
			for (j = 0 ; j < positions.get(i).size() ; j++) {
				_positions[positions.get(i).get(j)] = 
					new Point((j + 1) * wfoot,_height - ((i + 1) * hfoot));
			}
		}
	}

	protected int _width,_height;
	protected Point _positions[];

};

