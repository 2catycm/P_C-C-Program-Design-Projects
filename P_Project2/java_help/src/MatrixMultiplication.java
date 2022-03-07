import java.io.*;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Scanner;

class Matrix /*<E extends Number>*/{
    private ArrayList<ArrayList<BigDecimal>> data = new ArrayList<>(guessSize);
    public Matrix multiply(Matrix other){
        if(this.getColumnCount()!=other.getColumnCount())
            throw new InputMismatchException("Matrix multiplication of whose sizes do not match is not supported.");
        ArrayList<ArrayList<BigDecimal>> result = new ArrayList<>(this.getRowCount());
        for (int i = 0; i < this.getRowCount(); i++) {
            ArrayList<BigDecimal> row = new ArrayList<>(other.getColumnCount());
            for (int k = 0; k < this.getColumnCount(); ++k){
                BigDecimal temp = this.data.get(i).get(k);
                for (int j = 0; j < other.getColumnCount(); ++j) {
                    row.add(BigDecimal.ZERO);
                    row.set(j, row.get(j).add(temp.multiply(other.data.get(k).get(j))));
                }
            }
            result.add(row);
        }
        return new Matrix(result);
    }

    private Matrix(ArrayList<ArrayList<BigDecimal>> data) {
        this.data = data;
    }

    public Matrix(String filename) {
        try (final FileReader fileReader = new FileReader(filename)){
            final StreamTokenizer streamTokenizer = new StreamTokenizer(new BufferedReader(fileReader));
            streamTokenizer.eolIsSignificant(true);//end of line is significant
            ArrayList<ArrayList<BigDecimal>> lines = new ArrayList<>(guessSize);
            while(streamTokenizer.nextToken() != StreamTokenizer.TT_EOF){
                final ArrayList<BigDecimal> line = new ArrayList<>(guessSize);
                do {
                    line.add(new BigDecimal((int)(streamTokenizer.nval*10)).divide(BigDecimal.TEN));
                }while (streamTokenizer.nextToken()!=StreamTokenizer.TT_EOL);
                lines.add(line);
            }
            this.data = lines;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void writeToFile(String filename){
        try (final PrintWriter writer = new PrintWriter(new BufferedWriter(new FileWriter(filename)))){
            for (int i = 0; i < getRowCount(); i++) {
                for (int j = 0; j < getColumnCount(); j++) {
                    writer.printf("%.1f ",get(i,j));
                }
                writer.println();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public BigDecimal get(int i, int j){
        return data.get(i).get(j);
    }
    public BigDecimal set(int i, int j, BigDecimal val){
        return data.get(i).set(j, val);
    }
    public int getRowCount(){
        return data.size();
    }
    public int getColumnCount(){
        return data.get(0).size();
    }
    public ArrayList<ArrayList<BigDecimal>> getData() {
        return data;
    }

    @Override
    public String toString() {
        return "Matrix{" +
                "data=" + data +
                '}';
    }

    private static int guessSize = 2048;
}
public class MatrixMultiplication {
    private static int size = 2048;
    public static void main(String[] args) {
        Matrix a = new Matrix(String.format("../data/mat-A-%d.txt", size));
        Matrix b = new Matrix(String.format("../data/mat-B-%d.txt", size));
        a.multiply(b).writeToFile(String.format("../data/mat-C-%d_accurate_by_java.txt", size));
    }
}
