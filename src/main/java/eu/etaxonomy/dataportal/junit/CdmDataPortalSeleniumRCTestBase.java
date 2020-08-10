/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.junit;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;

import com.thoughtworks.selenium.Selenium;
import com.thoughtworks.selenium.webdriven.WebDriverBackedSelenium;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.DataPortalManager;

/**
 * http://seleniumhq.org/docs/03_webdriver.html#emulating-selenium-rc
 *
 * @author a.kohlbecker
 * @deprecated SeleniumRCTests should only be used if you are really desperatly
 *             short in time and you need to use the Selenium 1 IDE in order to
 *             quickly create tests.
 */
@Deprecated
public abstract class CdmDataPortalSeleniumRCTestBase extends CdmDataPortalTestBase {

    protected static Selenium selenium;

    @Before
    public void setUpSelenium() {
        if(selenium == null) {
            selenium = new WebDriverBackedSelenium(driver, getContext().getSiteUri().toString());
        }
    }

    @AfterClass
    public static void stopSelenium() {
        if (selenium != null) {
            selenium.stop();
            selenium = null;
        }
    }

}
