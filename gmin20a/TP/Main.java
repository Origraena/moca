
import java.util.ArrayList;

import moca.graphs.Heuristique;
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
			geograph.addPoint(25,125);		// s
			geograph.addPoint(100,25);		// v1
			geograph.addPoint(100,225);		// v2
			geograph.addPoint(200,25);		// v3
			geograph.addPoint(200,125);		// v4
			geograph.addPoint(200,225);		// v5
			geograph.addPoint(300,25);		// v6
			geograph.addPoint(400,125);		// v7
			geograph.addPoint(300,225);		// v8
			geograph.addPoint(400,25);		// v9
			geograph.addPoint(400,225);		// v10 
			geograph.addPoint(475,125);		// t

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
			ArrayList<Vertex<Point>> ends = new ArrayList<Vertex<Point>>();
			ends.add(geograph.getVertex(t.getID()));
			Heuristique<Long> h = new Heuristique<Long>();
			h.setEuclidianDistance(geograph, ends);
			
			ParentFunction<Point> parent = geograph.AStar(s.getID(),ends,h);
			System.out.println("AStar depuis la racine "+s.getID()+" avec une heuristique de distance euclidienne. Chemin obtenu :");
			geograph.printWay(parent, s, t);
			parent = geograph.AStar(s.getID(),ends,null);
			System.out.println("Dikjstra depuis la racine "+s.getID()+". Chemin obtenu :");
			geograph.printWay(parent, s, t);
			
			
			
			System.out.println("\n\nQuestion 4 : échiquier");
			Echiquier echiquier = new Echiquier(10, 10, new Point(4,0), new Point(4, 9));
			echiquier.createEdgesForApawn();
			ends = new ArrayList<Vertex<Point>>();
			ends.add(echiquier.getDestination());
			h = new Heuristique<Long>(echiquier.getNbVertices());
			h.setEuclidianDistance(echiquier.getGraph(), ends);
			ParentFunction<Point> geographParent = echiquier.AStar(echiquier.getNumSource(),ends,h);
			echiquier.computeVertexDrawFunction(geographParent);
			System.out.println("A* d'un pion dans un échiquier de 10 sur 10 :\n"+echiquier);
			
			
			
			
			System.out.println("\n\nQuestion 5 : labyrinthe");
			
			GeoGraph geograph2 = new GeoGraph(new VertexArrayList<Point>(), new UndirectedNeighboursLists<Long>());
			Labyrinth l = new Labyrinth(geograph2, new VertexIdentityFunction<Point>());
			l.fromFile("labyrinthe.txt");
			System.out.println("Labyrinthe d'entree :\n"+l);
			
			
			ends = new ArrayList<Vertex<Point>>();
			ends.add(l.getDestination());
			h = new Heuristique<Long>(geograph2.getNbVertices());
			h.setEuclidianDistance(geograph2, ends);
			
			geographParent = geograph2.AStar(l.getNumSource(),ends,h);
			l.computeVertexDrawFunction(geographParent);
			System.out.println("Chemin emprunté jusqu'à la sortie :\n"+l);

		}
		catch (Exception e) {
			System.out.println(e);
		}

	}
};

