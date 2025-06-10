package Drawers;

import javafx.scene.paint.Color;
import javafx.scene.shape.Shape;

import java.io.Serializable;

public abstract class ADrawer implements Serializable {
    public abstract Shape draw(double x_1,double y_1,double x_2,double y_2, String color);
}
