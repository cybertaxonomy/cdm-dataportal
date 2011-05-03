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

	cichorieae("http://160.45.63.201/dataportal/preview/cichorieae/", "http://127.0.0.1:8080", "534e190f-3339-49ba-95d9-fa27d5493e3e"), 
	palmae("http://160.45.63.201/dataportal/preview/cyprus/", "http://127.0.0.1:8080", "534e190f-3339-49ba-95d9-fa27d5493e3e"),
	cyprus("http://160.45.63.201/dataportal/preview/cyprus/", "http://127.0.0.1:8080", "0c2b5d25-7b15-4401-8b51-dd4be0ee5cab");

	URI baseUri;
	URI cdmServerUri;
	UUID classificationUUID;
	String themeName;

	private DataPortalContext(String baseUri, String cdmServerUri, String classificationUUID) {
		try {
			this.baseUri = new URI(baseUri);
			this.cdmServerUri = new URI(cdmServerUri);
		} catch (URISyntaxException e) {
			throw new RuntimeException(e);
		}
		this.classificationUUID = UUID.fromString(classificationUUID);
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
