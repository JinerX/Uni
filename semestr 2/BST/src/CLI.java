import parsers.Parser;

import java.util.Scanner;

public class CLI<T extends Comparable<T>> {
    public BST<T> tree;
    public Parser<T> parser;
    public boolean running = false;

    public CLI(Parser<T> p){
        parser = p;
        tree = new BST<T>();
    }

    public void start(){
        running = true;
        Scanner scanner = new Scanner(System.in);
        while(running){
            System.out.print("enter command: ");
            String command = null;

            try{
                command = scanner.nextLine();
            }
            catch(Exception e){
                System.out.println(e.getMessage());
                System.exit(0);
            }

            if(command.equals("exit")){
                running = false;
                System.exit(0);
            }

            else if(command.equals("p")){
                System.out.println(tree);
            }

            String[] commands = command.split(" ");
            if(commands.length !=2){
                System.out.println("Invalid command");
                continue;
            }

            String op = commands[0];
            T value = null;
            try{
                value = parser.parse(commands[1]);
            }
            catch(Exception e){
                System.out.println("Invalid value");
                continue;
            }

            switch(op){
                case "a":
                case "i":
                    tree.insert(value);
                    System.out.println(tree);
                    break;
                case "d":
                case "r":
                    try {
                        tree.delete(value);
                        System.out.println(tree);
                    }
                    catch(Exception e){
                        System.out.println(e.getMessage());
                    }
                    break;
                case "s":
                case "f":
                    System.out.println(tree.search(value));
                    break;
                default:
                    System.out.println("Invalid command");
            }
        }
        scanner.close();
    }
}
