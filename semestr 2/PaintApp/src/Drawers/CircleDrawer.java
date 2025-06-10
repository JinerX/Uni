package Drawers;

import Shapes.MyCircle;
import javafx.scene.paint.Color;

public class CircleDrawer extends ADrawer {
    /**
     *
     * @param x_1 x value of the first point
     * @param y_1 y value of the first point
     * @param x_2 x value of the second point
     * @param y_2 y value of the second point
     * @param color color that will be applied to the shape
     * @return MyCircle object
     */
    @Override
    public MyCircle draw(double x_1, double y_1, double x_2, double y_2, String color) {
        MyCircle circle = new MyCircle(x_1, y_1, x_2, y_2, color);
        return circle;
    }
}
