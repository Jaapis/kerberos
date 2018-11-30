package br.edu.unifei.eco;

import com.sun.security.auth.callback.TextCallbackHandler;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.PrivilegedAction;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.security.auth.*;
import javax.security.auth.login.*;
import javax.security.auth.callback.*;
import javax.security.auth.kerberos.*;;

/**
 * This class is an implementation of a simple server interface 
 * 
 * @author Guilherme M. Bortoletto <guilherme.mbortoletto@gmail.com>
 * @version 0.1
 */
public class Server {
    
    private static final int kPort     = 8080;
    private ServerSocket server        = null;
    private Socket client              = null;
    private String password            = "";
    private DataInputStream inStream   = null;
    private DataOutputStream outStream = null;
    
    // Kerberos variables
    LoginContext lc = null;
    
    /**
     * This method initializes everything that has to do with kerberos
     */
    public void initKerberos() {
        try {
            lc = new LoginContext("test server", new TextCallbackHandler());
            lc.login();
        } catch (LoginException ex) {
            Logger.getLogger(Server.class.getName())
                  .log(Level.SEVERE, null, ex);
        }
        Subject mySubject = lc.getSubject();
        PrivilegedAction action = /* new ServerAction(); */ null;
    }
    
    public Server() {
        init();
    }
    
    /**
     * This method initializes every variable to it's default value
     */
    public void init() {
        try {
            System.out.println("[ MESSAGE ] Iniciando servidor...");
            server = new ServerSocket(kPort);
            System.out.println("[ SUCCESS ] Servidor iniciado com sucesso...");
            System.out.println("[ MESSAGE ] Aguardando cliente...");
            client = server.accept();
            System.out.println("[ SUCCESS ] Conexão estabelecida com o cliente:"
                             + " \"" + client.getInetAddress() + "\"...");
            password = new String("unifei");
        } catch (IOException ex) {
            Logger.getLogger(Server.class.getName())
                  .log(Level.SEVERE, null, ex);
        }
        initKerberos();
    }
    
    /**
     * Gets input from client and authenticates password
     */
    public void authenticate() {
        boolean success  = false;
        String messageIn = null;
        // Initializes data streams
        try {
            inStream  = new DataInputStream(client.getInputStream());
            outStream = new DataOutputStream(client.getOutputStream());
        } catch (IOException ex) {
            Logger.getLogger(Server.class.getName())
                  .log(Level.SEVERE, null, ex);
        }
        
        while (!success) {
            
            // Reads input stream from client
            try {
                messageIn = inStream.readUTF();
            } catch (IOException ex) {
                Logger.getLogger(Server.class.getName())
                      .log(Level.SEVERE, null, ex);
            }
            
            // Checks password handshake
            try {
                if (messageIn != null && password.equals(messageIn)) {
                    outStream.writeUTF("SUCCESS");
                    System.out.println("[ SUCCESS ] Conectado com sucesso...");
                    success = true;
                } else {
                    outStream.writeUTF("FAIL");
                }
                outStream.flush();
            } catch (IOException ex) {
                Logger.getLogger(Server.class.getName())
                      .log(Level.SEVERE, null, ex);
            }
        }
    }
    
    /**
     * Terminates the server process freeing every resource
     */
    public void terminate() {
        try {
            outStream.close();
            inStream.close();
            client.close();
            server.close();
        } catch (IOException ex) {
            Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    /**
     * Main function for client
     * @param args arguments from terminal
     */
    public static void main(String[] args) {
        Server server = new Server();
        server.authenticate();
        server.terminate();
    }
    
}
