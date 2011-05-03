// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy 
 * http://www.e-taxonomy.eu
 * 
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium;

import org.junit.BeforeClass;
import org.openqa.selenium.WebDriverBackedSelenium;

import com.thoughtworks.selenium.Selenium;

import eu.etaxonomy.dataportal.DataPortalManager;

/**
 * http://seleniumhq.org/docs/03_webdriver.html#emulating-selenium-rc
 * 
 * @author a.kohlbecker
 * 
 */
public abstract class CdmDataPortalSeleniumRCTestBase extends
		CdmDataPortalTestBase {

	protected static Selenium selenium;

	@BeforeClass
	public static void setUpDriver() {
		CdmDataPortalTestBase.setUpDriver();
		selenium = new WebDriverBackedSelenium(driver, DataPortalManager
				.currentDataPortalContext().getBaseUri().toString());
	}

	@BeforeClass
	public static void closeDriver() {
		if (selenium != null) {
			selenium.stop();
		}
		CdmDataPortalTestBase.closeDriver();
	}

}
