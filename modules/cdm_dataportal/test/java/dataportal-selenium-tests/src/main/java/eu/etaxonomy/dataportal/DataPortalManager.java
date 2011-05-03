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

	private static final String SYSTEM_PROPERTY_NAME_DATA_PORTAL_CONTEXT = "dataPortalContext";

	static DataPortalManager managerInstance = null;

	private DataPortalContext currentDataPortalContext = null;
	
	private DataPortalManager(){
		try {
			currentDataPortalContext = DataPortalContext.valueOf(System.getProperty(SYSTEM_PROPERTY_NAME_DATA_PORTAL_CONTEXT));
		} catch (NullPointerException e) {
			SystemUtils.reportInvalidSystemProperty(SYSTEM_PROPERTY_NAME_DATA_PORTAL_CONTEXT, e);
		} catch (IllegalArgumentException e) {
			SystemUtils.reportInvalidSystemProperty(SYSTEM_PROPERTY_NAME_DATA_PORTAL_CONTEXT, e);
		}
	}

	public static void prepare() {
		if (managerInstance == null) {
			managerInstance = new DataPortalManager();
			managerInstance.setupDataPortal();
		}
	}

	public static DataPortalContext currentDataPortalContext() {
		prepare();
		return managerInstance.currentDataPortalContext;
	}

	private void setupDataPortal() {
		// TODO configure the data portal using drush, see http://dev.e-taxonomy.eu/svn/trunk/drupal/modules/cdm_dataportal/jenkins-ci/integration.sh
	}

}
