package Controllers;

import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.event.EventHandler;
import javafx.scene.Node;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Shape;

import static java.lang.Math.cos;
import static java.lang.Math.sin;

public class DragController {
    private final Node target;
    private double anchorX;
    private double anchorY;
    private double mouseOffsetFromNodeZeroX;
    private double mouseOffsetFromNodeZeroY;
    private EventHandler<MouseEvent> setAnchor;
    private EventHandler<MouseEvent> updatePositionOnDrag;
    private EventHandler<MouseEvent> commitPositionOnRelease;
    private final int ACTIVE = 1;
    private final int INACTIVE = 0;
    private int cycleStatus = INACTIVE;
    private BooleanProperty isDraggable;
    private double toolBarHeight;

    /**
     * @param target a node that will be made draggable
     */
    public DragController(Node target) {
        this(target, false,0);
    }

    /***
     *
     * @param target a node that will be made draggable
     * @param isDraggable a boolean whether or not to make a node draggable
     */
    public DragController(Node target, boolean isDraggable){
        this(target,isDraggable,0);
    }

    /***
     *
     * @param target a node that will be made draggable
     * @param isDraggable a boolean whether or not to make a node draggable
     * @param toolBarHeight height of toolbar to adjust height
     */
    public DragController(Node target, boolean isDraggable,double toolBarHeight) {
        this.target = target;
        this.toolBarHeight = toolBarHeight;
        createHandlers();
        createDraggableProperty();
        this.isDraggable.set(isDraggable);
    }

    private void createHandlers() {
        setAnchor = event -> {
            if (event.isPrimaryButtonDown()) {
                cycleStatus = ACTIVE;
                anchorX = event.getSceneX();
                anchorY = event.getSceneY();
                mouseOffsetFromNodeZeroX = event.getX();
                mouseOffsetFromNodeZeroY = event.getY();
                ((Shape) target).setStroke(Color.RED);
            }
            if (event.isSecondaryButtonDown()) {
                cycleStatus = INACTIVE;
                target.setTranslateX(0);
                target.setTranslateY(0);
                ((Shape) target).setStroke(Color.BLACK);
            }
        };
        updatePositionOnDrag = event -> {
            if (cycleStatus != INACTIVE) {
                target.setTranslateX(event.getSceneX() - anchorX);
                target.setTranslateY(event.getSceneY() - anchorY);
            }
        };
        commitPositionOnRelease = event -> {
            if (cycleStatus != INACTIVE) {
                //commit changes to LayoutX and LayoutY
                target.setLayoutX((event.getSceneX() - mouseOffsetFromNodeZeroX - 1));
                target.setLayoutY((event.getSceneY() - mouseOffsetFromNodeZeroY - toolBarHeight - 1));
                //clear changes from TranslateX and TranslateY
                target.setTranslateX(0);
                target.setTranslateY(0);
                ((Shape) target).setStroke(Color.BLACK);
            }
        };
    }

    public void createDraggableProperty() {
        isDraggable = new SimpleBooleanProperty();
        isDraggable.addListener((observable, oldValue, newValue) -> {
            if (newValue) {
                target.addEventFilter(MouseEvent.MOUSE_PRESSED, setAnchor);
                target.addEventFilter(MouseEvent.MOUSE_DRAGGED, updatePositionOnDrag);
                target.addEventFilter(MouseEvent.MOUSE_RELEASED, commitPositionOnRelease);
            } else {
                target.removeEventFilter(MouseEvent.MOUSE_PRESSED, setAnchor);
                target.removeEventFilter(MouseEvent.MOUSE_DRAGGED, updatePositionOnDrag);
                target.removeEventFilter(MouseEvent.MOUSE_RELEASED, commitPositionOnRelease);
            }
        });
    }

    /**
     *
     * @return boolean value specifying whether the object is draggable
     */
    public boolean isIsDraggable() {
        return isDraggable.get();
    }
    public BooleanProperty isDraggableProperty() {
        return isDraggable;
    }
}