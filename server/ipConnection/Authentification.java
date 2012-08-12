import java.net.*;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.io.*;

public class Authentification implements Runnable 
{
	private Socket socket;
	private PrintWriter out = null;
	private BufferedReader in = null;
	private String identity =  null;
    private static String identityFile = "identity.txt";
	public boolean authentifier = false;
	public Thread t2;
	
	public Authentification(Socket s)
    {
		 socket = s;
	}
	
    public void run() 
    {
		try {	
			in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			out = new PrintWriter(socket.getOutputStream());
			
		    while(!authentifier){		
                out.println("[SERVER] Provide your identity code");
                out.flush();
                identity = in.readLine();

                if(isValid(identity)){
                    out.println("ok");
                    System.out.println("[SERVER] " + identity + " is connected ");
                    out.flush();
                    authentifier = true;	
                }
                else {out.println("[SERVER] ** identity error"); out.flush();}
             }
                t2 = new Thread(new DistributedExchange(socket,identity));
                t2.start();
                
		} catch (IOException e) {
			System.err.println("[SERVER] ** " + identity + " not responding");
		}
	}
	
	private static boolean isValid(String identity) 
    {		
		boolean connection = false;
		try {
			Scanner sc = new Scanner(new File(identityFile));		
			while(sc.hasNext()){
				if(sc.nextLine().equals(identity)){
              	  connection=true;
				  break;
				}
             }
		} catch (FileNotFoundException e) {	
			System.err.println("[SERVER] ** Authentification file not found");
		}
	return connection;
	}
}

