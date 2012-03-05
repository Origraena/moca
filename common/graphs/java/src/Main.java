import moca.*;
import moca.graphs.*;
import moca.graphs.edges.*;
import moca.graphs.vertices.*;
import java.lang.Exception;
import java.util.Iterator;

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
	}
};

