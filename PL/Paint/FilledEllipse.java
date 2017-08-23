import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;

/**
 * The FilledEllipse class is a child class and extends Ellipse. It has an
 * extra variable called myColor that contains the background color. This
 * class also extends the Draw and ToString methods.
 */
public class FilledEllipse extends Ellipse
{
    int width; //an int to hold the width of the shape
    int height; //an int to hold the height of the shape
    Color myColor; //a Color variable to hold the background color

    /**
     * Filled Ellipse Constructor takes in 2 sets of coords and the color of its rim and its center and calculates the smaller and sets its own params accordingly. The Fcolor is also passed to its super since fcolor is held in the Shape virtual base class
     */
    public FilledEllipse(int sx, int sy, int ex, int ey, Color fcolor, Color bcolor)
    {
        super(sx, sy, ex, ey, fcolor); //pass params to the super class
        width = endx - startx; //calculate the width of the shape
        height = endy - starty; //calculate the height of the shape
        myColor = bcolor; //set myColor to the background color
    }

    /**
    * The Draw method takes a Graphics variable as a parameter. It first sets
    * the color to the background color and calls fillOval. Then it sets the
    * color to the foreground color and calls drawOval.
    */
    public void Draw(Graphics g)
    {
        g.setColor(myColor); //set color to background color
        g.fillOval(startx, starty, width, height); //draw filled ellipse
	    g.setColor(ShapeColor); //set color to foreground color
	    g.drawOval(startx, starty, width, height); //draw oval
    }

    /**
    * The ToString method converts shape data to a displayable string.
    */
    public String ToString()
    {
        return "Shape: Filled Ellipse\nStart Point: (" + startx + "," + starty
               + ") End Point: (" + endx + "," + endy + ")\nForeground Color: "
               + ShapeColor + " Background Color: " + myColor + "\n";
    }
}
