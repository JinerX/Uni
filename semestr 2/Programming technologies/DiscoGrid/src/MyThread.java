import javafx.scene.paint.Color;

import java.util.ArrayList;

public class MyThread extends Thread{
    private Object locker;
    private long speed;
    private double probability = 1.0;
    private Color color;
    private Cell cell;
    private ArrayList<MyThread> neighbours;
    private int row;
    private int col;
    private boolean isActive = true;
    private boolean shouldStop = false;


    /***
     *
     * @param color - a color of which will be the cell operated by this thread
     * @param locker - an object to synchronize
     * @param speed - speed of simulation
     * @param probability - likelihood of changing to a random color
     * @param row - the row of the cell which the thread oversees
     * @param col - the column of the cell which the thread oversees
     */
    public MyThread(Color color,Object locker, long speed, double probability, int row, int col) {
        this.color = color;
        this.locker = locker;
        this.speed = speed;
        this.probability = probability;
        this.neighbours = new ArrayList<>();
        this.row = row;
        this.col = col;
    }

    /**
     *
     * @param cell - a cell to oversee
     */
    public void setCell(Cell cell) {
        this.cell = cell;
    }

    public synchronized void updateCell(){
        if(cell != null){
            cell.setColor(this.color);
        }
    }

    /**
     *
     * @param myThread - neighbour to be added
     */
    public void addNeighbour(MyThread myThread){
        neighbours.add(myThread);
    }

    public synchronized Color getColor(){
        return color;
    }

    public synchronized Color getNeighbourColor(){
        ArrayList<Color> colors = new ArrayList<Color>();
        for(MyThread neighbour : neighbours){
            colors.add(neighbour.getColor());
        }

        return(ColorUtils.getAverageColor(colors));
    }

    public boolean isActive(){
        return isActive;
    }

    /***
     *
     * @param active whether or not the thread is active
     */
    public void setActive(boolean active){
        isActive = active;
    }

    public int getRow(){
        return row;
    }
    public int getCol(){
        return col;
    }

    /**
     *
     * @param shouldStop - setting the shouldStop value
     */
    public void setShouldStop(boolean shouldStop){
        this.shouldStop = shouldStop;
    }



    public void changecolor() {
        synchronized (locker) {
            synchronized (this) {
                synchronized (neighbours.get(0)) {
                    synchronized (neighbours.get(1)) {
                        synchronized (neighbours.get(2)) {
                            synchronized (neighbours.get(3)) {
                                if(Math.random() < probability){
                                    this.color = ColorUtils.getRandomColor();
                                    this.updateCell();
                                }
                                else {
                                    this.color = getNeighbourColor();
                                    this.updateCell();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    public void run() {
        while(isActive()) {
            if(shouldStop){
                return;
            }
            changecolor();
            try {
                sleep((long) (speed * (0.5 + Math.random())));
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}


