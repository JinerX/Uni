//import Controllers.MouseController;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.shape.Shape;
import javafx.stage.Stage;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;

/**
 *
 */
public class Main extends Application {
    public static void main(String[] args) {
        ConsoleHandler ch = new ConsoleHandler();
        MyLogger.loggerConfig();
        launch(args);
    }

    /***
     *
     * @param stage stage of the app
     */
    @Override
    public void start(Stage stage){
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("PaintAppGUI.fxml"));
            Parent root = fxmlLoader.load();
            Scene scene = new Scene(root);
            stage.setScene(scene);
            stage.setTitle("PaintApp");
            MyLogger.logger.log(Level.FINE, "PaintApp started");
            stage.show();
        }
        catch (IOException ex) {
            Alert errorAlert = new Alert(Alert.AlertType.ERROR);
            errorAlert.setTitle("IOException");
            errorAlert.setContentText(ex.getMessage());
            errorAlert.show();
        }
        catch(Exception ex) {
            Alert errorAlert = new Alert(Alert.AlertType.ERROR);
            errorAlert.setTitle("Exception");
            errorAlert.setContentText(ex.getMessage());
            errorAlert.show();
        }
    }
}