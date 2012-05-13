package moca.graphs;

import moca.operators.LongOperatorPlus;
import moca.comparators.LongComparator;

import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexCollection;
import moca.graphs.vertices.ParentFunction;
import moca.graphs.edges.Edge;
import moca.graphs.edges.EdgeCollection;
import moca.graphs.edges.IllegalEdgeException;
import moca.graphs.edges.NeighbourEdge;

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
								   Heuristique<Long> heuristique) {
		return super.AStar(root,new Long(0), new LongOperatorPlus(), new LongComparator(), ends, heuristique);
	}
	
	public GeoGraph clone() {
		try {
			VertexCollection<Point> vertices = _vertices.getClass().newInstance();
			EdgeCollection<Long> edges = _edges.getClass().newInstance();
			GeoGraph graph = new GeoGraph(vertices,edges);
			Edge<Long> edge = null;
			for (Point v : this)
				graph.addVertex(v);
			for (Iterator<Edge<Long> > e = edgeIterator() ; e.hasNext() ; ) {
				edge = e.next();
				graph.addEdge(edge.getIDU(),edge.getIDV(),edge.getValue());
			}
			return graph;
		}
		catch (Exception e) {
			return null;
		}
	}
	
	public void printParents(ParentFunction<Point> parent, Vertex<Point> s) {
		Long cost;
		Vertex<Point> t;
		for (int i = 0 ; i < getNbVertices() ; i++) {
			t = getVertex(i);
			cost = new Long(0);
			if (parent.getParent(t) == null)
				;//System.out.println("pas besoin de parcourir " + t.getID());
			else {
				while (t != s) {
					System.out.print("\t"+t.getID()+" <-- ");
					cost += getEdgeValue(parent.getParent(t),t);
					t = parent.getParent(t);
				}
				System.out.print("racine    ("+cost+")");
				System.out.println(" ");
			}
		}
	}
	
	public void printWay(ParentFunction<Point> parent, Vertex<Point> s, Vertex<Point> t) {
		Long cost;
		Vertex<Point> tmp = t;
		cost = new Long(0);
		if (parent.getParent(tmp) == null)
			System.out.println("Aucun chemin trouvé jusqu'a " + t.getID());
		else {
			while (tmp != s) {
				System.out.print("\t"+tmp.getID()+" <-- ");
				cost += getEdgeValue(parent.getParent(tmp),tmp);
				tmp = parent.getParent(tmp);
			}
			System.out.print("racine    ("+cost+")");
			System.out.println(" ");
		}
	}
	
};

