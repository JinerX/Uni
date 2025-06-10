package Drawers;

import Shapes.MyRectangle;
import javafx.scene.paint.Color;


public class RectangleDrawer extends ADrawer {
    /**
     *
     * @param x_1 x value of the first point
     * @param y_1 y value of the first point
     * @param x_2 x value of the second point
     * @param y_2 y value of the second point
     * @param color color that will be applied to the shape
     * @return MyRectangle object
     */
    @Override
    public MyRectangle draw(double x_1, double y_1, double x_2, double y_2, String color) {
        MyRectangle rectangle = new MyRectangle(x_1, y_1, x_2, y_2, color);
        return rectangle;
    }
}
