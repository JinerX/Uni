package SaveLoad;

import javafx.scene.shape.Shape;

import java.io.Serializable;
import java.util.ArrayList;

public class ShapeList implements Serializable {
    private ArrayList<Shape> shapes;
    public ShapeList() {
        shapes = new ArrayList<Shape>();
    }

    /**
     *
     * @param shape shape to be added to the list
     */
    public void addShape(Shape shape) {
        shapes.add(shape);
    }

    /**
     *
     * @param shape a shape to be removed from the list
     */
    public void removeShape(Shape shape) {
        shapes.remove(shape);
    }

    /**
     *
     * @return ArrayList of shapes
     */
    public ArrayList<Shape> getShapes() {
        return shapes;
    }

    /**
     *
     * @param index index to be accessed
     * @return shape to be accessed
     */
    public Shape getShapeByIndex(int index) {
        return shapes.get(index);
    }


    public void clearShapes(){
        shapes.clear();
    }
}
