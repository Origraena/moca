package moca.graphs;

import moca.operators.LongOperatorPlus;
import moca.comparators.LongComparator;

import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexCollection;
import moca.graphs.vertices.ParentFunction;
import moca.graphs.edges.Edge;
import moca.graphs.edges.EdgeCollection;
import moca.graphs.edges.IllegalEdgeException;


import java.util.NoSuchElementException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;


public class GeoGraph extends Graph<Point,Long> {

	public GeoGraph(VertexCollection<Point> points, EdgeCollection<Long> edges) throws IllegalConstructionException {
		super(points,edges);
	}

	public GeoGraph(Graph<Point,Long> g) throws IllegalConstructionException {
		super(g);
	}

	public void addPoint(int x, int y) {
		add(new Point(x,y));
	}

	public void addEdge(int idU, int idV) throws NoSuchElementException, IllegalEdgeException {
		_edges.add(idU, idV, Point.euclidianDistance(get(idU),get(idV)));
	}

	public void addEdge(Vertex<Point> p, Vertex<Point> q) throws NoSuchElementException, IllegalEdgeException {
		addEdge(p.getID(),q.getID());
	}

	public ParentFunction<Point> Dijsktra(int root) {
		return super.Dijsktra(root,new Long(0),new LongOperatorPlus(),new LongComparator());
	}

	public ParentFunction<Point> AStar(int root, 
								   ArrayList<Vertex<Point> > ends, 
								   ArrayList<Long> heuristique) {
		return super.AStar(root,new Long(0), new LongOperatorPlus(), new LongComparator(), ends, heuristique);
	}

};

