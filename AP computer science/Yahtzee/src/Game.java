
import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author sodeerik
 */
public class Game {

    static Scanner in = new Scanner(System.in);
    static Dice d = new Dice(6);
    static int full = -1;
    static int str4 = -1;
    static int str5 = -1;
    static int yahtzee = -1;
    static int chance = -1;
    static int kind3 = -1;
    static int kind4 = -1;
    static int bottomTotal = -1;
    static int one = -1;
    static int two = -1;
    static int three = -1;
    static int four = -1;
    static int five = -1;
    static int six = -1;
    static int topTotal = -1;
    static ArrayList<Integer> dice = new ArrayList<Integer>();
    static boolean worked;
    static int counter = 0;
    static int score = 0;

    public static void main(String[] args) {

        while (true) {



            worked = false;
            System.out.println("press enter to roll the dice");
            in.nextLine();
            dice.add(d.roll());//roll 5 times
            dice.add(d.roll());
            dice.add(d.roll());
            dice.add(d.roll());
            dice.add(d.roll());


            System.out.println("you rolled " + dice.get(0) + " , " + dice.get(1) + " , " +dice.get(2) + " , " + dice.get(3) + " , " + dice.get(4));
            System.out.println("if you would like to reroll any dice type 1. otherwise type 0.");
            if (Integer.parseInt(in.nextLine()) == 1) {
                reroll();
            }

            while (!worked) {
                System.out.println("what will you put this result in? ");
                System.out.println("(if you select an option that dosnt work that option will have a score of 0)");
                System.out.println("1=ones 2=twos 3=threes 4=fours 5=fives 6=sixes 7=full house");
                System.out.println("8=small straight 9= large straight 10= 3 of a  kind ");
                System.out.println("11= 4 of a kind 12= chance 13 = yahtzee");
                int choice = in.nextInt();

                enterResult(choice);
                if (!worked) {
                    System.out.println("NOT VALID. THAT OPTION IS ALREADY FILLED");
                }
                dice.clear();
            }
            counter++;

            if (counter >= 13) {
                break;
            }




        }
        System.out.println("total top score is " + topTotal);
        if (topTotal >= 63) {
            score += 35;
            System.out.println("you earned the 35 point bonus in the top section!");
        }
        System.out.println("total bottom score is " + bottomTotal);
        System.out.println("total score is " + score);
    }

