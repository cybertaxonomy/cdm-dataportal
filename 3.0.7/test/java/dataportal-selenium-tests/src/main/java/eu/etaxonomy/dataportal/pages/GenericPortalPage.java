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
import java.sql.PreparedStatement;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.selenium.AllTrue;
import eu.etaxonomy.dataportal.selenium.PageTitleValidated;
import eu.etaxonomy.dataportal.selenium.UrlLoaded;
import eu.etaxonomy.dataportal.selenium.VisibilityOfElementLocated;

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

	public TaxonSearchResultPage submitQuery(String query) throws Exception{
		searchBlockElement.findElement(By.id("edit-query")).sendKeys(query);
		searchBlockElement.findElement(By.id("edit-submit")).submit();//Search results

		wait.until(new AllTrue(new PageTitleValidated(context.prepareTitle("Search results")), new VisibilityOfElementLocated(By.id("container"))));
		return new TaxonSearchResultPage(driver, context);
	}



}
