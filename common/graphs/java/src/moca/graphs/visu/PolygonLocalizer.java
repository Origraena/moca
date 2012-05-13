package moca.graphs.visu;

import moca.graphs.Graph;
import moca.graphs.Point;
import java.lang.Math;
import java.util.NoSuchElementException;

public class PolygonLocalizer extends AbstractLocalizer implements Localizer {

	public static final int DEFAULT_OFFSET_X = -60;
	public static final int DEFAULT_OFFSET_Y = 60;

	public PolygonLocalizer(Graph graph, int width, int height) {
		super();
		_width = width;
		_height = height;
		process(graph);
	}

	public PolygonLocalizer(int width, int height) {
		super();
		_width = width;
		_height = height;
		_offsetx = DEFAULT_OFFSET_X;
		_offsety = DEFAULT_OFFSET_Y;
	}

	public PolygonLocalizer(int width, int height, int offsetx, int offsety) {
		super();
		_width = width;
		_height = height;
		_offsetx = offsetx;
		_offsety = offsety;
	}

    public void process(Graph graph) {
    	final int nbVertices = graph.getNbVertices();
    	int i;
    	int x,y;
    	double foot;
    	double footAngle,angle;

    	_positions = new Point[nbVertices];
    	foot = (double)(_width + _height) / (double)nbVertices;
    	footAngle = Math.toRadians(360.0 / nbVertices);
    	angle = 0.0;
    	x = _width / 2 + _offsetx;
    	y = _height / 2 + _offsety;
    	for (i = 0 ; i < nbVertices ; i++) {
    		angle += footAngle;
    		x += Math.sin(angle) * foot;
    		y += Math.cos(angle) * foot;
    		_positions[i] = new Point(x,y);
    	}
    }

	public Point position(int vertexID) throws NoSuchElementException {
		if ((vertexID < 0) 
		 || (vertexID >= _positions.length) 
		 || (_positions[vertexID] == null))
			throw new NoSuchElementException();
		return _positions[vertexID];
	}
	
	protected Point[] _positions;
	protected int _width,_height;
	protected int _offsetx,_offsety;

};

