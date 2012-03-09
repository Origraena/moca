package moca.graphs;

import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexUnaryFunction;
import moca.graphs.edges.Edge;

import java.util.NoSuchElementException;
import java.util.Iterator;


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

//	public boolean fromFile(String filename) {
//		
//		/* init */
//		clear();
//		int i = 0;
//		int j = 0;
//		int imax = 0;
//		int jmax = 0;
//		char c = '\0';
//		String line = null;
//		String line2 = null;
//
		/* vertices */
//		line = file.readLine();
//		line2 = file.readLine();
//		imax = line.length / _tileSize;
//		while (/*TODO*/1) {
//			for (i = 0 ; i <= imax ; i++)
//				addPoint(i,j);
//			j++;
//			line = file.readLine();
//			line2 = file.readLine();
//		}
//		jmax = j;
		
		/* edges */
		// file.reload()
//		j = 0;
//		line = file.readLine();
//		line2 = file.readLine();
//		for (j = 0 ; j < jmax ; j++) {
//			for (int i = 0 ; i < line.length ; i+=_tileSize) {
//				c = line2.charAt(i);
//				if (c == ' ')
//					addEdge(j*imax+(i/_tileSize),(j+1)*imax+(i/_tileSize));
//				c = line.charAt(i);
//				if (c == ' ')
//					addEdge(j*imax+(i/_tileSize),j*imax+(i/_tileSize)+1);
//			}
//		}
//	}
//
//
//
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

