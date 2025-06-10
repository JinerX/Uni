/**
 * Sample Skeleton for 'PaintAppGUI.fxml' Controller Class
 */

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ColorPicker;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;

public class PaintAppGUIController{

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="circleButton"
    private Button circleButton; // Value injected by FXMLLoader

    @FXML // fx:id="clearButton"
    private Button clearButton; // Value injected by FXMLLoader

    @FXML // fx:id="displayPane"
    private Pane displayPane; // Value injected by FXMLLoader

    @FXML // fx:id="editModeCheckbox"
    private CheckBox editModeCheckbox; // Value injected by FXMLLoader

    @FXML // fx:id="picker"
    private ColorPicker picker; // Value injected by FXMLLoader

    @FXML // fx:id="rectangleButton"
    private Button rectangleButton; // Value injected by FXMLLoader

    @FXML // fx:id="triangleButton"
    private Button triangleButton; // Value injected by FXMLLoader

    @FXML
    void circleButtonPressed(ActionEvent event) {

    }

    @FXML
    void drawWithPoints(MouseEvent event) {

    }

    @FXML
    void rectangleButtonPressed(ActionEvent event) {

    }

    @FXML
    void triangleButtonPressed(ActionEvent event) {

    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert circleButton != null : "fx:id=\"circleButton\" was not injected: check your FXML file 'PaintAppGUI.fxml'.";
        assert clearButton != null : "fx:id=\"clearButton\" was not injected: check your FXML file 'PaintAppGUI.fxml'.";
        assert displayPane != null : "fx:id=\"displayPane\" was not injected: check your FXML file 'PaintAppGUI.fxml'.";
        assert editModeCheckbox != null : "fx:id=\"editModeCheckbox\" was not injected: check your FXML file 'PaintAppGUI.fxml'.";
        assert picker != null : "fx:id=\"picker\" was not injected: check your FXML file 'PaintAppGUI.fxml'.";
        assert rectangleButton != null : "fx:id=\"rectangleButton\" was not injected: check your FXML file 'PaintAppGUI.fxml'.";
        assert triangleButton != null : "fx:id=\"triangleButton\" was not injected: check your FXML file 'PaintAppGUI.fxml'.";

    }

}
