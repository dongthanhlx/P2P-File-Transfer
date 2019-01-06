package Client3;

import java.io.File;

public class FileList {
	private static int numFile;
	String list = new String("");
	public void setFileList() {
		String workingDir = System.getProperty("user.dir");
		File fileList = new File(workingDir+"/src/Client3");
		File[] file = fileList.listFiles();
		numFile = file.length;
		for (File index : file) {
			list = list.concat(index.getName().concat(" "));
		}
	}
	
	public String getFileList() {
		return list;
	}
	
	public int getNumFile() {
		return numFile;
	}
}