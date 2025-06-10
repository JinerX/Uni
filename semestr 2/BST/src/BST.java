import javax.management.InvalidAttributeValueException;
import java.util.logging.Level;

public class BST <T extends Comparable<T>> {
    Node<T> root;

    private class Node<T extends Comparable<T>> {
        public T data;
        public Node<T> left = null;
        public Node<T> right = null;
        public Node<T> parent;

        public Node(T data, Node<T> parent){
            this.data = data;
            this.parent = parent;
        }

        public Node(T data){
            this(data, null);
        }


        public void setChild(T value){
            if(value == null){
                MyLogger.logger.log(Level.WARNING,"the value is null");
            }
            else if(data.compareTo(value) <= 0){
                if(right != null){
                    MyLogger.logger.log(Level.WARNING,"node already has a right child");
                }
                else{
                    right = new Node<T>(value,this);
//                    System.out.println("right val: " + value);
//                    System.out.println("Parent val: " + this.data);
                }
            }
            else if(data.compareTo(value) > 0){
                if(left != null){
                    MyLogger.logger.log(Level.WARNING,"node already has a left child");
                }
                else{
                    left = new Node<T>(value,this);
//                    System.out.println("left val: " + value);
//                    System.out.println("Parent val: " + this.data);
                }
            }
        }

        public void nodeSetter(Node<T> node){
            if(node.parent.left == node){
                node.parent.left = null;
            }
            if(node.parent.right == node){
                node.parent.right = null;
            }

            node.left = this.left;
            node.right = this.right;
            node.parent = this.parent;
            if(node.right != null) {
                node.right.parent = node;
            }
            if(node.left != null) {
                node.left.parent = node;
            }
        }

        public void replaceYourself(Node<T> node) throws Exception{
            if(node == null){
                MyLogger.logger.log(Level.WARNING,"node is null");
                return;
            }
            if(node.parent == null){
                return;
            }
            if(this.parent.left == this){
                if(this.parent.data.compareTo(node.data) < 0){
                    throw new InvalidAttributeValueException("Attempted appending a higher value as a left node");
                }

                nodeSetter(node);

                this.parent.left = node;
                this.parent = null;
            }
            else if(this.parent.right == this){
                if(this.parent.data.compareTo(node.data) > 0){
                    throw new InvalidAttributeValueException("Attempted appending a lower value as a right node");
                }

                nodeSetter(node);

                this.parent.right = node;
                this.parent = null;
            }
        }
    }


    public BST() {
        this.root = null;
    }

    public void insert(T data){
        if(root == null){
            root = new Node<T>(data);
        }

        else{
            Node<T> current = root;
            Node<T> parent = null;

            while(current != null){
                parent = current;
                if(data.compareTo(current.data) < 0){
                    current = current.left;
                }
                else if(data.compareTo(current.data) > 0){
                    current = current.right;
                }
                else if(data.compareTo(current.data) == 0){
                    parent = null;
                    break;
                }
            }
            if(parent != null) {
                parent.setChild(data);
            }
        }
    }

    public boolean search(T data){

        if(root == null){
            return false;
        }

        Node<T> current = root;
        while(current != null){
            if(data.compareTo(current.data) == 0){
                return true;
            }
            else if(data.compareTo(current.data) < 0){
                current = current.left;
            }
            else if(data.compareTo(current.data) > 0){
                current = current.right;
            }
        }
        return false;
    }

//    public Node<T> deleteRecursively(Node<T> root,T data){
//        if(root == null){
//            return root;
//        }
//
//        if(data.compareTo(root.data) < 0){
//            root.left = deleteRecursively(root.left, data);
//        }
//
//        else if(data.compareTo(root.data) > 0){
//            root.right = deleteRecursively(root.right, data);
//        }
//
//        else{
//            if(root.left == null){
//                return root.right;
//            }
//            else if(root.right == null){
//                return root.left;
//            }
//
//            root.data = minValue(root.right);
//
//            root.right = deleteRecursively(root.right, root.data);
//        }
//        return root;
//    }



    public Node<T> deleteRecursively(Node<T> root, T data) {
        if (root == null) {
            return root;
        }

        if (data.compareTo(root.data) < 0) {
            root.left = deleteRecursively(root.left, data);
        } else if (data.compareTo(root.data) > 0) {
            root.right = deleteRecursively(root.right, data);
        } else {
            // Node with only one child or no child
            if (root.left == null) {
                if (root == this.root) {  // Check if the node to be deleted is the root
                    this.root = root.right;
                }
                return root.right;
            } else if (root.right == null) {
                if (root == this.root) {  // Check if the node to be deleted is the root
                    this.root = root.left;
                }
                return root.left;
            }

            // Node with two children: Get the inorder successor (smallest in the right subtree)
            root.data = minValue(root.right);

            // Delete the inorder successor
            root.right = deleteRecursively(root.right, root.data);
        }
        return root;
    }


    T minValue(Node<T> root){
        T min = root.data;
        while(root.left != null){
            min = root.left.data;
            root = root.left;
        }
        return min;
    }

    public void delete(T data){
        deleteRecursively(root, data);
    }

    public String printRec(Node<T> node, int indent, boolean isRight){
        StringBuilder sb = new StringBuilder();
        if(node != null){
            sb.append(printRec(node.right, indent + 4, true));
            if(indent != 0){
                for(int i = 0; i < indent - 4; i++) sb.append(" ");
                sb.append(isRight ? " /" : " \\");
                sb.append("----- ");
            }
            sb.append(node.data).append("\n");
            sb.append(printRec(node.left, indent + 4, false));
        }
        return sb.toString();
    }


    @Override
    public String toString(){
        return printRec(root, 0, true);
    }
}
