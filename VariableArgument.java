import java.util.Arrays;

public class VariableArgument{
    public static void main(String[] args) {
        sum(50,33,5,3,34);
        name("ankit","Isha","Rahul");
    }
    static void sum(int... ankit){
        //this is used to print the array of all element
    System.out.println(Arrays.toString(ankit));
    //this is have variable n which store the data elemnet of ankit array one by one
      for (int n : ankit) {
        //this print the element one by one
    System.out.print(n + " ");
    System.out.print("");
}
    }

    static void name(String... ak){
       System.out.println(Arrays.toString(ak));
       for(String naam:ak){
        System.out.print(naam + " ");
       }
    }
}