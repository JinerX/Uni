import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;

import java.util.ArrayList;

public class GridManager {
    private GridPane grid;
    private Object locker;
    private ArrayList<ArrayList<Cell>> cellRows;
    private ArrayList<ArrayList<MyThread>> threadRows;


    /***
     *
     * @param locker - object used for synchronizing the threads
     */
    GridManager(Object locker) {
        this.locker = locker;
        cellRows = new ArrayList<>();
        threadRows = new ArrayList<>();
    }

    /***
     *
     * @return ArrayList of ArrayLists of Threads
     */
    public ArrayList<ArrayList<MyThread>> getThreadRows() {return threadRows;}

    /***
     *
     * @param rows - number of rows in grid
     * @param cols - number of cols in grid
     * @param speed - the speed of the simulation
     * @param probability - likelihood of changing color to random
     * @return - a grid
     */
    public GridPane getGrid(int rows, int cols, long speed, double probability) {
        GridPane grid = new GridPane();


        for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
            ArrayList<MyThread> threadRow = new ArrayList<MyThread>();

            //pętla tworząca thready
            for (int colIndex = 0; colIndex < cols; colIndex++) {
                MyThread thread = new MyThread(ColorUtils.getRandomColor(),locker,speed,probability, rowIndex, colIndex);
                threadRow.add(thread);
            }
            threadRows.add(threadRow);
        }

        //pętla dodająca threadom sąsiadów
        int rowSize = threadRows.getFirst().size();
        int colSize = threadRows.size();

        for(int rowIndex = 0; rowIndex < colSize; rowIndex++) {
            for(int colIndex = 0; colIndex < rowSize; colIndex++) {
                threadRows.get(rowIndex).get(colIndex).addNeighbour(threadRows.get(rowIndex).get((colIndex-1+rowSize)%rowSize)); //lewy
                threadRows.get(rowIndex).get(colIndex).addNeighbour(threadRows.get(rowIndex).get((colIndex+1)%rowSize)); //prawy
                threadRows.get(rowIndex).get(colIndex).addNeighbour(threadRows.get((rowIndex-1+colSize)%colSize).get(colIndex)); // dolny
                threadRows.get(rowIndex).get(colIndex).addNeighbour(threadRows.get((rowIndex+1)%colSize).get(colIndex)); // górny
            }
        }

        for(int rowIndex = 0; rowIndex < rows; rowIndex++) {
            ArrayList<Cell> cellRow = new ArrayList<Cell>();
            for(int colIndex = 0; colIndex < cols; colIndex++) {
                Cell pane = new Cell(threadRows.get(rowIndex).get(colIndex));
                //pane.setColor(ColorUtils.getRandomColor());
                cellRow.add(pane);
                GridPane.setHgrow(pane, Priority.ALWAYS);
                GridPane.setVgrow(pane, Priority.ALWAYS);
                grid.add(pane, colIndex, rowIndex);
            }
            this.cellRows.add(cellRow);
        }

        for(ArrayList<MyThread> threadRow : threadRows) {
            for(MyThread thread : threadRow) {
                thread.start();
            }
        }

        grid.setGridLinesVisible(true);
        return grid;
    }
}
