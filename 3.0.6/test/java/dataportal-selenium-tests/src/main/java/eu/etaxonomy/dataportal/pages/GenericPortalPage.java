// $Id$
/**
* Copyright (C) 2011 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;

import org.openqa.selenium.WebDriver;

import eu.etaxonomy.dataportal.DataPortalContext;

/**
 * @author andreas
 * @date Jul 5, 2011
 *
 */
public class GenericPortalPage extends PortalPage {


	/**
	 * @param driver
	 * @param context
	 * @param pagePathSuffix
	 * @throws MalformedURLException
	 */
	public GenericPortalPage(WebDriver driver, DataPortalContext context, String pagePathSuffix) throws MalformedURLException {
		super(driver, context, pagePathSuffix);
	}

	/**
	 * @param driver
	 * @param context
	 * @throws Exception
	 */
	public GenericPortalPage(WebDriver driver, DataPortalContext context) throws Exception {
		super(driver, context);
	}

	protected static String drupalPagePathBase = "cdm_dataportal";

	/* (non-Javadoc)
	 * @see eu.etaxonomy.dataportal.pages.PortalPage#getDrupalPageBase()
	 */
	@Override
	protected String getDrupalPageBase() {
		return drupalPagePathBase;
	}

}
