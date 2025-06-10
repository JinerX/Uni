package Shapes;

import Drawers.RectangleDrawer;
import SaveLoad.Blueprint;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

import static java.lang.Math.abs;
import static java.lang.Math.min;

public class MyRectangle extends Rectangle implements IShape {
    private double pivotX;
    private double pivotY;
    public Blueprint blueprint;
    private double movementX;
    private double movementY;
    private double totalScale;
    private String color;
    private double rotateAngle = 0;
    private double x_1;
    private double y_1;
    private double x_2;
    private double y_2;


    /***
     *
     * @param x_1 x value of the first point
     * @param y_1 y value of the first point
     * @param x_2 x value of the second point
     * @param y_2 y value of the second point
     */
    public MyRectangle(double x_1, double y_1, double x_2, double y_2) {
        this(x_1, y_1, x_2, y_2, String.valueOf(Color.GRAY));
    }

    /***
     *
     * @param x_1 x value of the first point
     * @param y_1 y value of the first point
     * @param x_2 x value of the second point
     * @param y_2 y value of the second point
     * @param color color that will be applied to the shape
     */
    public MyRectangle(double x_1, double y_1, double x_2, double y_2, String color) {
        super(min(x_1,x_2), min(y_1,y_2), abs(x_1-x_2), abs(y_1-y_2));
        super.setFill(Color.web(color));
        super.setStroke(Color.BLACK);
        super.setStrokeWidth(5);
        this.color = color;

        pivotX = (x_2+x_1)/2;
        pivotY = (y_2+y_1)/2;

        this.movementX = 0;
        this.movementY = 0;
        this.x_1 = x_1;
        this.y_1 = y_1;
        this.x_2 = x_2;
        this.y_2 = y_2;
    }

    /**
     *
     * @param angle angle to be rotated by
     */
    @Override
    public void rotate(double angle) {
        rotateAngle = getRotate()+angle;
        super.setRotate(rotateAngle);
    }

    /***
     *
     * @param scale a value to scale the shape by
     */
    @Override
    public void scale(double scale) {
        super.setWidth(super.getWidth()*scale);
        super.setHeight(super.getHeight()*scale);
        totalScale=totalScale*scale;
    }

    /**
     *
     * @param deltaX a value to move the pivotX by
     * @param deltaY a value to move the pivotY by
     */
    @Override
    public void updatePivots(double deltaX, double deltaY) {
        pivotX += deltaX;
        pivotY += deltaY;
    }

    /***
     *
     * @param color a color to change the fill
     */
    @Override
    public void setColor(String color) {
        super.setFill(Color.web(color));
        this.color = color;
    }

    /**
     *
     * @return picotX
     */
    @Override
    public double getPivotXelo() {
        return pivotX;
    }


    /**
     *
     * @return pivotY
     */
    @Override
    public double getPivotYelo() {
        return pivotY;
    }

    /**
     *
     * @return Blueprint to encode the shape
     */
    public Blueprint getBlueprint() {
        double x_1 = super.getX()+getLayoutX();
        double y_1 = super.getY()+getLayoutY();
        double x_2 = super.getX()+super.getWidth()+getLayoutX();
        double y_2 = super.getY()+super.getHeight()+getLayoutY();
        blueprint = new Blueprint(new RectangleDrawer(),
                x_1,y_1,
                x_2,y_2,
                rotateAngle,color, 1,1);
        return blueprint;
    }

    /**
     *
     * @param movementX a value that a shape has been moved by
     * @param movementY a value that a shape has been moved by
     */
    @Override
    public void reportMovement(double movementX, double movementY) {
        this.movementX += movementX;
        this.movementY += movementY;
    }
}
