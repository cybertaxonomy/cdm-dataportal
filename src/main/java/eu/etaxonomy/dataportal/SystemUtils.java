/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal;

public class SystemUtils {

	public static void handleInvalidSystemProperty(String propertyName, Exception e) {
		throw new RuntimeException("Invalid system property  -D" +
				propertyName +  "=" + System.getProperty(propertyName), e);
	}

}
