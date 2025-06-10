import javafx.application.Platform;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;


public class Cell extends Pane {

    private MyThread myThread;
    private Color color;
    private int row;
    private int col;

    private Runnable colorSetter = new Runnable() {
        @Override
        public void run() {
            setBackground(new Background(new BackgroundFill(color, null, null)));
        }
    };

    /**
     * @param myThread - a thread that is responsible for changing the colors
     */
    public Cell(MyThread myThread) {
        this.myThread = myThread;
        this.color = myThread.getColor();
        this.row = myThread.getRow();
        this.col = myThread.getCol();

        myThread.setCell(this);
        super.setBackground(new Background(new BackgroundFill(color, null, null)));
    }


    /**
     *
     * @param color - color to set
     */
    public void setColor(Color color) {
        this.color = color;
        Platform.runLater(colorSetter);
    }

}
