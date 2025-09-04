public class Overloading{
    public static void main(String[] args) {
        sum(5);
        sum("Ankit");
        sum(5,10);
    }
    static void sum(int a){
      System.out.println("Alone:- akela badsha\n");
    }
    static void sum(String b){
      System.out.println("mera naam hi kaafi hai\n");
    }
    static void sum(int b,int c){
     System.out.println("Two number wale aa gaye hai");
    }
}