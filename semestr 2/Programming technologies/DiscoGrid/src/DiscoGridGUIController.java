/**
 * Sample Skeleton for 'DiscoGridGUI.fxml' Controller Class
 */

import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;
import java.util.function.LongToDoubleFunction;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

import javax.swing.*;


public class DiscoGridGUIController
{
    private GridManager gridManager = null;
    private Object locker = new Object();
    private Stage stage;
    private int rows;
    private int cols;
    private long speed;
    private double probability;

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="colInput"
    private TextField colInput; // Value injected by FXMLLoader

    @FXML // fx:id="emptySpace"
    private ScrollPane emptySpace; // Value injected by FXMLLoader

    @FXML // fx:id="probInput"
    private TextField probInput; // Value injected by FXMLLoader

    @FXML // fx:id="rowInput"
    private TextField rowInput; // Value injected by FXMLLoader

    @FXML // fx:id="speedInput"
    private TextField speedInput; // Value injected by FXMLLoader

    @FXML // fx:id="startButton"
    private Button startButton; // Value injected by FXMLLoader

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert colInput != null : "fx:id=\"colInput\" was not injected: check your FXML file 'DiscoGridGUI.fxml'.";
        assert emptySpace != null : "fx:id=\"emptySpace\" was not injected: check your FXML file 'DiscoGridGUI.fxml'.";
        assert probInput != null : "fx:id=\"probInput\" was not injected: check your FXML file 'DiscoGridGUI.fxml'.";
        assert rowInput != null : "fx:id=\"rowInput\" was not injected: check your FXML file 'DiscoGridGUI.fxml'.";
        assert speedInput != null : "fx:id=\"speedInput\" was not injected: check your FXML file 'DiscoGridGUI.fxml'.";
        assert startButton != null : "fx:id=\"startButton\" was not injected: check your FXML file 'DiscoGridGUI.fxml'.";
    }

    /***
     *
     * @param event - button pressed to start simulation
     */
    public void start(ActionEvent event){
        gridManager = new GridManager(locker);
        String srow = rowInput.getText();
        String scol = colInput.getText();
        String sprob = probInput.getText();
        String sspeed = speedInput.getText();

        if(Verifiers.verifyRow(srow)&&Verifiers.verifyColumn(scol)&&Verifiers.verifyProbability(sprob)&&Verifiers.verifySpeed(sspeed)){
            GridPane grid = gridManager.getGrid(Integer.parseInt(srow),   Integer.parseInt(scol), Long.parseLong(sspeed), Double.parseDouble(sprob));
            emptySpace.setContent(grid);
        }
    }

    /***
     *
     * @param stage - primary stage of the app
     */
    public void setStage(Stage stage){
        this.stage = stage;
        stage.setOnCloseRequest(new EventHandler<WindowEvent>() {

            @Override
            public void handle(WindowEvent event) {
                if(gridManager != null) {
                    for (ArrayList<MyThread> threadRow : gridManager.getThreadRows()) {
                        for (MyThread thread : threadRow) {
                            thread.setShouldStop(true);
                        }
                    }
                }
            }
        });
    }
}
