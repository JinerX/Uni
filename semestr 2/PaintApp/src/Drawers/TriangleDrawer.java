package Drawers;

import Shapes.MyTriangle;
import javafx.scene.paint.Color;

public class TriangleDrawer extends ADrawer {
    /**
     *
     * @param x_1 x value of the first point
     * @param y_1 y value of the first point
     * @param x_2 x value of the second point
     * @param y_2 y value of the second point
     * @param color color that will be applied to the shape
     * @return MyTriangle object
     */
    @Override
    public MyTriangle draw(double x_1, double y_1, double x_2, double y_2, String color) {
        MyTriangle triangle = new MyTriangle(x_1, y_1, x_2, y_2, color);
        return triangle;
    }
}
