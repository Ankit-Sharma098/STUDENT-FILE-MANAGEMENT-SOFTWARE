import java.util.Arrays;

public class Array{
    public static void main(String[] args) {
        int[] num={1,2,3,4,5}; 
        change(num);
        System.out.println(Arrays.toString(num));
    }
    static void change(int[] num){
       num[0]=99;
    }
}