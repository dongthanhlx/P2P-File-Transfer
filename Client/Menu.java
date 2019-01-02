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
		Scanner scanner = new Scanner(System.in);
		do {
			System.out.print("Enter select: ");
//			input = scanner.nextInt();
			input = Integer.parseInt(scanner.nextLine());
		}while(input<0 && input > 3);
//		return input;
		scanner.close();
	}
	public String getSelectTypeString() {
		String result = null;
		switch(input) {
		case 1: result = "mot\0";
			break;
		case 2: result = "hai\0";
			break;
		case 3: result = "ba\0";
			break;
		}
		return result;
	}
	public int getSelectTypeNum() {
		return input;
	}
}
