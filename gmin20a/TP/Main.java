import java.util.ArrayList;
import java.util.Iterator;
import java.util.Random;

import moca.graphs.BipartedGraph;
import moca.graphs.Echiquier;
import moca.graphs.GeoGraph;
import moca.graphs.Graph;
import moca.graphs.Labyrinth;
import moca.graphs.Point;
import moca.graphs.edges.Edge;
import moca.graphs.edges.IllegalEdgeException;
import moca.graphs.edges.NeighbourEdge;
import moca.graphs.edges.NeighboursLists;
import moca.graphs.edges.UndirectedNeighboursLists;
import moca.graphs.vertices.ParentFunction;
import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexArrayList;
import moca.graphs.vertices.VertexArrayListUnaryFunction;
import moca.graphs.vertices.VertexIdentityFunction;



public class Main {
	public static void main(String args[]) {
		try {
			System.out.println("\nQuestion 1 : representation d'un graphe\n");
			GeoGraph geograph = new GeoGraph(new VertexArrayList<Point>(), new NeighboursLists<Long>());

			/* Vertices */
			geograph.addPoint(0,1);		// s
			geograph.addPoint(1,0);		// v1
			geograph.addPoint(1,2);		// v2
			geograph.addPoint(2,0);		// v3
			geograph.addPoint(2,1);		// v4
			geograph.addPoint(2,2);		// v5
			geograph.addPoint(3,0);		// v6
			geograph.addPoint(4,1);		// v7
			geograph.addPoint(3,2);		// v8
			geograph.addPoint(4,0);		// v9
			geograph.addPoint(4,2);		// v10 
			geograph.addPoint(5,1);		// t

			/* Edges */
			geograph.addEdge(0,1,7L);		// s -> v1
			geograph.addEdge(1,0,7L);
			geograph.addEdge(0,2,3L);		// s -> v2
			geograph.addEdge(2,0,3L);
			geograph.addEdge(1,3,1L);		// v1 -> v3
			geograph.addEdge(3,1,1L);
			geograph.addEdge(1,5,2L);		// v1 -> v4
			geograph.addEdge(5,1,2L);
			geograph.addEdge(2,5,3L);		// v2 -> v5
			geograph.addEdge(5,2,3L);
			geograph.addEdge(2,4,3L);		// v2 -> v4
			geograph.addEdge(4,2,3L);
			geograph.addEdge(2,3,3L);		// v2 -> v3
			geograph.addEdge(3,2,3L);
			geograph.addEdge(3,6,4L);		// v3 -> v6
			geograph.addEdge(6,3,4L);
			geograph.addEdge(4,6,2L);		// v4 -> v6
			geograph.addEdge(6,4,2L);
			geograph.addEdge(4,7,4L);		// v4 -> v7
			geograph.addEdge(7,4,4L);
			geograph.addEdge(4,8,2L);		// v4 -> v8
			geograph.addEdge(8,4,2L);
			geograph.addEdge(5,8,3L);		// v5 -> v8
			geograph.addEdge(8,5,3L);
			geograph.addEdge(6,9,3L);		// v6 -> v9
			geograph.addEdge(9,6,3L);
			geograph.addEdge(7,9,2L);		// v7 -> v9
			geograph.addEdge(9,7,2L);
			geograph.addEdge(7,10,2L);		// v7 -> v10
			geograph.addEdge(10,7,2L);
			geograph.addEdge(8,10,3L);		// v8 -> v10
			geograph.addEdge(10,8,3L);
			geograph.addEdge(9,11,4L);		// v9 -> t
			geograph.addEdge(11,9,4L);
			geograph.addEdge(10,11,3L);		// v10 -> t
			geograph.addEdge(11,10,3L);
			
			// TODO print graph
			System.out.println("Graphe : \n"+geograph);
			
			
			
			System.out.println("\n\nQuestions 2 et 3 : A*\n");
			
			Vertex<Point> s = geograph.getVertex(0);
			Vertex<Point> t = geograph.getVertex(11);
			Long cost;
			ArrayList<Vertex<Point> > ends = new ArrayList<Vertex<Point> >();
			ends.add(geograph.getVertex(11));
			ParentFunction<Point> parent = geograph.AStar(0,ends,null);
			System.out.println("AStar from root 0");
			for (int i = 0 ; i < geograph.getNbVertices() ; i++) {
				t = geograph.getVertex(i);
				cost = new Long(0);
				if (parent.getParent(t) == null)
					System.out.println("no need to go to " + t.getID());
				else {
					while (t != s) {
						System.out.print(""+t.getID()+" <-- ");
						cost += geograph.getEdgeValue(parent.getParent(t),t);
						t = parent.getParent(t);
					}
					System.out.print("root    ("+cost+")");
					System.out.println(" ");
				}
			}
			
			
			
			// TODO echiquier
			System.out.println("\n\nQuestion 4 : echiquier");
			Echiquier echiquier = new Echiquier(10, 10, new Point(4,0), new Point(4, 9));
			System.out.println(echiquier);
			
			ArrayList<Vertex<Point> > geographEnds = new ArrayList<Vertex<Point>>();
			geographEnds.add(echiquier.getGraph().getVertex(echiquier.getDestination()));
			ArrayList<Long> heuristique = new ArrayList<Long>(echiquier.getGraph().getNbVertices());
			for (int i = 0 ; i < echiquier.getGraph().getNbVertices() ; i++) {
				Long min = new Long(-1);
				for (Vertex<Point> q : geographEnds) {
					if ((min < 0) || (Point.euclidianDistance(q.getValue(),echiquier.getGraph().get(i)) < min))
						min = Point.euclidianDistance(q.getValue(),echiquier.getGraph().get(i));
				}
				heuristique.add(min);
			}
			
			ParentFunction<Point> geographParent = echiquier.getGraph().AStar(echiquier.getSource(),geographEnds,heuristique);
		
			VertexArrayListUnaryFunction<Point> parentFunction = new VertexArrayListUnaryFunction<Point>(echiquier.getGraph().getNbVertices());
			parentFunction.set(echiquier.getGraph().getVertex(echiquier.getSource())," >>");
			Vertex<Point> current;
			for (Vertex<Point> q : geographEnds) {
				parentFunction.set(q," X");
				current = geographParent.getParent(q);
				while ((current != null) && (current != echiquier.getGraph().getVertex(echiquier.getSource()))) {
					parentFunction.set(current," .");
					current = geographParent.getParent(current);
				}
			}
			System.out.println("A star :\n"+new Echiquier(echiquier,parentFunction));
			
			
			
			
			
			System.out.println("\n\nQuestion 5 : labyrinthe");
			
			GeoGraph geograph2 = new GeoGraph(new VertexArrayList<Point>(), new NeighboursLists<Long>());
			Labyrinth l = new Labyrinth(geograph2,new VertexIdentityFunction<Point>());
			l.fromFile("labyrinthe.txt");
			System.out.println("Labyrinthe d'entree :\n"+l);
			
			
			geographEnds = new ArrayList<Vertex<Point>>();
			geographEnds.add(geograph2.getVertex(l.getDestination()));
			heuristique = new ArrayList<Long>(geograph2.getNbVertices());
			for (int i = 0 ; i < geograph2.getNbVertices() ; i++) {
				Long min = new Long(-1);
				for (Vertex<Point> q : geographEnds) {
					if ((min < 0) || (Point.euclidianDistance(q.getValue(),geograph2.get(i)) < min))
						min = Point.euclidianDistance(q.getValue(),geograph2.get(i));
				}
				heuristique.add(min);
			}
			
			geographParent = geograph2.AStar(l.getSource(),geographEnds,heuristique);
		
			parentFunction = new VertexArrayListUnaryFunction<Point>(geograph2.getNbVertices());
			parentFunction.set(geograph2.getVertex(l.getSource())," >>");
			
			for (Vertex<Point> q : geographEnds) {
				parentFunction.set(q," X");
				current = geographParent.getParent(q);
				while ((current != null) && (current != geograph2.getVertex(l.getSource()))) {
					parentFunction.set(current," .");
					current = geographParent.getParent(current);
				}
			}
			System.out.println("Labyrinthe emprunte :\n"+new Labyrinth(geograph2,4,' ','|','-','+',parentFunction));

		}
		catch (Exception e) {
			System.out.println(e);
		}

	}
};

