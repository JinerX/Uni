import Controllers.DragController;
import Controllers.EditController;
import Drawers.ADrawer;
import Drawers.CircleDrawer;
import Drawers.RectangleDrawer;
import Drawers.TriangleDrawer;
import SaveLoad.BlueprintList;
import SaveLoad.Deserializer;
import SaveLoad.Serializer;
import SaveLoad.ShapeList;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.shape.Shape;

import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.logging.Level;

import static java.lang.Math.abs;
import static java.lang.Math.min;

public class PaneController {

    public enum State{
        GENERATEMODE,
        EDITMODE;
    }

    private ADrawer drawer = new RectangleDrawer();
    private DragController dragController;
    private EditController editController;
    private double firstXclicked = Double.NaN;
    private double firstYclicked = Double.NaN;
    private double secondXclicked = Double.NaN;
    private double secondYclicked = Double.NaN;
    private State currentState = State.EDITMODE;
    private ShapeList shapeList = new ShapeList();
    private BlueprintList blueprintList = new BlueprintList();
    private Line mouseLine = null;
    private DoubleProperty mouseX = new SimpleDoubleProperty();
    private DoubleProperty mouseY = new SimpleDoubleProperty();

    @FXML
    private Pane displayPane;

    @FXML
    private Button circleButton;

    @FXML
    private Button rectangleButton;

    @FXML
    private Button triangleButton;

    @FXML
    private CheckBox editModeCheckbox;

    @FXML
    private ToolBar toolBar;

    @FXML
    private ColorPicker picker = new ColorPicker();

    @FXML
    private Button clearButton;

    @FXML
    private Button saveButton;

    @FXML
    private Button loadButton;

    @FXML
    private Button infoButton;


    /***
     *
     * @param event checking or unchecking the checkbox
     */
    public void switchMode(ActionEvent event) {
        if(currentState==State.GENERATEMODE){
            currentState=State.EDITMODE;
        }
        if(currentState==State.EDITMODE){
            currentState=State.GENERATEMODE;
        }
    }

    /**
     *
     * @param event the press of a button
     */
    public void circleButtonPressed(ActionEvent event) {
        drawer = new CircleDrawer();

    }

