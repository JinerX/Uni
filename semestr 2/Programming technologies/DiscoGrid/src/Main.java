import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.util.logging.ConsoleHandler;


public class Main extends Application {
    public static void main(String[] args) {
        ConsoleHandler ch = new ConsoleHandler();
        MyLogger.loggerConfig();
        launch(args);
    }

    @Override
    public void start(Stage stage) throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("DiscoGridGUI.fxml"));
        Parent root = fxmlLoader.load();
        ((DiscoGridGUIController) fxmlLoader.getController()).setStage(stage);
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.setTitle("DiscoGrid");
        stage.show();
    }
}