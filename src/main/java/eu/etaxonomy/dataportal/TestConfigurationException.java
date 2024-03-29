/**
* Copyright (C) 2011 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal;

/**
 * @author andreas
 * @since Aug 29, 2011
 *
 */
public class TestConfigurationException extends Exception {

	/**
	 * @param message
	 * @param e
	 */
	public TestConfigurationException(String message, Exception e) {
		super(message, e);
	}

	/**
	 * @param message
	 */
	public TestConfigurationException(String message) {
		super(message);
	}

	/**
	 *
	 */
	private static final long serialVersionUID = 1265655528811777837L;

}
