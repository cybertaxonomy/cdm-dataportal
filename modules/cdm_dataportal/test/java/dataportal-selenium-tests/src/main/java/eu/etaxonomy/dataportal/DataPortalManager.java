// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy 
 * http://www.e-taxonomy.eu
 * 
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal;


/**
 * 
 * @author a.kohlbecker
 * 
 */
public class DataPortalManager {

	static DataPortalManager managerInstance = null;

	
	private DataPortalManager(){
		
	}

	public static void prepare() {
		if (managerInstance == null) {
			managerInstance = new DataPortalManager();
			managerInstance.setupDataPortal();
		}
	}

	private void setupDataPortal() {
		// TODO configure the data portal using drush, see http://dev.e-taxonomy.eu/svn/trunk/drupal/modules/cdm_dataportal/jenkins-ci/integration.sh
	}

}
