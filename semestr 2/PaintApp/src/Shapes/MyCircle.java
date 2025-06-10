package Shapes;

import Drawers.CircleDrawer;
import SaveLoad.Blueprint;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;

import static java.lang.Math.pow;
import static java.lang.Math.sqrt;

public class MyCircle extends Circle implements IShape {
    private double radius;
    private double pivotX;
    private double pivotY;
    private Blueprint blueprint;
    private double movementX;
    private double movementY;
    private String color;
    private double rotateAngle;
    private double x_1;
    private double y_1;
    private double x_2;
    private double y_2;


    /**
     *
     * @param x_1 x value of the first point
     * @param y_1 y value of the first point
     * @param x_2 x value of the second point
     * @param y_2 y value of the second point
     */
    public MyCircle(double x_1, double y_1, double x_2, double y_2){
        this( x_1,  y_1,  x_2,  y_2, String.valueOf(Color.GRAY));
    }

    /**
     *
     * @param x_1 x value of the first point
     * @param y_1 y value of the first point
     * @param x_2 x value of the second point
     * @param y_2 y value of the second point
     * @param color color that will be applied to the shape
     */
    public MyCircle(double x_1, double y_1, double x_2, double y_2, String color) {
        super(x_1,y_1, sqrt(pow((x_2 - x_1), 2) + pow((y_2 - y_1), 2)),Color.web(color));
        super.setStroke(Color.BLACK);
        super.setStrokeWidth(5);
        radius = sqrt(pow((x_2 - x_1), 2) + pow((y_2 - y_1), 2));
        pivotX = x_1;
        pivotY = y_1;
        this.movementX = 0;
        this.movementY = 0;
        this.color = color.toString();
        this.x_1 = x_1;
        this.y_1 = y_1;
        this.x_2 = x_2;
        this.y_2 = y_2;
    }


    /**
     *
     * @param angle angle that shape will be rotated by
     */
    @Override
    public void rotate(double angle) {
        super.setRotate(getRotate()+angle);
    }

    /***
     *
     * @param scale scale that shape will be scaled by
     */
    @Override
    public void scale(double scale) {
        super.setRadius(super.getRadius()*scale);
    }

    /***
     *
     * @param deltaX a value to move the pivotX
     * @param deltaY a value to move the pivotY
     */
    @Override
    public void updatePivots(double deltaX, double deltaY) {
        pivotX = pivotX+deltaX;
        pivotY = pivotY+deltaY;
    }

    /**
     *
     * @param color color to set a shape
     */
    @Override
    public void setColor(String color) {
        super.setFill(Color.web(color));
    }

    /**
     *
     * @return pivotX
     */
    @Override
    public double getPivotXelo(){
        return pivotX;
    }

    /**
     *
     * @return pivotY
     */
    @Override
    public double getPivotYelo(){
        return pivotY;
    }

    /**
     *
     * @return Blueprint to encode the object
     */
    public Blueprint getBlueprint() {
        double x_1 = getCenterX() + getLayoutX();
        double y_1 = getCenterY() + getLayoutY();
        double y_2 = y_1;
        double x_2 = x_1 + getRadius();
        blueprint = new Blueprint(new CircleDrawer(),
                x_1,y_1,
                x_2,y_2,
                rotateAngle,color, 1,1);
        return blueprint;
    }

    /**
     *
     * @param movementX how much the shape moved by
     * @param movementY
     */
    @Override
    public void reportMovement(double movementX, double movementY) {
        this.movementX += movementX;
        this.movementY += movementY;
    }
}
