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
import java.util.UUID;

import org.apache.log4j.Logger;

/**
 *
 * @author a.kohlbecker
 *
 */
public enum DataPortalContext {

	cichorieae,
	palmae,
	cyprus,
	floramalesiana;

	URI baseUri;
	URI cdmServerUri;
	UUID classificationUUID;
	String siteName; //TODO could be read with drush: $ drush vget site_name
	String themeName;


	public final Logger logger = Logger.getLogger(DataPortalContext.class);

	private DataPortalContext() {
		this.baseUri = TestConfiguration.getProperty(composePropertyKey("baseUri"), URI.class);
		this.cdmServerUri = TestConfiguration.getProperty(composePropertyKey("cdmServerUri"), URI.class);
		this.classificationUUID = TestConfiguration.getProperty(composePropertyKey("classificationUUID"), UUID.class);
		this.siteName = TestConfiguration.getProperty(composePropertyKey("siteName"));
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

	public String getSiteName() {
		return siteName;
	}

}
