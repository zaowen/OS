import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;

/**
 * The Ellipse class is a child class of Shape and extends its Draw and
 * ToString methods.
 */
public class Ellipse extends Shape
{
    int width; //an int to hold the width of the shape
    int height; //an int to hold the height of the shape


    /**
     * Ellipse Constructor takes in 2 sets of coords and the color of its rim and calculates the smaller and sets its own params accordingly.
     */
    public Ellipse(int sx, int sy, int ex, int ey, Color fcolor)
    {
	    super(sx, sy, ex, ey, fcolor); //pass the params to the superclass
	    width = endx - startx; //calculate the width of the shape
	    height = endy - starty; //calculate the height of the shape
    }

    /**
    * The Draw method takes a Graphics variable, sets the color, and calls
    * drawOval.
    */
    public void Draw(Graphics g)
    {
        g.setColor(ShapeColor); //set the color of the shape
        g.drawOval(startx, starty, width, height); //draw the oval
    }

    /**
    * The ToString method converts the shape data into a displayable string.
    */
    public String ToString()
    {
        return "Shape: Ellipse\nStart Point: (" + startx + "," + starty
               + ") End Point: (" + endx + "," + endy + ")\nColor: "
               + ShapeColor + "\n";
    }
}
