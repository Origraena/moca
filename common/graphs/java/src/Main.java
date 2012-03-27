import moca.*;
import moca.graphs.*;
import moca.graphs.edges.*;
import moca.graphs.vertices.*;
import java.lang.Exception;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.Random;

public class Main {
	public static void main(String args[]) {
		try {
			Graph<Integer,Integer> graph = new Graph<Integer,Integer>(new VertexArrayList<Integer>(), new NeighboursLists<Integer>());
			for (int i = 0 ; i < 10 ; i++) {
				graph.addVertex(new Integer(i));
			}
			for (int i = 1 ; i < 9 ; i++) {
				graph.addEdge(i,i+1,new Integer(i));
				graph.addEdge(i-1,i+1,new Integer(i+1));
			}

			System.out.println("Vertex values");
			for (Integer vertex : graph) {
				System.out.println(vertex);
			}

			System.out.println("Edges");
			for (Iterator<Edge<Integer> > iterator = graph.edgeIterator() ; iterator.hasNext() ; ) {
				System.out.println(iterator.next());
			}
	
			System.out.println("DFS");
			for (Graph<Integer,Integer>.WalkIterator dfs = graph.DFSIterator() ; dfs.hasNext() ; ) {
				System.out.println(dfs.next().getID());
			}

			System.out.println("BFS");
			for (Graph<Integer,Integer>.WalkIterator bfs = graph.BFSIterator() ; bfs.hasNext() ; ) {
				System.out.println(bfs.next().getID());
			}
	
			System.out.println("BIPARTED GRAPH");
			BipartedGraph<Integer,Integer> bigraph = new BipartedGraph<Integer,Integer>(new VertexArrayList<Integer>(), new VertexArrayList<Integer>(), new NeighboursLists<Integer>());
			
			bigraph.addInSecondSet(23);
			for (int i = 0 ; i < 10 ; i++)
				bigraph.addInFirstSet(i);
			for (int i = 10 ; i < 15 ; i++)
				bigraph.addInSecondSet(i);

			System.out.println("Vertices :");
			for (Integer v : bigraph) 
				System.out.println(v);

			for (Iterator<Vertex<Integer> > iterator = bigraph.vertexIterator() ; iterator.hasNext() ;)
				System.out.println(iterator.next());

			for (int i = 0 ; i < 10 ; i++) {
				for (int j = 10 ; j < 15 ; j++) {
					bigraph.addEdge(i,j,(i+1)*j);
				}
			}

			try {
				bigraph.addEdge(2,2,10);
			}
			catch (IllegalEdgeException e) {
				System.out.println(e);
			}
		
			System.out.println("Edges : ");
			for (Iterator<Edge<Integer> > edgeIterator = bigraph.edgeIterator() ; edgeIterator.hasNext() ; )
				System.out.println(edgeIterator.next());



			System.out.println("Specific Graph");
			System.out.println("First(A,A);Second(A,B,C,D);Third(B)");
			BipartedGraph<String,Integer> g = new BipartedGraph<String,Integer>(new VertexArrayList<String>(), new VertexArrayList<String>(), new UndirectedNeighboursLists<Integer>());
			g.addInFirstSet("First");
			g.addInFirstSet("Second");
			g.addInFirstSet("Third");
			g.addInSecondSet("A");
			g.addInSecondSet("B");
			g.addInSecondSet("C");
			g.addInSecondSet("D");
			g.addEdge(0,3,0);
			g.addEdge(0,3,1);
			g.addEdge(1,3,0);
			g.addEdge(1,4,1);
			g.addEdge(1,5,2);
			g.addEdge(1,6,3);
			g.addEdge(2,4,0);
			
			System.out.println("vertices :");
			for (Iterator<Vertex<String> > i = g.vertexIterator() ; i.hasNext() ; System.out.println(i.next()));
			System.out.println("values :");
			for (String s : g) System.out.println(s);
			System.out.println("edges :");
			for (Iterator<Edge<Integer> > i = g.edgeIterator() ; i.hasNext() ; ) {
				Edge<Integer> e = i.next();
				System.out.println(g.get(e.getIDU()) + " --> " + g.get(e.getIDV()) + " = " + e.getValue());
			}
			for (Iterator<NeighbourEdge<Integer> > e = g.neighbourIterator(0) ; e.hasNext() ;)
				System.out.println(e.next());
		}
		catch (Exception e) {
			System.out.println(e);
		}


		try {
			System.out.println("\n\n\nGEOGRAPH");
			GeoGraph geograph = new GeoGraph(new VertexArrayList<Point>(), new NeighboursLists<Long>());

			/* Points */
			geograph.addPoint(7,1);		// 0
			geograph.addPoint(7,4);		// 1
			geograph.addPoint(9,3);		// 2
			geograph.addPoint(12,3);	// 3
			geograph.addPoint(13,4);	// 4
			geograph.addPoint(9,5);		// 5
			geograph.addPoint(10,7);	// 6
			geograph.addPoint(12,7);	// 7
			geograph.addPoint(6,7);		// 8
			geograph.addPoint(7,9);		// 9
			geograph.addPoint(4,8);		// 10 
			geograph.addPoint(4,6);		// 11
			geograph.addPoint(2,8);		// 12
			geograph.addPoint(2,10);	// 13
			geograph.addPoint(5,10);	// 14

			/* Edges */
			geograph.addEdge(0,1);		// 0
			geograph.addEdge(1,2);		// 1
			geograph.addEdge(1,5);
			geograph.addEdge(1,11);
			geograph.addEdge(2,1);		// 2
			geograph.addEdge(2,3);
			geograph.addEdge(3,2);		// 3
			geograph.addEdge(3,4);
			geograph.addEdge(4,3);		// 4
			geograph.addEdge(4,5);
			geograph.addEdge(5,1);		// 5
			geograph.addEdge(5,4);
			geograph.addEdge(5,6);
			geograph.addEdge(5,7);
			geograph.addEdge(5,8);
			geograph.addEdge(6,5);		// 6
			geograph.addEdge(6,7);
			geograph.addEdge(6,8);
			geograph.addEdge(7,5);		// 7
			geograph.addEdge(7,6);
			geograph.addEdge(7,9);
			geograph.addEdge(8,6);		// 8
			geograph.addEdge(8,9);
			geograph.addEdge(8,5);
			geograph.addEdge(8,11);
			geograph.addEdge(9,7);		// 9
			geograph.addEdge(9,8);
			geograph.addEdge(10,11);	// 10
			geograph.addEdge(10,14);
			geograph.addEdge(11,8);		// 11
			geograph.addEdge(11,1);
			geograph.addEdge(11,10);
			geograph.addEdge(11,12);
			geograph.addEdge(12,11);	// 12
			geograph.addEdge(12,13);
			geograph.addEdge(13,14);	// 13
			geograph.addEdge(13,12);
			geograph.addEdge(14,10);	// 14
			geograph.addEdge(14,13);
	
			Vertex<Point> root = geograph.getVertex(0);
			Vertex<Point> p = null;
			ParentFunction<Point> parent = geograph.Dijsktra(0);
			Long cost = new Long(0);
			System.out.println("Dijkstra from root 0");
			for (int i = 0 ; i < geograph.getNbVertices() ; i++) {
				p = geograph.getVertex(i);
				cost = new Long(0);
				while (p != root) {
					System.out.print(""+p.getID()+" <-- ");
					cost += geograph.getEdgeValue(parent.getParent(p),p);
					p = parent.getParent(p);
				}
				System.out.print("root    ("+cost+")");
				System.out.println(" ");
			}

			root = geograph.getVertex(0);
			ArrayList<Vertex<Point> > ends = new ArrayList<Vertex<Point> >();
			ends.add(geograph.getVertex(7));
			ends.add(geograph.getVertex(9));
			ends.add(geograph.getVertex(12));
			parent = geograph.AStar(0,ends,null);
			System.out.println("AStar from root 0");
			for (int i = 0 ; i < geograph.getNbVertices() ; i++) {
				p = geograph.getVertex(i);
				cost = new Long(0);
				if (parent.getParent(p) == null)
					System.out.println("no need to go to " + p.getID());
				else {
					while (p != root) {
						System.out.print(""+p.getID()+" <-- ");
						cost += geograph.getEdgeValue(parent.getParent(p),p);
						p = parent.getParent(p);
					}
					System.out.print("root    ("+cost+")");
					System.out.println(" ");
				}
			}


			GeoGraph geograph2 = new GeoGraph(new VertexArrayList<Point>(), new UndirectedNeighboursLists<Long>());

			// points
			for (int i = 0 ; i < 9 ; i++) {
				for (int j = 0 ; j < 10 ; j++) {
					geograph2.addPoint(j,i);
				}
			}

			// line 0
			for (int i = 1 ; i < 10 ; i++)
				geograph2.addEdge(i-1,i);
			geograph2.addEdge(2,12);
			geograph2.addEdge(5,15);
			geograph2.addEdge(9,19);

			// line 1
			geograph2.addEdge(10,11);
			geograph2.addEdge(13,14);
			geograph2.addEdge(16,17);
			geograph2.addEdge(17,18);
			geograph2.addEdge(10,20);
			geograph2.addEdge(12,22);
			geograph2.addEdge(15,25);
			geograph2.addEdge(16,26);
			geograph2.addEdge(19,29);

			// line 2
			geograph2.addEdge(20,30);
			geograph2.addEdge(21,31);
			geograph2.addEdge(21,22);
			geograph2.addEdge(22,23);
			geograph2.addEdge(23,24);
			geograph2.addEdge(24,25);
			geograph2.addEdge(24,34);
			geograph2.addEdge(25,26);
			geograph2.addEdge(26,36);
			geograph2.addEdge(27,28);
			geograph2.addEdge(28,29);
					
			// line 3
			geograph2.addEdge(30,31);
			geograph2.addEdge(30,40);
			geograph2.addEdge(31,32);
			geograph2.addEdge(32,33);
			geograph2.addEdge(34,35);
			geograph2.addEdge(34,44);
			geograph2.addEdge(35,45);
			geograph2.addEdge(36,37);
			geograph2.addEdge(36,46);
			geograph2.addEdge(37,38);
			geograph2.addEdge(38,39);

			// line 4
			geograph2.addEdge(40,41);
			geograph2.addEdge(41,42);
			geograph2.addEdge(42,43);
			geograph2.addEdge(43,44);
			geograph2.addEdge(46,47);
			geograph2.addEdge(47,48);
			geograph2.addEdge(40,50);
			geograph2.addEdge(45,55);
			geograph2.addEdge(49,59);

			// line 5
			geograph2.addEdge(51,52);
			geograph2.addEdge(52,53);
			geograph2.addEdge(54,55);
			geograph2.addEdge(56,57);
			geograph2.addEdge(57,58);
			geograph2.addEdge(58,59);
			geograph2.addEdge(50,60);
			geograph2.addEdge(51,61);
			geograph2.addEdge(53,63);
			geograph2.addEdge(54,64);
			geograph2.addEdge(59,69);

			// line 6
			geograph2.addEdge(61,62);
			geograph2.addEdge(64,65);
			geograph2.addEdge(65,66);
			geograph2.addEdge(66,67);
			geograph2.addEdge(67,68);
			geograph2.addEdge(68,69);
			geograph2.addEdge(60,70);
			geograph2.addEdge(62,72);
			geograph2.addEdge(63,73);
			geograph2.addEdge(65,75);
			geograph2.addEdge(66,76);
			geograph2.addEdge(67,77);
			geograph2.addEdge(69,79);

			// line 7
			geograph2.addEdge(70,71);
			geograph2.addEdge(73,74);
			geograph2.addEdge(75,76);
			geograph2.addEdge(78,79);
			geograph2.addEdge(72,82);
			geograph2.addEdge(74,84);
			geograph2.addEdge(77,87);
			geograph2.addEdge(79,89);

			// line 8
			for (int i = 80 ; i < 89 ; i++) {
				if ((i != 82) && (i != 88))
					geograph2.addEdge(i,i+1);
			}

			// WALKITERATORS
			int zzz = 0;
			System.out.println("DFS");
			VertexArrayListUnaryFunction<Point> dfsfunction = new VertexArrayListUnaryFunction<Point>(geograph2.getNbVertices());
			for (Graph<Point,Long>.WalkIterator dfs = geograph2.DFSIterator() ; dfs.hasNext() ; zzz++) {
				dfsfunction.set(dfs.next(),String.valueOf(zzz));
			}

			System.out.println(" ");
			
			System.out.println("BFS");
			zzz = 0;
			VertexArrayListUnaryFunction<Point> bfsfunction = new VertexArrayListUnaryFunction<Point>(geograph2.getNbVertices());
			for (Graph<Point,Long>.WalkIterator bfs = geograph2.BFSIterator() ; bfs.hasNext() ; zzz++) {
				bfsfunction.set(bfs.next(),String.valueOf(zzz));
			}

			System.out.println(" ");
			System.out.println(" ");
		
			System.out.println("LABYRINTH");
			System.out.println(" ");
			System.out.println("classic");
			System.out.println(new Labyrinth(geograph2,new VertexIdentityFunction<Point>()));
			System.out.println("bfs");
			System.out.println(new Labyrinth(geograph2,bfsfunction));
			System.out.println("dfs");
			System.out.println(new Labyrinth(geograph2,dfsfunction));
			ParentFunction<Point> geographParent = geograph2.Dijsktra(0);
	
	
			VertexArrayListUnaryFunction<Point> parentFunction = new VertexArrayListUnaryFunction<Point>(geograph2.getNbVertices());
			Vertex<Point> current = geograph2.getVertex(88);
			parentFunction.set(current," X");
			parentFunction.set(geograph2.getVertex(0)," O");
			current = geographParent.getParent(current);
			while (current != geograph2.getVertex(0)) {
				parentFunction.set(current," .");
				current = geographParent.getParent(current);
			}
			System.out.println(new Labyrinth(geograph2,parentFunction));

			parentFunction = new VertexArrayListUnaryFunction<Point>(geograph2.getNbVertices());
			current = geograph2.getVertex(71);
			parentFunction.set(current," X");
			parentFunction.set(geograph2.getVertex(0)," O");
			current = geographParent.getParent(current);
			while (current != geograph2.getVertex(0)) {
				parentFunction.set(current," .");
				current = geographParent.getParent(current);
			}
			System.out.println(new Labyrinth(geograph2,parentFunction));



			/* AStar */
			ArrayList<Vertex<Point> > geographEnds = new ArrayList<Vertex<Point> >();
			geographEnds.add(geograph2.getVertex(89));
			geographEnds.add(geograph2.getVertex(80));
			geographEnds.add(geograph2.getVertex(83));
			geographEnds.add(geograph2.getVertex(56));
			geographEnds.add(geograph2.getVertex(88));
			Heuristique<Long> heuristique = new Heuristique<Long>(geograph2.getNbVertices());
			heuristique.setEuclidianDistance(geograph2, geographEnds);
			
			geographParent = geograph2.AStar(0,geographEnds,heuristique);
			System.out.println("AStar ended!");
		
		
			parentFunction = new VertexArrayListUnaryFunction<Point>(geograph2.getNbVertices());
			parentFunction.set(geograph2.getVertex(0)," >>");
			for (Vertex<Point> q : geographEnds) {
				parentFunction.set(q," X");
				current = geographParent.getParent(q);
				while ((current != null) && (current != geograph2.getVertex(0))) {
					parentFunction.set(current," .");
					current = geographParent.getParent(current);
				}
			}
			System.out.println(new Labyrinth(geograph2,4,' ','|','-','+',parentFunction));


			/* Auto Generation */
			System.out.println("Autogenerated labyrinth");
			int imax = 9;
			int jmax = 9;
			if (args.length >= 2) {
				imax = Integer.valueOf(args[0]);
				jmax = Integer.valueOf(args[1]);
			}
			GeoGraph labyrinth = new GeoGraph(new VertexArrayList<Point>(),new UndirectedNeighboursLists<Long>());
			for (int i = 0 ; i <= jmax ; i++) {
				for (int j = 0 ; j <= imax ; j++) {
					labyrinth.addPoint(j,i);
				}
			}
			ArrayList<Vertex<Point> > labyrinthExits = new ArrayList<Vertex<Point> >();
			for (int i = 0 ; i <= imax ; i++)
				labyrinthExits.add(labyrinth.getVertex((jmax)*(imax+1)+i));
			boolean wayExist = false;
			long seed = System.currentTimeMillis();
					// non unicity of shortest path : 1331152844653L
					// nice labyrinth : 1331152117907L
			Random rand = new Random(seed);
			int x = 0;
			int y = 0;
			Iterator<Vertex<Point> > labyrinthBFS;
			while (!wayExist) {
				while ((x == y) || (labyrinth.isEdge(x,y))) {
					x = rand.nextInt(labyrinth.getNbVertices()-1);//  %%%%%labyrinth.getNbVertices();
					if ((rand.nextInt(2) == 0) && (x - ((x / (imax+1)) * (imax+1))  != imax))
						y = x+1;
					else if (x < (imax+1)*jmax)
						y = x+imax+1;
					else
						y = x;
				}
				//System.out.println(x+" "+y);
				labyrinth.addEdge(x,y);
				labyrinthBFS = labyrinth.BFSIterator(0);
				while (labyrinthBFS.hasNext()) {
					if (labyrinthBFS.next().getID() >= (imax+1)*(jmax))
						wayExist = true;
				}
			}

//			System.out.println(new Labyrinth(labyrinth,new VertexIdentityFunction<Point>()));
			heuristique = new Heuristique<Long>(labyrinth.getNbVertices());
			heuristique.setEuclidianDistance(labyrinth, labyrinthExits);
			
			
			geographParent = labyrinth.AStar(0,labyrinthExits,heuristique);
			parentFunction = new VertexArrayListUnaryFunction<Point>(labyrinth.getNbVertices());
			parentFunction.set(labyrinth.getVertex(0)," >");
			long costL = 0;
			for (Vertex<Point> q : labyrinthExits) {
				parentFunction.set(q," X");
				current = geographParent.getParent(q);
				if (current != null)
					costL = labyrinth.getEdgeValue(current,q);
				while ((current != null) && (current != labyrinth.getVertex(0))) {
					parentFunction.set(current," .");
					costL += labyrinth.getEdgeValue(geographParent.getParent(current),current);
					current = geographParent.getParent(current);
				}
			}
			System.out.println(new Labyrinth(labyrinth,4,' ','|','-','+',parentFunction));
			System.out.println("Shortest path = "+costL);
			System.out.println("Seed : "+seed);
			
			long minCost = -1;
			geographParent = labyrinth.Dijsktra(0);
			for (Vertex<Point> q : labyrinthExits) {
				costL = -1;
				parentFunction.set(q," X");
				current = geographParent.getParent(q);
				if (current != null)
					costL = labyrinth.getEdgeValue(current,q);
				while ((current != null) && (current != labyrinth.getVertex(0))) {
					parentFunction.set(current," .");
					costL += labyrinth.getEdgeValue(geographParent.getParent(current),current);
					current = geographParent.getParent(current);
				}
				System.out.println("Shortest path to "+q.getID()+" = "+costL);
				if ((minCost < 0) || ((costL > 0) && (minCost > costL)))
					minCost = costL;
			}
			System.out.println("Real shortest path = "+minCost);

//			for (int i = 0 ; i < labyrinth.getNbVertices() ; i++)
//				if (geographParent.getParent(i) != null)
//					System.out.println("parent["+i+"] = "+geographParent.getParent(i).getID());


/*			System.out.println(new Labyrinth(labyrinth,new VertexIdentityFunction<Point>()));
			labyrinth.contract(0,1);
			labyrinth.contract(2,1);
			System.out.println(new Labyrinth(labyrinth,new VertexIdentityFunction<Point>()));
*/
		}
		catch (Exception e) {
			System.out.println(e);
		}



		try {
			Labyrinth l = new Labyrinth(new GeoGraph(new VertexArrayList<Point>(), new NeighboursLists<Long>()), 4, ' ', '|', '-', '+');
			l.fromFile("labyrinthe.txt");
			System.out.println(l);
		}
		catch (Exception e) {
			System.out.println(e);
		}

	}
};

