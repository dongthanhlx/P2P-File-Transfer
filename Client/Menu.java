package Client3;

import java.util.Scanner;

public class Menu {
	private static int input;
	public void show() {
		System.out.println("1. Check list file");
		System.out.println("2. Download");
		System.out.println("3. Disconnect");
	}
	
	public void select() {
		@SuppressWarnings("resource")
		Scanner scanner = new Scanner(System.in);
		do {
			System.out.print("Enter select: ");
			input = Integer.parseInt(scanner.nextLine());
		}while(input<1 || input > 3);
	}
	public String getSelectTypeString() {
		String result = null;
		switch(input) {
		case 1: result = "request-file-list\0";
			break;
		case 2: result = "download\0";
			break;
		case 3: result = "disconnect\0";
			break;
		}
		return result;
	}
	public int getSelectTypeNum() {
		return input;
	}
}