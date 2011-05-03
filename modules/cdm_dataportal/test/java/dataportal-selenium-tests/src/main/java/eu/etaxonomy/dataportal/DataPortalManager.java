/**
 * 
 */
package eu.etaxonomy.dataportal;

import static eu.etaxonomy.dataportal.DataPortalContext.*;

/**
 * @author a.kohlbecker
 *
 */
public class DataPortalManager {
	
	static DataPortalManager managerInstance = null;
	
	private DataPortalContext currentDataPortalContext = cichorieae;
	
	public static void prepare() {
		if(managerInstance == null){
			managerInstance = new DataPortalManager();
			managerInstance.setupDataPortal();
		}
	}
	
	public static DataPortalContext currentDataPortalContext(){
		prepare();
		return managerInstance.currentDataPortalContext;
	}

	private void setupDataPortal() {
		//TODO 
	}

}
