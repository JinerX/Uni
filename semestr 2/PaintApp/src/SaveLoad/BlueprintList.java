package SaveLoad;

import Shapes.IShape;
import javafx.scene.shape.Shape;

import java.io.Serializable;
import java.util.ArrayList;

public class BlueprintList implements Serializable {
    ArrayList<Blueprint> blueprints;

    public BlueprintList() {
        blueprints = new ArrayList<Blueprint>();
    }

    /**
     *
     * @param shapeList a list of shapes that will be turned into blueprints
     */
    public BlueprintList(ShapeList shapeList){
        blueprints = new ArrayList<Blueprint>();
        ArrayList<Shape> shapes = shapeList.getShapes();
        for(Shape shape : shapes) {
            blueprints.add(((IShape) shape).getBlueprint());
        }
    }

    /**
     *
     * @param blueprint blueprint to be added
     */
    public void addBlueprint(Blueprint blueprint) {
        blueprints.add(blueprint);
    }

    /**
     *
     * @param blueprint blueprint to be removed
     */
    public void removeBlueprint(Blueprint blueprint) {
        blueprints.remove(blueprint);
    }

    /**
     *
     * @return ArrayList of the blueprints
     */
    public ArrayList<Blueprint> getBlueprints() {
        return blueprints;
    }


    public void clearBlueprints() {
        blueprints.clear();
    }

    /**
     *
     * @param index index to be accessed
     * @return blueprint at the index position
     */
    public Blueprint getBlueprintByIndex(int index) {
        return blueprints.get(index);
    }
}
