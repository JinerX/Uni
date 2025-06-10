package SaveLoad;

import Drawers.ADrawer;

import java.io.Serializable;

public class Blueprint implements Serializable {
    private ADrawer drawer;
    private double x_1;
    private double y_1;
    private double x_2;
    private double y_2;
    private String color;
    private double rotationAngle;
    private double scaleX;
    private double scaleY;
    public Blueprint(ADrawer drawer, double x_1, double y_1, double x_2, double y_2, double rotationAngle, String color, double scaleX, double scaleY) {
        this.drawer = drawer;
        this.x_1 = x_1;
        this.y_1 = y_1;
        this.x_2 = x_2;
        this.y_2 = y_2;
        this.rotationAngle = rotationAngle;
        this.color = color;
        this.scaleX = scaleX;
        this.scaleY = scaleY;
    }

    /**
     *
     * @param drawer a drawer object that will be used to generate the figure
     */
    public void setDrawer(ADrawer drawer) {
        this.drawer = drawer;
    }

    /**
     *
     * @return drawer object
     */
    public ADrawer getDrawer() {
        return drawer;
    }

    /**
     *
     * @param x_1 x value of the first point
     * @param y_1 y value of the first point
     */
    public void setPoint_1(double x_1, double y_1) {
        this.x_1 = x_1;
        this.y_1 = y_1;
    }

    /**
     *
     * @param x_2 x value of the second point
     * @param y_2 y value of the second point
     */
    public void setPoint_2(double x_2, double y_2) {
        this.x_2 = x_2;
        this.y_2 = y_2;
    }

    /**
     *
     * @param color color that will be applied to the figure
     */
    public void setColor(String color) {
        this.color = color;
    }

    /**
     *
     * @param rotationAngle angle of rotation of the object
     */
    public void setRotationAngle(double rotationAngle) {
        this.rotationAngle = rotationAngle;
    }

    /**
     *
     * @return x value of the first point
     */
    public double getX1() {
        return x_1;
    }

    /**
     *
     * @return y value of the first point
     */
    public double getY1() {
        return y_1;
    }

    /**
     *
     * @return x value of the second point
     */
    public double getX2() {
        return x_2;
    }

    /**
     *
     * @return y value of the second point
     */
    public double getY2() {
        return y_2;
    }

    /**
     *
     * @return color of the object
     */
    public String getColor() {
        return color;
    }

    /**
     *
     * @return rotation angle
     */
    public double getRotationAngle() {
        return rotationAngle;
    }

    /**
     *
     * @return scale of the shape along x-axis
     */
    public double getScaleX(){
        return scaleX;
    }

    /**
     *
     * @return scale of the shape along y-axis
     */
    public double getScaleY(){
        return scaleY;
    }
}
