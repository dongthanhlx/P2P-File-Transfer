package Client3;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

import Client3.FileList;

public class MainClient {

	private final static String serverHost = "127.0.0.1";
	private static int port ;
	private static int bufferSize = 1024;
	private static Socket socketOfClient = null;
	private static BufferedWriter os = null;
//	private static BufferedReader is = null;
	private static InputStreamReader in = null;
	private static FileList fileList;
	public static Scanner scanner = new Scanner(System.in);
	private static String list = "";
	private static String fileName = "";
	public static void InitSocket() {
		System.out.println("Enter Port: ");
		port = scanner.nextInt();
		scanner.nextLine();
		try {
			socketOfClient = new Socket(serverHost, port);
			os = new BufferedWriter(new OutputStreamWriter(socketOfClient.getOutputStream(), "UTF-8"));
//			is = new BufferedReader(new InputStreamReader(socketOfClient.getInputStream()));
			in = new InputStreamReader(socketOfClient.getInputStream());
		}	catch(UnknownHostException e) {
			System.out.println(e);
		}	catch(IOException e) {
			System.out.println("Counldn't get I/O for the conenect to "+serverHost);
			return ;
		}
	}
	
	public static void InitFileList() {
		fileList = new FileList();
		fileList.setFileList();
		list = fileList.getFileList().concat("\0");
	}
	
	public static String GetFileList() {
		return list;
	}
	public static void SendMsg(String msg) {
		try  {
			os.write(msg);
			os.flush();
		}	catch(UnknownHostException e) {
			e.printStackTrace();
		}	catch(IOException e) {
			e.printStackTrace();
		}
	}
	
	public static String ReceicedMsg(String rMsg) {
		BufferedReader br = new BufferedReader(in);
		char[] msg = new char[bufferSize];
		try {
			rMsg = new String(msg, 0, br.read(msg, 0, 1024));
		}	catch(IOException e) {
			e.printStackTrace();
		}
		return rMsg;
	}
	public static void FMenu() {
		Scanner scanner = new Scanner(System.in);
		Menu menu = new Menu();
		menu.show();
		menu.select();
		try {
			int temp = menu.getSelectTypeNum();
			switch (temp) {
			case 1: {		// show list file
				os.write(menu.getSelectTypeString());
				os.flush();
				list = ReceicedMsg(list);
				System.out.println("Received: "+list);
				break;
			}
			case 2: {		// download
				os.write(menu.getSelectTypeString());
				os.flush();
				System.out.print("Enter file name: ");
				fileName = scanner.nextLine();
				fileName.concat("\0");
				os.write(fileName);
				os.flush();
				scanner.close();
				break;
			}
			case 3: {		// disconnect
				
			}
			}
		}	catch(IOException e) {
			e.printStackTrace();
		}
	}
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		InitSocket();
		InitFileList();
		SendMsg(GetFileList());
		FMenu();
//		list = ReceicedMsg(list);
//		System.out.println("Received: "+list);
//		while(true) {
//			FMenu();
//		}
	}
}