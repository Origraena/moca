package moca.graphs;

import moca.graphs.vertices.Vertex;
import moca.graphs.vertices.VertexArrayList;
import moca.graphs.vertices.VertexUnaryFunction;
import moca.graphs.edges.Edge;
import moca.graphs.edges.NeighboursLists;

import java.util.NoSuchElementException;
import java.util.Iterator;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.io.File;


public class Echiquier {

	public static final int DEFAULT_TILE_SIZE = 4;

	public Echiquier(int nbRaws, int nbColumns, Point s, Point t) {
		try {
			_graph = new GeoGraph(new VertexArrayList<Point>(), new NeighboursLists<Long>());
		}
		catch (Exception e) {
			System.out.println(e);
		}
		_nbColumns = nbColumns;
		_nbRaws = nbRaws;
		_s = s.x + s.y*nbColumns;
		_t = t.x + t.y*nbColumns;
		
		createVertices();
		
		createEdgesForApawn();
	}
	
	public Echiquier(int nbRaws, int nbColumns, Point s, Point t, VertexUnaryFunction<String,Point> vertexDrawFunction) {
		try {
			_graph = new GeoGraph(new VertexArrayList<Point>(), new NeighboursLists<Long>());
		}
		catch (Exception e) {
			System.out.println(e);
		}
		_nbColumns = nbColumns;
		_nbRaws = nbRaws;
		_s = s.x + s.y*nbColumns;
		_t = t.x + t.y*nbColumns;
		
		createVertices();
		
		createEdgesForApawn();
		_vertexDrawFunction = vertexDrawFunction;
	}
	
	public Echiquier(Echiquier e, VertexUnaryFunction<String,Point> vertexDrawFunction) {
		try {
			_graph = e._graph.clone();
		}
		catch (Exception ex) {
			System.out.println(ex);
		}
		_nbColumns = e._nbColumns;
		_nbRaws = e._nbRaws;
		_s = e._s;
		_t = e._t;
		
		_vertexDrawFunction = vertexDrawFunction;
	}
	
	private void createVertices() {
		int i, j, xmax = _nbColumns-1, ymax = _nbRaws-1;
		for(i = 0 ; i <= ymax ; i++)
		{
			for(j = 0 ; j <= xmax ; j++)
			{
				_graph.addPoint(j,i);
			}
		}
	}
	
	private void createEdgesForApawn() {
		int i, j, xmax = _nbColumns-1, ymax = _nbRaws-1;
		for(i = 0 ; i < ymax ; i++)
		{
			for(j = 0 ; j <= xmax ; j++)
			{
				try {
					_graph.addEdge(i*_nbColumns+j, (i+1)*_nbColumns+j);
					if(i == 0)
						_graph.addEdge(i*_nbColumns+j, (i+2)*_nbColumns+j);
				}
				catch (Exception e) {
					System.out.println(e);
				}
			}
		}
	}
	
	public String toString() {
		StringBuilder res = new StringBuilder();
		String vertexStr = new String(new StringBuilder(_freespace));
		int i, j, k, l;
		
		for(k = 0 ; k < _nbColumns ; k++)
		{
			res.append(_intersectionWall);
			for(l = 0 ; l < _tileSize-1 ; l++)
				res.append(_horizontalWall);
		}
		res.append(_intersectionWall+"\n");
		
		for(i = 0 ; i < _nbRaws ; i++)
		{
				for(k = 0 ; k < _nbColumns ; k++)
				{
					res.append(_verticalWall);
					
					// drawing vertex
					if (_vertexDrawFunction != null)
					{
						vertexStr = _vertexDrawFunction.exec(_graph.getVertex(i*_nbColumns+k));
						if (vertexStr.length() > _tileSize-1)
							vertexStr = (new StringBuilder(_freespace)).toString();
						res.append(vertexStr);
						for(l = vertexStr.length() ; l < _tileSize-1 ; l++)
							res.append(_freespace);
					}
					else
					{
						for(l = 0 ; l < _tileSize-1 ; l++)
						{
							res.append(_freespace);
						}
					}
				}
			
			res.append(_verticalWall+"\n");
			
			for(k = 0 ; k < _nbColumns ; k++)
			{
				res.append(_intersectionWall);
				for(l = 0 ; l < _tileSize-1 ; l++)
					res.append(_horizontalWall);
			}
			res.append(_intersectionWall+"\n");
		}

		return res.toString();
	}

	public int getSource()
	{
		return _s;
	}
	
	public int getDestination()
	{
		return _t;
	}
	
	public GeoGraph getGraph()
	{
		return _graph;
	}
	

	/* internal processing variables */
	private GeoGraph _graph = null;
	private int _s = -1, _t = -1;
	private int _nbColumns = -1, _nbRaws = -1;
	
	/* skin */
	private char _freespace = ' ';
	private char _verticalWall = '|';
	private char _horizontalWall = '-';
	private char _intersectionWall = '+';
	private int _tileSize = DEFAULT_TILE_SIZE;
	private VertexUnaryFunction<String,Point> _vertexDrawFunction = null;
	
};


