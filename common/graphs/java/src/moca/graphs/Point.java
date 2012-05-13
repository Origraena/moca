package moca.graphs;

import java.lang.Math;

public class Point implements Comparable<Point> {
	
	public Point() {
		this.x = 0;
		this.y = 0;
	}
	
	public Point(int x, int y) {
		this.x = x;
		this.y = y;
	}

	public Point(long x, long y) {
		// TODO check size of long
		this.x = (int) x;
		this.y = (int) y;
	}
	
	public Point(Point p) {
		this.x = p.x;
		this.y = p.y;
	}

	public String toString() {
		return x + "," + y;
	}
	
	public int x;
	public int y;

	public static long euclidianDistance(Point p, Point q) {
		return (long)Math.floor(Math.sqrt(euclidianDistanceS(p,q)));
	}

	public static long euclidianDistanceS(Point p, Point q) {
		return ((p.x - q.x) * (p.x - q.x)) + ((p.y - q.y) * (p.y - q.y));
	}

	public static long scalar(Point p, Point q) {
		return (p.x * q.x) + (p.y * q.y);
	}

	public static long determinant(Point p, Point q) {
		return (p.x * q.y) - (p.y * q.x);
	}

	public static Point vector(Point p, Point q) {
		return new Point(q.x - p.x, q.y - p.y);
	}

	public int compareTo(Point p) {
/*		int result = p.x - q.x;
		if (result == 0)
			result = p.y - q.y;
		return result;*/
		return (x==p.x)?y-p.y:x-p.x;
	}

};

