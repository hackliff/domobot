import java.io.*;
import java.util.Map;
import java.util.Collection;
import java.util.Set;
import com.google.gson.Gson;

public class Reception implements Runnable 
{
	private BufferedReader in;
	private String message = null, login = null;
    public Thread t;
	
	public Reception(BufferedReader in, String login)
    {
		this.in = in;
		this.login = login;
	}
	
	public void run() 
    {	
        String lineTmp;
        boolean listening = true;
		while( listening ){
	        try {        	
                message = in.readLine();
		    } catch (IOException e) {
				e.printStackTrace();
			}
            Map data = new Gson().fromJson(message, Map.class);
            System.out.println("Received: " + data);
            if ( data.containsKey("exec") )
            {
                Collection c = data.values();
                Set s = data.entrySet();
                System.out.println("Size: " + data.size());
                System.out.println("Values: " + c);
                System.out.println("Entry set: " + s);
                if ( data.get("exec").equals("process") ) 
                {
                    System.out.println("Running " + data.get("process") + " in " + data.get("wd"));
                    t = new Thread(new ProcessThread((String)data.get("process"), (String)data.get("wd")));
                    t.start();
                }
                else if ( data.get("exec").equals("intern") ){
                    if (data.get("process").equals("end"))
                        listening = false;
                }
		    }
	    }
        System.out.println(login + " ended conversation.");
    }
}
