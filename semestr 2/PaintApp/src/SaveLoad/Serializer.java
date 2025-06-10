package SaveLoad;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class Serializer implements Serializable {
    /**
     *
     * @param shapeList a list of shapes to be  turned into serialzied as blueprints
     * @throws IOException
     */
    public static void serialize(ShapeList shapeList) throws IOException {
        BlueprintList shapeBlueprintList= new BlueprintList(shapeList);
        FileOutputStream fileOutputStream = new FileOutputStream("ShapeBluePrintListInfo.ser");
        ObjectOutputStream objectOutputStream = new ObjectOutputStream(fileOutputStream);
        objectOutputStream.writeObject(shapeBlueprintList);
        objectOutputStream.close();
        fileOutputStream.close();
    }
}
