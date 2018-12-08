package Client1;

import java.io.File;

public class FileList {
	private static int numFile;
//	private static String source = "./src/Client";
	String list = new String("");
	public void setFileList() {
		String workingDir = System.getProperty("user.dir");
		File fileList = new File(workingDir+"/src/Client1");
		File[] file = fileList.listFiles();
		numFile = file.length;
		for (File index : file) {
			list = list.concat(index.getName().concat("/"));
		}
		System.out.println(list);
	}
	
	public String getFileList() {
		return list;
	}
	
	public int getNumFile() {
		return numFile;
	}
}
