import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.OutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

class BetterServer {
    private static ServerSocket serverSocket;
    private static Socket clientSocket;
    private static BufferedReader in;
    private static PrintWriter out;
    private static String output = "";
    private static String eor = "[EOR]"; // a code for end-of-response
    
    // establishing a connection
    private static void setup() throws IOException {
        
        serverSocket = new ServerSocket(0);
        toConsole("Server port is " + serverSocket.getLocalPort());
        
        clientSocket = serverSocket.accept();

        // get the input stream and attach to a buffered reader
        in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        
        // get the output stream and attach to a printwriter
        out = new PrintWriter(clientSocket.getOutputStream(), true);

        toConsole("Accepted connection from "
                 + clientSocket.getInetAddress() + " at port "
                 + clientSocket.getPort());
            
        sendGreeting();
    }
    
    // the initial message sent from server to client
    private static void sendGreeting()
    {
        appendOutput("Welcome to BetterServer!\n");
        appendOutput("Submit text and I will convert it to upper case:");
        sendOutput();
    }
    
    // what happens while client and server are connected
    private static void talk() throws IOException {
        /* placing echo functionality into a separate private method allows it to be easily swapped for a different behaviour */
        clientProcessor();
        disconnect();
    }
    
    // repeatedly take input from client and send back in upper case
    private static void echoClient() throws IOException
    {
        String clientInput;
        while ((clientInput = in.readLine()) != null) {
            appendOutput(clientInput.toUpperCase());
            sendOutput();
            toConsole(clientInput);
        }
    }
    
    private static void disconnect() throws IOException {
        out.close();
        toConsole("Disconnected.");
        System.exit(0);
    }
    
    // add a line to the next message to be sent to the client
    private static void appendOutput(String line) {
        output += line + "\r";
    }
    
    // send next message to client
    private static void sendOutput() {
        out.println( output + "[EOR]");
        out.flush();
        output = "";
    }
    
    // because it makes life easier!
    private static void toConsole(String message) {
        System.out.println(message);
    }
    
    public static void main(String[] args) {
        try {
            setup();
            talk();
        }
        catch( IOException ioex ) {
            toConsole("Error: " + ioex );
        }
    }
    // checks for username and password, then allows the user to guess the word!
    private static void clientProcessor() throws IOException{
        String clientInput;
        int numberOfLoginAttempts = 0;
        int numberOfGuessAttempts = 1; 
        char word [] = {'T','O','P','P','L','E'};

        while ((clientInput = in.readLine()) != null) {
            if (clientInput.equals("Sammy")){
                appendOutput("Enter password: ");
                sendOutput();
                toConsole("Password Accepted");
                break;
            }
            else{
                numberOfLoginAttempts++;
                if (numberOfLoginAttempts == 5) {disconnect();}
                appendOutput("Wrong Username\n");
                appendOutput("Enter The Username: ");
                sendOutput();
                toConsole("Wrong Username Given");
                continue;
            }
        }
        while ((clientInput = in.readLine()) != null) {
            if (clientInput.equals("WOOF")){
                appendOutput("Welcome Sammy!\n");
                appendOutput("Guess the mystery six-letter word\n");
                appendOutput("Enter a word without any space: ");
                sendOutput();
                toConsole("Correct Password Given");
                break;
            }
            else{
                numberOfLoginAttempts++;
                if (numberOfLoginAttempts == 5) {disconnect();}
                appendOutput("Wrong Password\n");
                appendOutput("Enter password: ");
                sendOutput();
                toConsole("Wrong Password Given");
                continue;
            }
        }
        while ((clientInput = in.readLine()) != null) {
            char returnArray [] = new char[6];
            char[] guessArray = clientInput.toCharArray();
            if ((clientInput.toCharArray().length != 6) || (!clientInput.matches("[a-zA-Z]+"))){
                numberOfGuessAttempts++;
                appendOutput("Oops! You need enter a six letter word containing only alphabetical characters and no spaces.\n");
                appendOutput("Try Again: ");
                sendOutput();
                toConsole("Wrong format was entered");
                continue;
            }
            for (int i=0; i<6; i++){
                if (Character.toUpperCase(guessArray[i]) == word[i]){
                    returnArray[i] = word[i];
                }
                else{
                    returnArray[i] = '*';
                }
            }
            if (String.valueOf(returnArray).equals(String.valueOf(word))){
                appendOutput("You got it in " + numberOfGuessAttempts + " - well done and goodbye!");
                toConsole("Correct answer guessed!");
                sendOutput();
                disconnect();
            }
            else{
                numberOfGuessAttempts++;
                appendOutput(String.valueOf(returnArray)+ "\n");
                appendOutput("Try again: ");
                sendOutput();
                toConsole("Wrong answer guessed!");
                continue;
            }
        }
    }
}
