import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;

/**
 * The Rectangle class is a child class of the Shape class. It extends the
 * Shape class methods Draw and ToString. It also has class variables width
 * and height that are needed for the drawRect call. 
 */
public class Rectangle extends Shape
{
    int width; //an int to hold the width of the shape
    int height; //an int to hold the height of the shape

    /**
     * Rectangle Constructor takes in 2 sets of coords and the color of its rim and calculates the smaller and sets its own params accordingly.
     */
    public Rectangle(int sx, int sy, int ex, int ey, Color fcolor)
    {
        super(sx, sy, ex, ey, fcolor); //pass the params into the super class
        width = endx - startx; //calculate the width
	height = endy - starty; //calculate the height
    }

    /**
    * The Draw method takes a Graphics variable as a parameter. It sets the
    * color and then calls drawRect.
    */
    public void Draw(Graphics g)
    {
        g.setColor(ShapeColor); //set the color
        g.drawRect(startx, starty, width, height); //draw rectangle
    }

    /**
    * The ToString method simply converts the shape data into a displayable
    * string.
    */
    public String ToString()
    {
        return "Shape: Rectangle\nStart Point: (" + startx + "," + starty
               + ") End Point: (" + endx + "," + endy + ")\nColor: "
               + ShapeColor + "\n";
    }
}
