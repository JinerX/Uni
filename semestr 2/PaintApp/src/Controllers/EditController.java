package Controllers;

import Shapes.IShape;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.event.EventHandler;
import javafx.scene.control.ColorPicker;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Shape;


public class EditController {
    private EventHandler<ScrollEvent> rotationHandler;
    private EventHandler<MouseEvent> typeOfEventEval;
    private EventHandler<ScrollEvent> scalingHandler;
    private EventHandler<MouseEvent> restartStatuses;
    private EventHandler<MouseEvent> changeColor;
    private Shape target;
    private double pivotX;
    private double pivotY;
    private final int ACTIVE = 1;
    private final int INACTIVE = 0;
    private int rotationStatus = INACTIVE;
    private int scalingStatus = INACTIVE;
    private int coloringStatus = INACTIVE;
    private BooleanProperty isEditable;
    private double currentScale = 1.0;
    private ColorPicker picker;

    /**
     *
     *
     * @param target node that will be made editable
     */
    public EditController(Shape target) {
        this(target,false, null);
    }

    /**
     *
     * @param target node that will be made editable
     * @param isEditable boolean specifying whether to made the node editable
     * @param picker ColorPicker that will be used with colorChange
     */
    public EditController(Shape target, boolean isEditable, ColorPicker picker){
        if(target instanceof IShape) {
            this.target = target;
            this.pivotX = ((IShape) target).getPivotXelo();
            this.pivotY = ((IShape) target).getPivotYelo();
            createHandlers();
            createEditableProperty();
            this.isEditable.set(isEditable);
            this.picker = picker;
        }
    }


    private void createHandlers(){
        typeOfEventEval = (event) ->{
            if(event.isPrimaryButtonDown()){
                rotationStatus = ACTIVE;
                scalingStatus = INACTIVE;
                coloringStatus = INACTIVE;
            }
            else if(event.isSecondaryButtonDown()){
                rotationStatus = INACTIVE;
                scalingStatus = ACTIVE;
                coloringStatus = INACTIVE;
            } else if (event.isMiddleButtonDown()) {
                coloringStatus = ACTIVE;
                rotationStatus = INACTIVE;
                scalingStatus = INACTIVE;
            }
        };

        changeColor = (event) ->{
            ((IShape) target).setColor(String.valueOf(picker.getValue()));
        };

        rotationHandler = (event) -> {
            if(rotationStatus == ACTIVE){
                ((IShape) target).rotate(0.2*event.getDeltaY());
            }
        };

        scalingHandler = (event) -> {
            if(scalingStatus == ACTIVE){
                double delta = 1;
                if(event.getDeltaY() > 1){
                    delta = 1.05;
                    ((IShape) target).scale(delta);
                    currentScale = currentScale*1.05;
                }
                else if(event.getDeltaY() < 1){
                    delta = 0.95;
                    ((IShape) target).scale(delta);
                    currentScale = currentScale*0.95;
                }
            }
        };

        restartStatuses = (event) -> {
            rotationStatus = INACTIVE;
            scalingStatus = INACTIVE;
            coloringStatus = INACTIVE;
        };
    }


    public void createEditableProperty() {
        isEditable = new SimpleBooleanProperty();
        isEditable.addListener((observable, oldValue, newValue) -> {
            if (newValue) {
                target.addEventFilter(MouseEvent.MOUSE_PRESSED, typeOfEventEval);
                target.addEventFilter(ScrollEvent.SCROLL, rotationHandler);
                target.addEventFilter(ScrollEvent.SCROLL, scalingHandler);
                target.addEventFilter(MouseEvent.MOUSE_RELEASED, restartStatuses);
                target.addEventFilter(MouseEvent.MOUSE_PRESSED,changeColor);
            }
        });
    }

    /**
     *
     *
     * @return a boolean specifying whether the node is editable
     */
    public boolean isIsEditable(){
        return isEditable.get();
    }

    /**
     *
     * @return a BooleanProperty
     */
    public BooleanProperty isEditableProperty(){
        return isEditable;
    }

}