    public void infoButtonPressed(ActionEvent event) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("Info");
        alert.setHeaderText("Information about the app");
        alert.setContentText(
                "Autor : Jędrzej Sajnóg \n" +
                "Opis : Aplikacja paint służy do rysowania i manipulowania figurami geometrycznymi(Prostokąt, trójkąt równoramienny, koło) \n" +
                        "Operacje :\n" +
                        "przy niezaznaczonym checkbox'ie: tworzenie figur przez naciśnięcie na dwa punkty na polu do rysowania po naciśnięciu na odpowiedni przycisk trzymając ctrl, przesuwania figur znajdujących się na polu do rysowania przez przytrzymywanie ich i przeciąganie\n" +
                        "przy zaznaczonym checbox'ie: rotacja figur(PPM+scroll), skalowanie figur(LPM+scroll), zmiana koloru na ten zaznaczony pickerem(SPM)"

        );
        alert.show();
    }

    /**
     * @param event the press of a button
     */
    public void rectangleButtonPressed(ActionEvent event) {
        drawer = new RectangleDrawer();
    }

    /**
     *
     * @param event the press of a button
     */
    public void triangleButtonPressed(ActionEvent event) {
        drawer = new TriangleDrawer();
    }

    /***
     *
     * @param event the press of a button
     */
    public void clearButtonPressed(ActionEvent event) {
        displayPane.getChildren().clear();
        shapeList.clearShapes();
    }

    /***
     *
     * @param event the press of a button
     */
    public void save(ActionEvent event) {
        try {
            new FileWriter("C:\\Users\\JS\\IdeaProjects\\PaintApp\\ShapeBluePrintListInfo.ser", false).close();
            Serializer.serialize(shapeList);
        }
        catch (IOException ex) {
            Alert errorAlert = new Alert(Alert.AlertType.ERROR);
            errorAlert.setTitle("IOException");
            errorAlert.setContentText(ex.getMessage());
        }
    }

    /***
     *
     * @param event the press of a button
     */
    public void load(ActionEvent event){
        try {
            displayPane.getChildren().clear();
            shapeList.clearShapes();
            shapeList = Deserializer.deserialize("ShapeBluePrintListInfo.ser");
            for (Shape shape : shapeList.getShapes()) {
                dragController = new DragController(shape, true, toolBar.getHeight());
                dragController.isDraggableProperty().bind(editModeCheckbox.selectedProperty().not());
                editController = new EditController(shape, true, picker);
                editController.isEditableProperty().bind(editModeCheckbox.selectedProperty());
                displayPane.getChildren().add(shape);
            }
        }
        catch (IOException e){
            Alert errorAlert = new Alert(Alert.AlertType.ERROR);
            errorAlert.setTitle("IOException");
            errorAlert.setContentText(e.getMessage());
            errorAlert.show();
        }
        catch (ClassNotFoundException e){
            Alert errorAlert = new Alert(Alert.AlertType.ERROR);
            errorAlert.setTitle("ClassNotFoundException");
            errorAlert.setContentText(e.getMessage());
            errorAlert.show();
        }
    }

    /***
     *
     * @param mouseEvent clicking a mouse on the pane
     */
    public void drawWithPoints(MouseEvent mouseEvent) {
        if (mouseEvent.isControlDown()) {
            if (Double.isNaN(firstXclicked)) {

                displayPane.setOnMouseMoved(e->{
                    mouseX.set(e.getX());
                    mouseY.set(e.getY());
                });

                firstXclicked = mouseEvent.getX();
                firstYclicked = mouseEvent.getY();
                mouseLine = new Line();
                mouseLine.setStartX(firstXclicked);
                mouseLine.setStartY(firstYclicked);
                mouseX.set(mouseEvent.getSceneX());
                mouseY.set(mouseEvent.getSceneY() - toolBar.getHeight());
                mouseLine.endXProperty().bind(mouseX);
                mouseLine.endYProperty().bind(mouseY);

                displayPane.getChildren().add(mouseLine);
                System.out.println(String.format("%.2f, %.2f", firstXclicked, firstYclicked));

            } else {
                secondXclicked = mouseEvent.getX();
                secondYclicked = mouseEvent.getY();

                mouseLine.endXProperty().unbind();
                mouseLine.endYProperty().unbind();
                displayPane.getChildren().remove(mouseLine);

                System.out.println(String.format("secondXClicked: %.2f, secondYclicked: %.2f", secondXclicked, secondYclicked));
                System.out.println(String.format("%.2f, %.2f", firstXclicked, firstYclicked));

                if(!editModeCheckbox.isSelected()) {
                    Color myColor = picker.getValue();
                    Shape shape = drawer.draw(firstXclicked, firstYclicked, secondXclicked, secondYclicked, String.valueOf(myColor));

                    dragController = new DragController(shape, true, toolBar.getHeight());
                    dragController.isDraggableProperty().bind(editModeCheckbox.selectedProperty().not());
                    editController = new EditController(shape, true, picker);
                    editController.isEditableProperty().bind(editModeCheckbox.selectedProperty());

                    MyLogger.logger.log(Level.FINE, "Shape generated with points: " + firstXclicked + " " + firstYclicked + " " + secondXclicked + " " + secondYclicked);
                    displayPane.getChildren().remove(mouseLine);
                    shapeList.addShape(shape);
                    displayPane.getChildren().add(shape);
                }

                firstXclicked = Double.NaN;
                firstYclicked = Double.NaN;
                secondXclicked = Double.NaN;
                secondYclicked = Double.NaN;
            }
        }

    }
}

