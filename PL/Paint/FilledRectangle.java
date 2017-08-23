import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;

/**
 * The FilledRectangle class is a child class and extends Rectangle. It has an
 * extra variable to hold the background color. It also extends the Draw and
 * ToString methods.
 */
public class FilledRectangle extends Rectangle
{
    int width; //an int to hold the width of the shape
    int height; //an int to hold the height of the shape
    Color mycolor; //a Color variable to hold the background color for the shape

    /**
     * Rectangle Constructor takes in 2 sets of coords and the color of its rim and its center and calculates the smaller and sets its own params accordingly. The Fcolor is also passed to its super since fcolor is held in the Shape virtual base class
     */
    public FilledRectangle(int sx, int sy, int ex, int ey, Color fcolor, Color bcolor)
    {
        super(sx, sy, ex, ey, fcolor); //pass the params to the super class
        mycolor = bcolor; //set mycolor to the background color
        width = endx - startx; //calculate the width of the shape
        height = endy - starty; //calculate the height of the shape
    }

    /**
    * The Draw method takes a Graphics variable as a parameter. It sets the
    * color to the background color and calls fillRect. Then it sets the color
    * to the foreground color and calls drawRect.
    */
    public void Draw(Graphics g)
    {
        g.setColor(mycolor); //set color to background color
        g.fillRect(startx, starty, width, height); //draw filled rectangle
	    g.setColor(ShapeColor); //set color to foreground color
	    g.drawRect(startx, starty, width, height); //draw rectangle
    }

    /**
    * The ToString method converts the shape data into a displayable string.
    */
    public String ToString()
    {
        return "Shape: Filled Rectangle\nStart Point: (" + startx + ","
               + starty + ") End Point: (" + endx + "," + endy
               + ")\nForeground Color: " + ShapeColor + " Background Color: "
               + mycolor + "\n";
    }
}
