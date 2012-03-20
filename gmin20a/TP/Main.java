import java.util.ArrayList;

import moca.graphs.Echiquier;
import moca.graphs.GeoGraph;
import moca.graphs.Labyrinth;
import moca.graphs.Point;
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
			System.out.println("\nQuestion 1 : représentation du graphe de la figure 1\n");
			GeoGraph geograph = new GeoGraph(new VertexArrayList<Point>(), new UndirectedNeighboursLists<Long>());

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
			geograph.addEdge(0,2,3L);		// s -> v2
			geograph.addEdge(1,3,1L);		// v1 -> v3
			geograph.addEdge(1,4,2L);		// v1 -> v4
			geograph.addEdge(2,5,3L);		// v2 -> v5
			geograph.addEdge(2,4,3L);		// v2 -> v4
			geograph.addEdge(2,3,3L);		// v2 -> v3
			geograph.addEdge(3,6,4L);		// v3 -> v6
			geograph.addEdge(4,6,2L);		// v4 -> v6
			geograph.addEdge(4,7,4L);		// v4 -> v7
			geograph.addEdge(4,8,2L);		// v4 -> v8
			geograph.addEdge(5,8,3L);		// v5 -> v8
			geograph.addEdge(6,9,3L);		// v6 -> v9
			geograph.addEdge(7,9,2L);		// v7 -> v9
			geograph.addEdge(7,10,2L);		// v7 -> v10
			geograph.addEdge(8,10,3L);		// v8 -> v10
			geograph.addEdge(9,11,4L);		// v9 -> t
			geograph.addEdge(10,11,3L);		// v10 -> t
			
			System.out.println("Graphe : \n"+geograph);
			
			
			
			System.out.println("\n\nQuestions 2 et 3 : A* sur le graphe précédant\n");
			
			Vertex<Point> s = geograph.getVertex(0);
			Vertex<Point> t = geograph.getVertex(11);
			Long cost;
			ArrayList<Vertex<Point>> ends = new ArrayList<Vertex<Point>>();
			
			ends.add(geograph.getVertex(t.getID()));
			ParentFunction<Point> parent = geograph.AStar(s.getID(),ends,null);
			System.out.println("AStar depuis la racine "+s.getID());
			for (int i = 0 ; i < geograph.getNbVertices() ; i++) {
				t = geograph.getVertex(i);
				cost = new Long(0);
				if (parent.getParent(t) == null)
					System.out.println("pas besoin de parcourir " + t.getID());
				else {
					while (t != s) {
						System.out.print("\t"+t.getID()+" <-- ");
						cost += geograph.getEdgeValue(parent.getParent(t),t);
						t = parent.getParent(t);
					}
					System.out.print("racine    ("+cost+")");
					System.out.println(" ");
				}
			}
			
			
			
			
			System.out.println("\n\nQuestion 4 : échiquier");
			Echiquier echiquier = new Echiquier(10, 10, new Point(4,0), new Point(4, 9));
			
			ends = new ArrayList<Vertex<Point>>();
			ends.add(echiquier.getDestination());
			ArrayList<Long> heuristique = new ArrayList<Long>(echiquier.getNbVertices());
			for (int i = 0 ; i < echiquier.getNbVertices() ; i++) {
				Long min = new Long(-1);
				for (Vertex<Point> q : ends) {
					if ((min < 0) || (Point.euclidianDistance(q.getValue(),echiquier.get(i)) < min))
						min = Point.euclidianDistance(q.getValue(),echiquier.get(i));
				}
				heuristique.add(min);
			}
			
			ParentFunction<Point> geographParent = echiquier.AStar(echiquier.getNumSource(),ends,heuristique);
		
			VertexArrayListUnaryFunction<Point> parentFunction = new VertexArrayListUnaryFunction<Point>(echiquier.getNbVertices());
			parentFunction.set(echiquier.getSource()," >>");
			Vertex<Point> current;
			for (Vertex<Point> q : ends) {
				parentFunction.set(q," X");
				current = geographParent.getParent(q);
				while ((current != null) && (current != echiquier.getSource())) {
					parentFunction.set(current," .");
					current = geographParent.getParent(current);
				}
			}
			System.out.println("A* d'un pion dans un échiquier de 10 sur 10 :\n"+new Echiquier(echiquier,parentFunction));
			
			
			
			
			
			System.out.println("\n\nQuestion 5 : labyrinthe");
			
			GeoGraph geograph2 = new GeoGraph(new VertexArrayList<Point>(), new UndirectedNeighboursLists<Long>());
			Labyrinth l = new Labyrinth(geograph2, new VertexIdentityFunction<Point>());
			l.fromFile("labyrinthe.txt");
			System.out.println("Labyrinthe d'entree :\n"+l);
			
			
			ends = new ArrayList<Vertex<Point>>();
			ends.add(l.getDestination());
			heuristique = new ArrayList<Long>(geograph2.getNbVertices());
			for (int i = 0 ; i < geograph2.getNbVertices() ; i++) {
				Long min = new Long(-1);
				for (Vertex<Point> q : ends) {
					if ((min < 0) || (Point.euclidianDistance(q.getValue(),geograph2.get(i)) < min))
						min = Point.euclidianDistance(q.getValue(),geograph2.get(i));
				}
				heuristique.add(min);
			}
			
			geographParent = geograph2.AStar(l.getNumSource(),ends,heuristique);
		
			parentFunction = new VertexArrayListUnaryFunction<Point>(geograph2.getNbVertices());
			parentFunction.set(l.getSource()," >>");
			
			for (Vertex<Point> q : ends) {
				parentFunction.set(q," X");
				current = geographParent.getParent(q);
				while ((current != null) && (current != l.getSource())) {
					parentFunction.set(current," .");
					current = geographParent.getParent(current);
				}
			}
			System.out.println("Chemin emprunté jusqu'à la sortie :\n"+new Labyrinth(geograph2,4,' ','|','-','+',parentFunction));

		}
		catch (Exception e) {
			System.out.println(e);
		}

	}
};

