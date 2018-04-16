
public class HangmanProcessor {

    private String wordToGuess = "";
    private String displayWord = "";
    private int numWrong;

    public HangmanProcessor(String word) {//setup game
        wordToGuess = word;
        numWrong = 0;
        for (int i = 0; i < word.length(); i++) {
            if (word.substring(i, i + 1).compareTo(" ") == 0) {
                
                displayWord += " ";
            } else {
                
                displayWord += "_";
            }
        }
    }

    String getWordToGuess() {
        return wordToGuess;
    }

    String getDisplayWord() {
        return displayWord;
    }
    int getNumWrong(){
        return numWrong;
    }

    public boolean isInWord(String target) {
        for (int i = 0; i < wordToGuess.length(); i++) {
            if (wordToGuess.substring(i, i + 1).compareToIgnoreCase(target) == 0) {
                return true;
            }
        }
        return false;
    }

    public boolean solvedWord() {
        if (wordToGuess.compareToIgnoreCase(displayWord) == 0) {
            displayWord += "~~~~YOU WIN";
            return true;
        } else {
            return false;
        }
    }

    public boolean lostGame() {
        if (numWrong >= 5) {
            return true;
        } else {
            return false;
        }
    }

    public void setWordToGuess(String word) {
        wordToGuess = word;
        displayWord = "";
        numWrong = 0;
        for (int i = 0; i < word.length(); i++) {
            if (word.substring(i, i + 1).compareTo(" ") == 0) {
                displayWord += " ";
            } else {
                displayWord += "-";
            }
        }
    }

    public void replaceLetters(String letter) {
        int numReplaced=0;
        String temp = new String();
        for (int i = 0; i < wordToGuess.length(); i++) {
            if (wordToGuess.substring(i, i + 1).compareToIgnoreCase(letter) == 0) {
                temp += letter;
                numReplaced++;
            } else {
                temp += displayWord.substring(i, i + 1);
            }
        }
        displayWord = temp;
        if(displayWord.compareToIgnoreCase(wordToGuess)==0){
            displayWord += "~~~~YOU WIN";
        }
        if(numReplaced==0){
            numWrong++;
        }
    }
}
