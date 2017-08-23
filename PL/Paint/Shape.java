import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;
import java.util.ArrayList;

/**
 * The Shape class is the base/super class that all the other shape classes
 * extend. It contains the foreground color, start x and y, end x and y, and
 * the center x and y values. The methods extended by the child classes are:
 * Draw and ToString. 
 */
abstract public class Shape
{
    public int startx = 0; //int to hold the x value of the start point
    public int starty =0; //int to hold the y value of the start point
    public int endx = 0; //int to hold the x value of the end point
    public int endy = 0; //int to hold the y value of the end point
    public int centx = 0; //int to hold the x value of the centroid
    public int centy = 0; //int to hold the y value of the centroid
    Color ShapeColor; //a Color variable to hold the foreground color
	
    public Shape(int sx, int sy, int ex, int ey, Color incolor)
    {
	    ShapeColor = incolor; //set the foreground color
        startx = sx;
        endx = ex;
        starty = sy;
        endy = ey;

        if(sx < ex) //set the start and end x values correctlly
        {
            startx = sx;
            endx = ex;
        }
        else
        {
            startx = ex;
            endx = sx;
        }

        if(sy < ey) //set the start and end y values correctly
        {
            starty = sy;
            endy = ey;
        }
        else
        {
            starty = ey;
            endy = sy;
	    }

	    centy = (endy+starty)/2; //calculate the centroids y value
	    centx = (startx+endx)/2; //calculate the centroids x value
    }

    /*
    * The Move method changes the start and end points based on the amount
    * the shape was moved.
    */   
    public void Move(int deltax, int deltay)
    {
        startx += deltax;
        endx += deltax;
        starty += deltay;
        endy += deltay;
	    centx += deltax;
	    centy += deltay;
    }

    public int Get_Centroidy()
    {
	    return centy;
    }

    public int Get_Centroidx()
    {
	    return centx;
    }
    
    abstract void Draw( Graphics g ); //abstract method implemented in child classes

    abstract String ToString(); //abstract method implemented in child classes
    
    /*
    * The Dist method calculates and returns the distance of the centroid to
    * the point given.
    */
    public double Dist( int x, int y)
    {    
        int myx = startx - endx;
        int myy = starty - endy;
        
        return ( Math.pow( (double)(myx - x),2.0) - Math.pow( (double)(myy - y),2.0) );
    }          
}
