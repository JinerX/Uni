package SaveLoad;

import Drawers.ADrawer;
import javafx.scene.shape.Shape;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serializable;

public class Deserializer implements Serializable {
    /**
     *
     * @param filePath a path to the file
     * @return ShapeList of shapes made from blueprints
     * @throws IOException
     * @throws ClassNotFoundException
     */
    public static ShapeList deserialize(String filePath) throws IOException,ClassNotFoundException{
        ADrawer drawer;
        ShapeList shapeList = new ShapeList();
        FileInputStream fis = new FileInputStream(filePath);
        ObjectInputStream ois = new ObjectInputStream(fis);
        BlueprintList blueprintList = null;
        blueprintList = (BlueprintList) ois.readObject();
        ois.close();
        fis.close();

        for(Blueprint blueprint : blueprintList.getBlueprints()){
            drawer = blueprint.getDrawer();
            Shape shape = drawer.draw(blueprint.getX1(), blueprint.getY1(), blueprint.getX2(), blueprint.getY2(), blueprint.getColor());
            shape.setRotate(blueprint.getRotationAngle());
            shape.setScaleX(blueprint.getScaleX());
            shape.setScaleY(blueprint.getScaleY());
            shapeList.getShapes().add(shape);
        }
        return shapeList;
    }
}
