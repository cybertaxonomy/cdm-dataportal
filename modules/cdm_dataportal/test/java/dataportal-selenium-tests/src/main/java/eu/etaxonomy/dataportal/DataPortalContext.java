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

import java.net.URI;
import java.net.URISyntaxException;
import java.util.UUID;

import org.apache.log4j.Logger;

/**
 * TODO 1. allow overwriting the hard coded contexts configuration by {user.home}/.cdmLibrary/DataPortalTest.properties
 * TODO 2. let constructor read from a dataPortalTestContext.properties located in the jar (/dataportal-selenium-tests/src/main/resources/eu/etaxonomy/dataportal/DataPortalTest.properties)
 * TODO 3. DataPortalTest.properties should allow setting the defalut eu.etaxonomy.dataportal.selenium.CdmDataPortalTestBase.SYSTEM_PROPERTY_NAME_BROWSER
 * TODO 4. DataPortalTest.properties should allow setting webdriver.firefox.bin etc in order to circumven the need to set it by -Dwebdriver.firefox.bin
 * 
 * @author a.kohlbecker
 *
 */
public enum DataPortalContext {

	cichorieae(""),
	palmae(""),
	cyprus("");

	URI baseUri;
	URI cdmServerUri;
	UUID classificationUUID;
	String themeName;

	public final Logger logger = Logger.getLogger(DataPortalContext.class);

	private DataPortalContext(String dummy) {

		this.baseUri = TestConfiguration.getProperty(composePropertyKey("baseUri"), URI.class);
		this.cdmServerUri = TestConfiguration.getProperty(composePropertyKey("cdmServerUri"), URI.class);
		this.classificationUUID = TestConfiguration.getProperty(composePropertyKey("classificationUUID"), UUID.class);

	}

	private String composePropertyKey(String fieldName) {
		String key = this.getClass().getSimpleName().substring(0, 1).toLowerCase() + this.getClass().getSimpleName().substring(1) + "." + this.name() + "." + fieldName;
		return key;
	}

	public URI getBaseUri() {
		return baseUri;
	}

	public URI getCdmServerUri() {
		return cdmServerUri;
	}

	public UUID getClassificationUUID() {
		return classificationUUID;
	}

}
