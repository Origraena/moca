package moca.graphs.visu;

import moca.graphs.Graph;
import moca.graphs.vertices.VertexIdentityFunction;
import moca.graphs.edges.Edge;
import java.util.Iterator;
import java.io.File;
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

public class PostScriptConverter {

	public PostScriptConverter(Graph graph, 
							   int width, int height, 
							   Localizer localizer,
							   VertexIdentityFunction function) {
		_graph = graph;
		_width = width;
		_height = height;
		_localizer = localizer;
		_function = function;
	}

	public void writeToFile(String filepath) {
		final int nbVertices = _graph.getNbVertices();
		int i;
		_localizer.process(_graph);
		File outputFile = new File(filepath);
		OutputStreamWriter out = null; 
		String size = "5";
		try { 

			out = new OutputStreamWriter(new BufferedOutputStream(new FileOutputStream(filepath))); 

			// header
			out.write("%!PS-Adobe-3.0\n");
			out.write("%%BoundingBox: 0 0 ");
			out.write(new Integer(_width).toString());
			out.write(" ");
			out.write(new Integer(_height).toString());
			out.write("\n\n");
			out.write("%%BeginProlog \n");
			out.write("/arrowhead {% stack: s x1 y1, current point: x0 y0\n");
			out.write("gsave\n");
			out.write("currentpoint % s x1 y1 x0 y0\n");
			out.write("4 2 roll exch % s x0 y0 y1 x1\n");
			out.write("4 -1 roll exch % s y0 y1 x0 x1\n");
			out.write("sub 3 1 roll % s x1-x2 y0 y1\n");
			out.write("sub exch % s y0-y1 x1-x2\n");
			out.write("atan rotate % rotate over arctan((y0-y1)/(x1-x2))\n");
			out.write("dup scale % scale by factor s\n");
			out.write("-7 2 rlineto 1 -2 rlineto -1 -2 rlineto\n");
			out.write("closepath fill % fill arrowhead\n");
			out.write("grestore\nnewpath\n} def\n");
			out.write("%%EndProlog \n");

			// vertices
			for (i = 0 ; i < nbVertices ; i++) {
				out.write(new Integer(_localizer.x(i)).toString());
				out.write(' ');
				out.write(new Integer(_localizer.y(i)).toString());
				out.write(" ");
				out.write(size);
				out.write(" 0 360 arc\n0 setgray\nfill\nstroke\n\n");
				out.write("/Times-Roman findfont\n");
				out.write("15 scalefont\n");
				out.write("setfont\n");
				out.write(new Integer(_localizer.x(i) - 10).toString());
				out.write(" ");
				out.write(new Integer(_localizer.y(i) + 10).toString());
				out.write(" moveto\n");
				out.write("(");
				if (_function == null)
					out.write(i);
				else
					out.write(_function.exec(_graph.getVertex(i)));
				out.write(") show\n\n");
			}
			// edges
			Edge<Boolean> edge;
			for (Iterator<Edge<Boolean> > edgeiterator = _graph.edgeIterator() ; edgeiterator.hasNext() ;) {
				edge = edgeiterator.next();
				if (edge.getIDU() != edge.getIDV()) {
					out.write(new Integer(_localizer.x(edge.getIDU())).toString());
					out.write(' ');
					out.write(new Integer(_localizer.y(edge.getIDU())).toString());
					out.write(" moveto\n");
					out.write(new Integer(_localizer.x(edge.getIDV())).toString());
					out.write(' ');
					out.write(new Integer(_localizer.y(edge.getIDV())).toString());
					out.write(" lineto\nstroke\n");
					out.write("newpath ");
					out.write(new Integer(_localizer.x(edge.getIDV())).toString());
					out.write(' ');
					out.write(new Integer(_localizer.y(edge.getIDV())).toString());
					out.write(" moveto 2 ");
					out.write(new Integer(_localizer.x(edge.getIDU())).toString());
					out.write(' ');
					out.write(new Integer(_localizer.y(edge.getIDU())).toString());
					out.write(" arrowhead\n\n");
				}
			}
			out.close(); 
		}
		catch (Exception e) {
			System.out.println(e);
			e.printStackTrace();
		}

	}

	private Graph _graph;
	private int _width,_height;
	private Localizer _localizer;
	private VertexIdentityFunction _function; 

};

