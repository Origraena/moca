package moca.graphs;

import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexUnaryFunction;
import moca.graphs.edges.Edge;

import java.util.NoSuchElementException;
import java.util.Iterator;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.io.File;


public class Labyrinth {

	public static final int DEFAULT_TILE_SIZE = 4;

	public Labyrinth(GeoGraph graph) {
		_graph = graph;

	}

	public Labyrinth(GeoGraph graph, VertexUnaryFunction<String,Point> vertexDrawFunction) {
		_graph = graph;
		_vertexDrawFunction = vertexDrawFunction;
	}

	public Labyrinth(GeoGraph graph, int tileSize, char freeSpace, char verticalWall, char horizontalWall, char intersectionWall) {
		_graph = graph;
		_tileSize = tileSize;
		_freespace = freeSpace;
		_verticalWall = verticalWall;
		_horizontalWall = horizontalWall;
		_intersectionWall = intersectionWall;
	}

	public Labyrinth(GeoGraph graph, int tileSize, char freeSpace, char verticalWall, char horizontalWall, char intersectionWall, VertexUnaryFunction<String,Point> vertexDrawFunction) {
		_graph = graph;
		_tileSize = tileSize;
		_freespace = freeSpace;
		_verticalWall = verticalWall;
		_horizontalWall = horizontalWall;
		_intersectionWall = intersectionWall;
		_vertexDrawFunction = vertexDrawFunction;
	}

	public boolean fromFile(String filename) {
		try {	
		/* init */
		int i = 0;
		int j = 0;
		int imax = 0;
		int jmax = 0;
		char c = '\0';
		File file = new File(filename);
		Scanner scan;
		scan = new Scanner(file, "UTF-8");
		scan.useDelimiter(Pattern.compile("[\n]"));
		String line1 = null;
		String line2 = null;
	

		/* vertices */
		line1 = scan.next();		// first line to be filtered
//		line1 = scan.next();
//		line2 = scan.next();
		System.out.print(line1);
		System.out.println(line1.length());
		imax = (line1.length()-1) / _tileSize -1;
		j = 0;
		while (scan.hasNext()) {
			for (i = 0 ; i <= imax ; i++)
				_graph.addPoint(i,j-1);
			j++;
			line1 = scan.next(); 
			line2 = scan.next();
		}
		jmax = j;
System.out.println("imax = "+imax+" jmax = "+jmax);
System.out.println("nbVertices = "+_graph.getNbVertices());
		/* edges */
		j = 0;
		scan = new Scanner(file,"UTF-8");
		scan.useDelimiter(Pattern.compile("[\n]"));
		scan.next();		// first line to be filtered
		line1 = scan.next();
		line2 = scan.next();
		for (j = 0 ; j < jmax ; j++) {
			i = _tileSize/2;
			int k = _tileSize;
			while (k < line1.length()) {
				c = line2.charAt(k);
				if (c == ' ')	// en dessous
					_graph.addEdge(j*imax+(k/_tileSize),(j+1)*imax+(k/_tileSize));
				c = line1.charAt(i);
				if (c == ' ')	// a droite
					_graph.addEdge(j*imax+(k/_tileSize),j*imax+(k/_tileSize)+1);
				i += _tileSize;
				k += _tileSize;
			}
		}
		scan.close();
		return true;
		}
		catch (Exception e) {
			System.out.println(e);
			e.printStackTrace();
			return false;
		}
	}



