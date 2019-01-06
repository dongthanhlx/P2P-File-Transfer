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

	private static String serverHost ;
	private static int port ;
	private static int bufferSize = 1024;
	private static Socket socketOfClient = null;
	private static BufferedWriter os = null;
	private static InputStreamReader in = null;
	private static FileList fileList;
	public static Scanner scanner = new Scanner(System.in);
	private static String list = "";
	private static String listIP = "";
	private static String fileName = null;
	public static void InitSocket() {
		System.out.print("Enter IP address: ");
		serverHost = scanner.nextLine();
		System.out.print("Enter Port: ");
		port = scanner.nextInt();
		scanner.nextLine();
		try {
			socketOfClient = new Socket(serverHost, port);
//			System.out.println("Show socket: "+socketOfClient.getLocalPort());
			os = new BufferedWriter(new OutputStreamWriter(socketOfClient.getOutputStream(), "UTF-8"));
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
			rMsg = new String(msg, 0, br.read(msg, 0, bufferSize));
		}	catch(IOException e) {
			e.printStackTrace();
		}
		return rMsg;
	}
	
	public static void setListIPToClient(String _listIP) {
		listIP = _listIP;
	}
	public static String getListIPToClient() {
		return listIP;
	}
	public static boolean FMenu() {
		@SuppressWarnings("resource")
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
				String notFile = new String("QUIT");
				os.write(menu.getSelectTypeString());
				os.flush();
				System.out.print("Enter file name: ");
				fileName = scanner.nextLine();
				fileName.concat("\0");
				os.write(fileName);
				os.flush();
				String rMsg = ReceicedMsg(listIP);
				if(notFile.equals(rMsg)) {
					System.out.println("The server has no file !!!");
					return true;
				}
				setListIPToClient(rMsg);
				System.out.println(getListIPToClient());
				break;
			}
			case 3: {		// disconnect
				os.write(menu.getSelectTypeString());
				os.flush();
				System.out.println("Disconnected!!!");
				socketOfClient.close();
				return false;
			}
			}
		}	catch(IOException e) {
			e.printStackTrace();
		}
		return true;
	}
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		InitSocket();
		InitFileList();
		boolean run = true;
		while(run) {
			SendMsg(GetFileList());
			run = FMenu();
		}
	}
}