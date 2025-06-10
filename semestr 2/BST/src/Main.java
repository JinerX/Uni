import parsers.DoubleParser;
import parsers.IntegerParser;
import parsers.StringParser;

import javax.swing.*;
import java.util.Scanner;
import java.util.logging.ConsoleHandler;

public class Main {
    public static void main(String[] args) {
        ConsoleHandler handler = new ConsoleHandler();
        MyLogger.loggerConfig();
        System.out.println("What type do you want the tree to have?");
        Scanner scanner = new Scanner(System.in);
        char type = scanner.next().charAt(0);

        switch (type) {
            case 'i':
                CLI<Integer> cliInt = new CLI<Integer>(new IntegerParser());
                cliInt.start();
                break;
            case 'd':
                CLI<Double> cliDouble = new CLI<Double>(new DoubleParser());
                cliDouble.start();
                break;
            case 's':
                CLI<String> cliString = new CLI<String>(new StringParser());
                cliString.start();
                break;
            default:
                System.out.println("Invalid type");
        }
        scanner.close();
    }
}