	public String toString() {
		Vertex<Point> v = null;
		for (int i = 0 ; (imax == 0) && (i < _graph.getNbVertices()) ; i++) {
			if ((i == _graph.getNbVertices()-2) || (_graph.get(i).x > _graph.get(i+1).x))
				imax = _graph.get(i).x;
		}
		jmax = _graph.getNbVertices() / imax;
		int  i = 0;
		int j = 0;
		String vertexStr = "";

		// first line
		_line1.append('+');
		for (int k = 0 ; k <= (imax+1)*_tileSize - 2 ; k++)
			_line1.append(_horizontalWall);
		_line1.append('+');
		_result.append(_line1);
		_result.append('\n');


		_line1 = new StringBuilder();
		_line2 = new StringBuilder();
		_line1.append(_verticalWall);
		if (isDownWall(i))
			_line2.append(_intersectionWall);
		else
			_line2.append(_verticalWall);
		for (Iterator<Vertex<Point> > iterator = _graph.vertexIterator() ; (iterator.hasNext()) && (i < _graph.getNbVertices() /*- (imax+2)*/) ; i++) {
			v = iterator.next();
			j++;
	
			// drawing vertex
			if (_vertexDrawFunction != null)
				vertexStr = _vertexDrawFunction.exec(v);
			if (vertexStr.length() > _tileSize - 1)
				vertexStr = "";
			else
				_line1.append(vertexStr);

			// drawing left free space
			for (int k = 1 ; k < (_tileSize - vertexStr.length()) ; k++)
				_line1.append(_freespace);

			// drawing last char of vertex i on line 1
			if (isRightWall(i))
				_line1.append(_verticalWall);
			else
				_line1.append(_freespace);

			// if there is a wall down to i
			if (isDownWall(i)) {
				for (int k = 1 ; k < _tileSize ; k++)
					_line2.append(_horizontalWall);
				if (((i < _graph.getNbVertices()-imax-1) && isRightWall(atDown(i))) 
				|| isRightWall(i) 
				|| ((i < _graph.getNbVertices()-imax-1) && !isDownWall(atRight(i))))
					_line2.append(_intersectionWall);
				else
					_line2.append(_horizontalWall);
			}
			else {
				for (int k = 1 ; k < _tileSize ; k++)
					_line2.append(_freespace);
				if (isRightWall(i)) {
					if (((j != imax+1) && (isDownWall(atRight(i)))) || (!isRightWall(atDown(i))))
						_line2.append(_intersectionWall);
					else
						_line2.append(_verticalWall);
				}
				else if ((isRightWall(atDown(i))) || (isDownWall(atRight(i))))
					_line2.append(_intersectionWall);
				else
					_line2.append(_freespace);
			}

			if (j == imax+1) {
				_result.append(_line1);
				_result.append("\n");
				_result.append(_line2);
				_result.append("\n");
				_line1 = new StringBuilder();
				_line2 = new StringBuilder();
				_line1.append(_verticalWall);
				if (isDownWall(i+1))
					_line2.append(_intersectionWall);
				else
					_line2.append(_verticalWall);
				j = 0;
			}
		}

		return _result.toString();
	}


	
	/* internal methods */
	/* cannot be used outsite because of the non valid values before toString() operation. */
	protected boolean isRightWall(int i) {
		try {
			return !_graph.isEdge(i,atRight(i));
		}
		catch (Exception e) {
			return true;
		}
	}
	protected boolean isDownWall(int i) {
		try {
			return !_graph.isEdge(i,atDown(i));
		}
		catch (Exception e) {
			return true;
		}
	}
	protected int atLeft(int i) {
		return i-1;
	}
	protected int atRight(int i) {
		return i+1;
	}
	protected int atDown(int i) {
		return i+imax+1;
	}
	protected int atUp(int i) {
		return i-imax-1;
	}

	/* internal processing variables */
	private GeoGraph _graph = null;
	private StringBuilder _result = new StringBuilder();
	private StringBuilder _line1 = new StringBuilder();
	private StringBuilder _line2 = new StringBuilder();
	private int imax = 0;
	private int jmax = 0;

	/* skin */
	private char _freespace = ' ';
	private char _verticalWall = '|';
	private char _horizontalWall = '-';
	private char _intersectionWall = '+';
	private int _tileSize = DEFAULT_TILE_SIZE;
	private VertexUnaryFunction<String,Point> _vertexDrawFunction = null;
	
};
/*
	public void writeToFile(String nomFichier)
	{
		int nbFaits = 0;
		int nbRegles = 0;
		int currentIndex = 0;
		String trans = "";

		try
		{
			FileWriter fw = new FileWriter(nomFichier, false);
			BufferedWriter out = new BufferedWriter(fw);
			
			nbFaits = BF.getListeAtomes().size();
			out.write(String.valueOf(nbFaits));
			out.write('\n');
	    	while (currentIndex < nbFaits)
	    	{
	    		out.write(BF.getListeAtomes().get(currentIndex).toString()+"\n");
	    		currentIndex++;
	    	}
	    	
	    	nbRegles = BR.size();
	    	out.write(String.valueOf(nbRegles));
			out.write("\n");
	    	currentIndex = 0;
	    	while (currentIndex < nbRegles)
	    	{
	    		trans = "";
	    		for(int i = 0 ; i < BR.get(currentIndex).getNbHypotheses() ; i++)
	    		{
	    			trans += BR.get(currentIndex).getHypothese(i) + ";";
	    		}
	    		trans += BR.get(currentIndex).getConclusion()+"\n";
	    		
	    		out.write(trans);
	    		currentIndex++;
	    	}
	    	out.flush();
		}
		catch(IOException e)
		{
			System.out.println("An error occured during writing file.");
		}
	}
*/

