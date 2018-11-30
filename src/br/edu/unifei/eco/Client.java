package br.edu.unifei.eco;

import com.sun.security.auth.callback.TextCallbackHandler;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.security.PrivilegedAction;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.security.auth.*;
import javax.security.auth.login.*;
import javax.security.auth.callback.*;
import javax.security.auth.kerberos.*;


/**
 * This class is an implementation of a simple client interface
 * 
 * @author Guilherme M. Bortoletto <guilherme.mbortoletto@gmail.com>
 * @version 0.1
 */
public class Client {
    
    private Scanner scan                = null;
    private InetAddress serverIpAddress = null;
    private Socket client               = null;
    private DataInputStream inStream    = null;
    private DataOutputStream outStream  = null;
    private int serverPort              = 0;
    
    // Kerberos variables
    private Subject subject             = null;
    private LoginContext loginContext   = null;
    
    /**
     * Initializes every variable
     */
    public Client() {
        init();
    }
    
    /**
     * This method initializes everything that has to do with kerberos
     */
    public void initKerberos() {
        subject = new Subject();
        try {
            loginContext = new LoginContext("test server"
                         , new TextCallbackHandler());
            loginContext.login();
        } catch (LoginException ex) {
            Logger.getLogger(Client.class.getName()).log(Level.SEVERE, null, ex);
        }
        subject = loginContext.getSubject();
        PrivilegedAction action = null; // in here should be a new privileged
                                        // action only principals can do
        Subject.doAs(subject, action);
        
    }
    
    /**
     * Initializes every variable
     */
    public void init() {
        
        // Get input from keyboard
        scan = new Scanner(System.in);
        
        // Get ip from user input and initializes the address
        try {
            System.out.print("[ PROMPT ] Insira o endereço ip do servidor: ");
            String ipPromptAnswer = scan.nextLine();
            // TOFIX: Check this code, right now it's useless
            serverIpAddress = InetAddress.getByName(ipPromptAnswer);
        } catch (UnknownHostException ex) {
            Logger.getLogger(Client.class.getName())
                  .log(Level.SEVERE, null, ex);
        }
        
        // Gets server port from user input and initializes it
        System.out.print("[ PROMPT ] Insira a porta do servidor: ");
        serverPort = scan.nextInt();
        
        // Create client socket
        try {
            client = new Socket(serverIpAddress, serverPort);
        } catch (IOException ex) {
            Logger.getLogger(Client.class.getName())
                  .log(Level.SEVERE, null, ex);
        }
        
        // Initialize data streams
        try {
            inStream = new DataInputStream(client.getInputStream());
            outStream = new DataOutputStream(client.getOutputStream());
        } catch (IOException ex) {
            Logger.getLogger(Client.class.getName())
                  .log(Level.SEVERE, null, ex);
        }
        initKerberos();
    }
    
    /**
     * Check for password handshake between client and server
     */
    public void authenticate() {
        
        // Gets password from user input and tries to match the server's one
        try {
            System.out.print("[ PROMPT ] Digite a senha: ");
            outStream.writeUTF(scan.nextLine());
            while (inStream.readUTF().equals("FAIL")) {
                System.out.print("[ PROMPT ] Senha errada, tente novamente: ");
                outStream.writeUTF(scan.nextLine());
            }
            System.out.println("[ SUCCESS ] Login realizado com sucesso...");
        } catch (IOException ex) {
            Logger.getLogger(Client.class.getName())
                  .log(Level.SEVERE, null, ex);
        }
    }
    
    /**
     * Terminates the client process freeing every resource
     */
    public void terminate() {
        try {
            outStream.close();
            inStream.close();
            client.close();
        } catch (IOException ex) {
            Logger.getLogger(Client.class.getName()).log(Level.SEVERE, null, ex);
        }
        scan.close();
    }
    
    /**
     * Main function for client
     * @param args arguments from terminal
     */
    public static void main(String[] args) {
        Client client = new Client();
        client.authenticate();
        client.terminate();
    }
    
}
