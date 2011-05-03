/**
 * 
 */
package eu.etaxonomy.dataportal.selenium;

import java.io.IOException;

import org.apache.log4j.Logger;
import org.junit.AfterClass;
import org.junit.BeforeClass; //$Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy 
 * http://www.e-taxonomy.eu
 * 
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
import org.junit.runner.RunWith;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.firefox.FirefoxDriver;
import org.openqa.selenium.firefox.FirefoxProfile;
import org.openqa.selenium.ie.InternetExplorerDriver;

import eu.etaxonomy.dataportal.Browser;
import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DataPortalContextAwareRunner;
import eu.etaxonomy.dataportal.DataPortalManager;
import eu.etaxonomy.dataportal.SystemUtils;

/**
 * @author a.kohlbecker
 * 
 */
@RunWith(DataPortalContextAwareRunner.class)
public abstract class CdmDataPortalTestBase {

	public static final Logger logger = Logger.getLogger(CdmDataPortalTestBase.class);

	private static final String SYSTEM_PROPERTY_NAME_BROWSER = "browser";

	private static final String FIREBUG_VERSION = "1.6.2";

	protected static WebDriver driver;

	@BeforeClass
	public static void setUpDriver() {
		try {
			Browser browser = Browser.valueOf(System.getProperty(SYSTEM_PROPERTY_NAME_BROWSER, Browser.firefox.name()));

			logger.info("Using browser: " + browser.name());
			switch (browser) {
			case firefox:
				driver = initFirefoxDriver();
				break;
			case chrome:
				driver = initChromeDriver();
				break;
			case iexplorer:
				driver = initInternetExplorerDriver();
				break;
			}

		} catch (NullPointerException e) {
			SystemUtils.reportInvalidSystemProperty(SYSTEM_PROPERTY_NAME_BROWSER, e);
		} catch (IllegalArgumentException e) {
			SystemUtils.reportInvalidSystemProperty(SYSTEM_PROPERTY_NAME_BROWSER, e);
		}

	}

	@AfterClass
	public static void closeDriver() {
		if (driver != null) {
			driver.quit();
		}
	}

	public static WebDriver initChromeDriver() {
		// System.setProperty("webdriver.chrome.bin",
		// "C:\\Dokumente und Einstellungen\\a.kohlbecker.BGBM\\Lokale Einstellungen\\Anwendungsdaten\\Google\\Chrome\\Application\\chrome.exe");
		return new ChromeDriver();
	}

	public static WebDriver initInternetExplorerDriver() {
		return new InternetExplorerDriver();
	}

	/**
	 * -Dwebdriver.firefox.bin=/usr/lib/iceweasel/firefox-bin
	 * 
	 * See http://code.google.com/p/selenium/wiki/FirefoxDriverInternals
	 * 
	 * @return
	 */
	public static WebDriver initFirefoxDriver() {
		// System.setProperty("webdriver.firefox.bin",
		// "C:\\Programme\\Mozilla Firefox 3\\firefox.exe");
		// System.out.println("##:" +
		// System.getProperty("webdriver.firefox.bin"));
		
		FirefoxProfile firefoxProfile = new FirefoxProfile();
		try {

			firefoxProfile.addExtension(CdmDataPortalTestBase.class, "/org/mozilla/addons/firebug-" + FIREBUG_VERSION + ".xpi");
			firefoxProfile.setPreference("extensions.firebug.currentVersion", FIREBUG_VERSION); // avoid displaying firt run page

			// --- allow enabling incompatible addons
			// firefoxProfile.addExtension(this.getClass(),
			// "/org/mozilla/addons/add_on_compatibility_reporter-0.8.3-fx+tb+sm.xpi");
			// firefoxProfile.setPreference("extensions.acr.firstrun", false);
			// firefoxProfile.setPreference("extensions.enabledAddons",
			// "fxdriver@googlecode.com,compatibility@addons.mozilla.org:0.8.3,fxdriver@googlecode.com:0.9.7376,{CAFEEFAC-0016-0000-0024-ABCDEFFEDCBA}:6.0.24,{20a82645-c095-46ed-80e3-08825760534b}:0.0.0,meetinglauncher@iconf.net:4.10.12.316,jqs@sun.com:1.0,{972ce4c6-7e08-4474-a285-3208198ce6fd}:4.0");
			// firefoxProfile.setPreference("extensions.checkCompatibility",
			// false);
			// firefoxProfile.setPreference("extensions.checkCompatibility.4.0",
			// false);
			// firefoxProfile.setPreference("extensions.checkCompatibility.4.1",
			// false);

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.exit(-1);
		}
		driver = new FirefoxDriver(firefoxProfile);

		return driver;
	}

	/**
	 * Return the {@link DataPortalContext#getBaseUri()} of the currently active
	 * context as String
	 * 
	 * @return string representatoin of the DataPortal base URI
	 */
	public String getBaseUrl() {
		return DataPortalManager.currentDataPortalContext().getBaseUri().toString();
	}

}
