import java.util.*;
import java.io.*;

class TreeNode {
    int data;
    TreeNode left, right;

    TreeNode(int val) {
        data = val;
    }
}

// BSTIterator class with in-order traversal and next/prev functionality
class BSTIterator {
    private List<Integer> inorder; // List to store the inorder traversal of the tree
    private int index; // Index to keep track of the current position in the inorder list

    // Helper function to perform in-order traversal
    private void inOrder(TreeNode root) {
        if (root == null)
            return;
        inOrder(root.left); // Traverse left subtree
        inorder.add(root.data); // Visit the node
        inOrder(root.right); // Traverse right subtree
    }

    public BSTIterator(TreeNode root) {
        inorder = new ArrayList<>();
        inOrder(root);
        index = -1; // Initially before the first element
    }

    public boolean hasNext() {
        return index + 1 < inorder.size();
    }

    public int next() {
        if (hasNext()) {
            index++;
            return inorder.get(index);
        }
        return -1; // Should never be called if hasNext() is false
    }

    public boolean hasPrev() {
        return index - 1 >= 0;
    }

    public int prev() {
        if (hasPrev()) {
            index--;
            return inorder.get(index);
        }
        return -1; // Should never be called if hasPrev() is false
    }
}

public class Main {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int t = Integer.parseInt(sc.nextLine().trim());

        while (t-- > 0) {

            String testCase = sc.nextLine();
            System.out.println(testCase);

            int n = Integer.parseInt(sc.nextLine().trim());

            String methodsLine = sc.nextLine();
            String[] functionCalls = methodsLine.trim().split("\\s+");

            String treeData = sc.nextLine();

            PrintStream originalOut = System.out;
            ByteArrayOutputStream logBuffer = new ByteArrayOutputStream();
            PrintStream logStream = new PrintStream(logBuffer);
            System.setOut(logStream);

            TreeNode root = null;
            BSTIterator obj = null;

            String[] res = new String[n];

            for (int i = 0; i < n; i++) {
                String func = functionCalls[i];

                switch (func) {
                    case "BSTIterator":
                        root = buildTree(treeData);
                        obj = new BSTIterator(root);
                        res[i] = "null";
                        break;

                    case "next":
                        res[i] = String.valueOf(obj.next());
                        break;

                    case "prev":
                        res[i] = String.valueOf(obj.prev());
                        break;

                    case "hasNext":
                        res[i] = String.valueOf(obj.hasNext());
                        break;

                    case "hasPrev":
                        res[i] = String.valueOf(obj.hasPrev());
                        break;

                    default:
                        System.out.println("Invalid function call: " + func);
                        return;
                }
            }

            System.out.flush();
            System.setOut(originalOut);
            System.err.println(testCase);
            System.err.print(logBuffer.toString());

            for (int i = 0; i < n; i++) {
                System.out.print(res[i]);

                if (i != n - 1) {
                    System.out.print(" ");
                }
            }

            System.out.println();
        }

        sc.close();
    }

    private static TreeNode buildTree(String s) {
        // Check if the string is null, empty, or only whitespace
        if (s == null || s.trim().isEmpty()) {
            return null;
        }

        // Trim the string to remove leading and trailing whitespaces and split by
        // spaces
        s = s.trim();
        String[] nodes = s.split("\\s+"); // Use \\s+ to split on one or more whitespace characters

        if (nodes.length == 0 || nodes[0].isEmpty()) {
            return null;
        }

        TreeNode root = new TreeNode(Integer.parseInt(nodes[0]));
        Queue<TreeNode> q = new LinkedList<>();
        q.add(root);

        int ind = 1;
        while (!q.isEmpty() && ind < nodes.length) {
            TreeNode currNode = q.poll();

            // Check and assign left child
            if (ind < nodes.length && !nodes[ind].isEmpty() && !nodes[ind].equalsIgnoreCase("n")
                    && !nodes[ind].equalsIgnoreCase("null")) {
                currNode.left = new TreeNode(Integer.parseInt(nodes[ind]));
                q.add(currNode.left);
            }
            ind++;

            // Check and assign right child
            if (ind < nodes.length && !nodes[ind].isEmpty() && !nodes[ind].equalsIgnoreCase("n")
                    && !nodes[ind].equalsIgnoreCase("null")) {
                currNode.right = new TreeNode(Integer.parseInt(nodes[ind]));
                q.add(currNode.right);
            }
            ind++;
        }

        return root;
    }
}
