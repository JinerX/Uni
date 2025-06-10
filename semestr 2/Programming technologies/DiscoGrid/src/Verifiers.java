import javafx.scene.control.Alert;

public class Verifiers {
    /***
     *
     * @param row - a string to check if is proper
     * @return - boolean
     */
    public static boolean verifyRow(String row) {
        int x = -1;
        try{
            x = Integer.parseInt(row);
        }
        catch(NumberFormatException e){
            Alert a =  new Alert(Alert.AlertType.ERROR);
            a.setTitle("Error");
            a.setHeaderText(null);
            a.setContentText(row + "is not a valid input for row");
            a.show();
            return false;
        }
        if(x<=0){
            Alert a =  new Alert(Alert.AlertType.ERROR);
            a.setTitle("Error");
            a.setHeaderText(null);
            a.setContentText(row + "is not a valid input for row");
            a.show();
            return false;
        }
        return true;
    }

    /***
     *
     * @param column - a string to check if is proper
     * @return - a boolean
     */
    public static boolean verifyColumn(String column) {
        int x = -1;
        try{
            x = Integer.parseInt(column);
        }
        catch(NumberFormatException e){
            Alert a =  new Alert(Alert.AlertType.ERROR);
            a.setTitle("Error");
            a.setHeaderText(null);
            a.setContentText(column + "is not a valid input for column");
            a.show();
            return false;
        }
        if(x<=0){
            Alert a =  new Alert(Alert.AlertType.ERROR);
            a.setTitle("Error");
            a.setHeaderText(null);
            a.setContentText(column + "is not a valid input for column");
            a.show();
            return false;
        }
        return true;
    }


    /***
     *
     * @param speed - a string to check if is proper
     * @return - a boolean
     */
    public static boolean verifySpeed(String speed) {
        long x = -1;
        try{
            x = Long.parseLong(speed);
        }
        catch(NumberFormatException e){
            Alert a =  new Alert(Alert.AlertType.ERROR);
            a.setTitle("Error");
            a.setHeaderText(null);
            a.setContentText(speed + "is not a valid input for speed");
            a.show();
            return false;
        }
        if(x<=0){
            Alert a =  new Alert(Alert.AlertType.ERROR);
            a.setTitle("Error");
            a.setHeaderText(null);
            a.setContentText(speed + "is not a valid input for speed");
            a.show();
            return false;
        }
        return true;
    }

    /***
     *
     * @param probability - a string to check if is proper
     * @return - a boolean
     */
    public static boolean verifyProbability(String probability) {
        double x;
        try{
            x = Double.parseDouble(probability);
        }
        catch(NumberFormatException e){
            Alert a =  new Alert(Alert.AlertType.ERROR);
            a.setTitle("Error");
            a.setHeaderText(null);
            a.setContentText(probability + "is not a valid input for probability");
            a.show();
            return false;
        }
        if(x<0 || x>1) {
            Alert a =  new Alert(Alert.AlertType.ERROR);
            a.setTitle("Error");
            a.setHeaderText(null);
            a.setContentText(probability + "is not a valid input for probability, the probability needs to be between 0 and 1");
            a.show();
            return false;
        }
        return true;
    }
}
