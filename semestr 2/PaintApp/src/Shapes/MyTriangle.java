package Shapes;

import Drawers.TriangleDrawer;
import SaveLoad.Blueprint;
import javafx.collections.ObservableList;
import javafx.geometry.Point2D;
import javafx.scene.paint.Color;
import javafx.scene.shape.Polygon;

import java.util.List;

public class MyTriangle extends Polygon implements IShape {
    private double pivotX;
    private double pivotY;
    private Blueprint blueprint;
    private double movementX;
    private double movementY;
    private String color;
    private double rotateAngle;
    private double currentScaleX=1;
    private double currentScaleY=1;
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
    public MyTriangle(double x_1, double y_1, double x_2, double y_2) {
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
    public MyTriangle(double x_1, double y_1, double x_2, double y_2, String color) {
        super.getPoints().addAll(new Double[]{
                x_1,y_1,
                x_2,y_2,
                2*x_1-x_2,y_2
        });

        this.x_1 = x_1;
        this.y_1 = y_1;
        this.x_2 = x_2;
        this.y_2 = y_2;

        this.color = color;
        super.setFill(Color.web(color));
        super.setStroke(Color.BLACK);
        super.setStrokeWidth(5);
        this.pivotX = x_1;
        this.pivotY = (y_1+2*y_2)/3;
        this.movementX = 0;
        this.movementY = 0;
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

    /**
     *
     * @param scale a value to scale the shape by
     */
    @Override
    public void scale(double scale) {
        currentScaleX = getScaleX()*scale;
        currentScaleY =  getScaleY()*scale;
        setScaleX(currentScaleX);
        setScaleY(currentScaleY);
    }

    /**
     *
     * @param deltaX a value to move the pivotX by
     * @param deltaY a value to move the pivotY by
     */
    @Override
    public void updatePivots(double deltaX, double deltaY) {
        this.pivotX += deltaX;
        this.pivotY += deltaY;
    }

    /**
     *
     * @param color a color to set the shape by
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

    /***
     *
     * @param movementX value that the shape has been moved by along x-axis
     * @param movementY value that the shape has been moved by along y-axis
     */
    @Override
    public void reportMovement(double movementX, double movementY) {
        this.movementX += movementX;
        this.movementY += movementY;
    }

    /**
     *
     * @return blueprint of the shape
     */
    public Blueprint getBlueprint() {
        ObservableList<Double> points = getPoints();
        double deltaX = getLayoutX();
        double deltaY = getLayoutY();
        Double point_1 = points.get(0);
        Double point_2 = points.get(1);
        Double point_3 = points.get(2);
        Double point_4 = points.get(3);

        blueprint = new Blueprint(new TriangleDrawer(),point_1+deltaX,point_2+deltaY,point_3+deltaX,point_4+deltaY,rotateAngle,color, currentScaleX, currentScaleY);
        return blueprint;
    }
}
