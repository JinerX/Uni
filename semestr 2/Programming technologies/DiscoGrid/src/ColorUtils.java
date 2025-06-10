import javafx.scene.paint.Color;

import java.util.ArrayList;


public class ColorUtils {
    /***
     *
     * @return - a random color
     */
    public static Color getRandomColor(){
        return Color.color(Math.random(), Math.random(), Math.random());
    }

    /**
     *
     * @param colors - ArrayList of colors of which we want to compute average
     * @return - average color
     */
    public static Color getAverageColor(ArrayList<Color> colors){
        double r=0;
        double g=0;
        double b=0;
        for(Color c : colors){
            r+=c.getRed();
            g+=c.getGreen();
            b+=c.getBlue();
        }
        r/=colors.size();
        g/=colors.size();
        b/=colors.size();
        return Color.color(r,g,b);
    }
}
