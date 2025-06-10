package Shapes;

import SaveLoad.Blueprint;


public interface IShape {

    public void rotate(double angle);
    public void scale(double scale);
    public double getPivotXelo();
    public double getPivotYelo();
    public void updatePivots(double deltaX, double deltaY);
    public void setColor(String color);
    public void reportMovement(double movementX, double movementY);
    public Blueprint getBlueprint();
}
