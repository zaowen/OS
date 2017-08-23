 /*
 * The Line class is a child class of Shape and extends its Draw and ToString
 * methods.
 */
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;

public class Line extends Shape
{
    public Line(int sx, int sy, int ex, int ey, Color mycolor)
    {
        super(sx, sy, ex, ey, mycolor); //pass the params to the superclass
    }

    /*
    * The Draw method takes a Graphics variable, sets the color, and calls
    * drawLine.
    */
    public void Draw(Graphics g)
    {
        g.setColor( ShapeColor ); //set the color of the shape
        g.drawLine(startx, starty, endx, endy); //draw a line
    }
    
    /*
    * The ToString method converts the shape data into a displayable string.
    */
    public String ToString()
    {
        return "Shape: Line\nStart Point: (" + startx + "," + starty
               + ") End Point: (" + endx + "," + endy + ")\nColor: "
               + ShapeColor + "\n";
    }
}
