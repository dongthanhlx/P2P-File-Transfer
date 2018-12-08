package Client1;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class mainClient {
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		final String serverHost = "127.0.0.1";
		int port ;
		Socket socketOfClient = null;
		BufferedWriter os = null;
		BufferedReader is = null;
		
		Scanner scanner = new Scanner(System.in);
//		System.out.println("Enter IP address: ");
//		ipAddr = scanner.nextLine();
		
		System.out.println("Enter Port: ");
		port = scanner.nextInt();
		scanner.nextLine();
		// init
//		Writer os = null;
		try {
			socketOfClient = new Socket(serverHost, port);
			os = new BufferedWriter(new OutputStreamWriter(socketOfClient.getOutputStream(), "UTF-8"));
			is = new BufferedReader(new InputStreamReader(socketOfClient.getInputStream()));
		}	catch(UnknownHostException e) {
			System.out.println(e);
		}	catch(IOException e) {
			System.out.println("Counldn't get I/O for the conenect to "+serverHost);
			return ;
		}
		
		FileList fileList = new FileList();
		fileList.setFileList();
		String list = fileList.getFileList().concat("\0");
//		int numFile = fileList.getNumFile();
		
		try  {
			System.out.println("Length: "+list.length());
			os.write(list);
//			os.newLine();
			os.flush();
		}	catch(UnknownHostException e) {
			e.printStackTrace();
		}	catch(IOException e) {
			e.printStackTrace();
		}
		System.out.println("End");
	}

}