    private static void enterResult(int choice) {
        if (choice == 1 && one < 0) {//1
            one = 0;
            for (int n = 0; n < 5; n++) {
                if (dice.get(n) == 1) {
                    one += 1;
                    topTotal += 1;
                    score += 1;
                }
            }
            worked = true;
        }


        if (choice == 2 && two < 0) {//2
            two = 0;
            for (int n = 0; n < 5; n++) {
                if (dice.get(n) == 2) {
                    two += 2;
                    topTotal += 2;
                    score += 2;
                }
            }
            worked = true;
        }


        if (choice == 3 && three < 0) {//3
            three = 0;
            for (int n = 0; n < 5; n++) {
                if (dice.get(n) == 3) {
                    three += 3;
                    topTotal += 3;
                    score += 3;
                }
            }
            worked = true;
        }


        if (choice == 4 && four < 0) {//4
            four = 0;
            for (int n = 0; n < 5; n++) {
                if (dice.get(n) == 4) {
                    four += 4;
                    topTotal += 4;
                    score += 4;
                }
            }
            worked = true;
        }


        if (choice == 5 && five < 0) {//5
            five = 0;
            for (int n = 0; n < 5; n++) {
                if (dice.get(n) == 5) {
                    five += 5;
                    topTotal += 5;
                    score += 5;
                }
            }
            worked = true;
        }


        if (choice == 6 && six < 0) {//6
            six = 0;
            for (int n = 0; n < 5; n++) {
                if (dice.get(n) == 6) {
                    six += 6;
                    topTotal += 6;
                    score += 6;
                }
            }
            worked = true;
        }


        if (choice == 7 && full < 0) {//fullhouse
            worked = true;
            full=0;
            int count = 1;
            int check1 = dice.get(0);
            for (int i = 0; i < 4; i++) {//start checking 3 are same;
                if (dice.get(i + 1) == check1) {
                    count++;
                } else {
                    break;
                }}
            if(count==3){
                for(int q=0;q<5;q++){
                    if(check1!=dice.get(q)){
                        for(int w = 0;w<5;w++){
                            if(dice.get(q)==dice.get(w)){
                                full=25;
                                score+=25;
                                bottomTotal+=25;        
                            }
                        }
                    }
                }
            }
            
            int count2 = 1;
            int check2 = dice.get(4);
            for (int i = 0; i < 4; i++) {
                if (dice.get(i) == check1) {
                    count++;
                } else {
                    break;
                }
            }
            if(count2==3){
                for(int q=0;q<5;q++){
                    if(check2!=dice.get(q)){
                        for(int w = 0;w<5;w++){
                            if(dice.get(q)==dice.get(w)){
                                full=25;
                                score+=25;
                                bottomTotal+=25;        
                            }
                        }
                    }
                }
            }
            int count3 = 0;
            if (dice.get(1) == dice.get(2) && dice.get(2) == dice.get(3)) {
                count3 = 3;
            }
            if(count3==3){
                if(dice.get(0)==dice.get(4)){
                    full=25;
                    score+=25;
                    bottomTotal+=25; 
                }
            }
            


        }


        if (choice == 8 && str4 < 0) {//4straight
            worked = true;
            str4=0;
            for (int i = 0; i < 5; i++) {//loop lowest
                int test = dice.get(i);
                for (int c = 0; c < 5; c++) {//loop second
                    int test2 = dice.get(c);
                    if (test2 == test + 1) {
                        for (int x = 0; x < 5; x++) {//loop third
                            int test3 = dice.get(x);
                            if (test3 == test2 + 1) {
                                for (int z = 0; z < 5; z++) {//loop fourth
                                    int test4 = dice.get(z);
                                    if (test4 == test3 + 1) {
                                        str4=30;
                                        score+=30;
                                        bottomTotal+=30;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        if (choice == 9 && str5 < 0) {//5straight
            worked = true;
            str5=0;
            for (int i = 0; i < 5; i++) {//loop lowest
                int test = dice.get(i);
                for (int c = 0; c < 5; c++) {//loop second
                    int test2 = dice.get(c);
                    if (test2 == test + 1) {
                        for (int x = 0; x < 5; x++) {//loop third
                            int test3 = dice.get(x);
                            if (test3 == test2 + 1) {
                                for (int z = 0; z < 5; z++) {//loop fourth
                                    int test4 = dice.get(z);
                                    if (test4 == test3 + 1) {
                                        for (int r = 0; r < 5; r++) {//loop fifth
                                            int test5 = dice.get(r);
                                            if (test5 == test4 + 1) {
                                                str5 = 50;
                                                bottomTotal += 40;
                                                score += 40;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        if (choice == 10 && kind3 < 0) {//3kind
            worked = true;
            kind3=0;
            int count = 1;
            int check1 = dice.get(0);
            for (int i = 0; i < 4; i++) {
                if (dice.get(i + 1) == check1) {
                    count++;
                } else {
                    break;
                }
            }
            int count2 = 1;
            int check2 = dice.get(4);
            for (int i = 0; i < 4; i++) {
                if (dice.get(i) == check1) {
                    count++;
                } else {
                    break;
                }
            }
            int count3 = 0;
            if (dice.get(1) == dice.get(2) && dice.get(2) == dice.get(3)) {
                count3 = 3;
            }
            if (count >= 3 || count2 >= 3 || count3 >= 3) {
                kind3 = (dice.get(0) + dice.get(1) + dice.get(2) + dice.get(3) + dice.get(4));
                score += kind3;
                bottomTotal += kind3;
            }
        }


        if (choice == 11 && kind4 < 0) {//4kind
            worked = true;
            kind4=0;
            int count = 1;
            int check1 = dice.get(0);
            for (int i = 0; i < 4; i++) {
                if (dice.get(i + 1) == check1) {
                    count++;
                } else {
                    break;
                }
            }
            int count2 = 1;
            int check2 = dice.get(4);
            for (int i = 0; i < 4; i++) {
                if (dice.get(i) == check1) {
                    count++;
                } else {
                    break;
                }
            }
            if (count >= 4 || count2 >= 4) {
                kind4 = (dice.get(0) + dice.get(1) + dice.get(2) + dice.get(3) + dice.get(4));
                score += kind4;
                bottomTotal += kind4;
            }
        }


        if (choice == 12 && chance < 0) {//chance
            worked = true;
            
            chance = (dice.get(0) + dice.get(1) + dice.get(2) + dice.get(3) + dice.get(4));
            score += chance;
            bottomTotal += chance;
        }


        if (choice == 13 && yahtzee < 0) {
            worked = true;
            yahtzee=0;
            if (dice.get(0) == dice.get(1) && dice.get(0) == dice.get(2) && dice.get(0) == dice.get(3) && dice.get(0) == dice.get(4)) {
                yahtzee = 50;
                score += 50;
                bottomTotal += 50;
            }
        }
    }

    static private void reroll() {//yahtzee
        int time = 1;
        ArrayList<Boolean> held = new ArrayList<Boolean>();
        held.add(false);
        held.add(false);
        held.add(false);
        held.add(false);
        held.add(false);
        System.out.println("would you like to hold die 1? 1=yes");
        if (in.nextInt() == 1) {
            held.set(0, true);
        }
        System.out.println("would you like to hold die 2? 1=yes");
        if (in.nextInt() == 1) {
            held.set(1, true);
        }
        System.out.println("would you like to hold die 3? 1=yes");
        if (in.nextInt() == 1) {
            held.set(2, true);
        }
        System.out.println("would you like to hold die 4? 1=yes");
        if (in.nextInt() == 1) {
            held.set(3, true);
        }
        System.out.println("would you like to hold die 5? 1=yes");
        if (in.nextInt() == 1) {
            held.set(4, true);
        }
        for (int c = 0; c < 5; c++) {
            if (!held.get(c)) {
                dice.set(c, d.roll());
            }
        }
        System.out.println("you rolled " + dice.get(0) + " , " + dice.get(1) + " , " +dice.get(2) + " , " + dice.get(3) + " , " + dice.get(4));


        if (time == 1) {
            time++;
            System.out.println("would you like to reroll again?1=yes");
            if (in.nextInt() == 1) {
                reroll();
            }
        }
    }
}
