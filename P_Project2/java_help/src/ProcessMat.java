import java.io.*;
import java.util.Scanner;

public class ProcessMat {
    public static void main(String[] args) {
        try (final Scanner fileInput = new Scanner(new BufferedInputStream(new FileInputStream("../data/mat-A-32.txt")));
        ){
//            fileInput.useDelimiter("");
            while(fileInput.hasNextLine()){
                System.out.println(fileInput.nextLine().replace(' ', ','));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
