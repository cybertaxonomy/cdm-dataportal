/**
 *
 */
package eu.etaxonomy.dataportal.selenium;

import java.io.IOException;

import org.apache.log4j.Logger;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.firefox.FirefoxDriver;
import org.openqa.selenium.firefox.FirefoxProfile;
import org.openqa.selenium.ie.InternetExplorerDriver;

import eu.etaxonomy.dataportal.Browser;
import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.SystemUtils;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;

/**
 * @author andreas
 *
 */
public class WebDriverFactory {

	public static final Logger logger = Logger.getLogger(WebDriverFactory.class);

	public static final String SYSTEM_PROPERTY_NAME_BROWSER = "browser";

	private static final String FIREBUG_VERSION = "1.6.2";
	private static final String FIREXPATH_VERSION = "0.9.6.1";

	public static WebDriver newWebDriver() {

		WebDriver newDriver = null;
		try {
			Browser browser = Browser.valueOf(System.getProperty(SYSTEM_PROPERTY_NAME_BROWSER, Browser.firefox.name()));

			logger.info("Using browser: " + browser.name());
			switch (browser) {
			case firefox:
				newDriver = initFirefoxDriver();
				break;
			case chrome:
				newDriver = initChromeDriver();
				break;
			case iexplorer:
				newDriver = initInternetExplorerDriver();
				break;
			}
		} catch (NullPointerException e) {
			SystemUtils.handleInvalidSystemProperty(SYSTEM_PROPERTY_NAME_BROWSER, e);
		} catch (IllegalArgumentException e) {
			SystemUtils.handleInvalidSystemProperty(SYSTEM_PROPERTY_NAME_BROWSER, e);
		}
		return newDriver;
	}


	private static WebDriver initChromeDriver() {
		return new ChromeDriver();
	}

	private static WebDriver initInternetExplorerDriver() {
		return new InternetExplorerDriver();
	}

	/**
	 * -Dwebdriver.firefox.bin=/usr/lib/iceweasel/firefox-bin
	 *
	 * See http://code.google.com/p/selenium/wiki/FirefoxDriverInternals
	 *
	 * @return
	 */
	private static WebDriver initFirefoxDriver() {

		WebDriver driver;

		CdmDataPortalTestBase.logger.info(("firefox binary:" + System.getProperty("webdriver.firefox.bin")));

		FirefoxProfile firefoxProfile = new FirefoxProfile();
		try {

			CdmDataPortalTestBase.logger.debug("FirefoxProfile: " + firefoxProfile.getClass().getSimpleName() + "(" + firefoxProfile.hashCode() + ")");

			firefoxProfile.addExtension(CdmDataPortalTestBase.class, "/org/mozilla/addons/firebug-" + FIREBUG_VERSION + ".xpi");

			firefoxProfile.setPreference("extensions.firebug.currentVersion", FIREBUG_VERSION); // avoid displaying first run page

			firefoxProfile.addExtension(CdmDataPortalTestBase.class, "/org/mozilla/addons/firexpath-" + FIREXPATH_VERSION + "-fx.xpi");

			firefoxProfile.setEnableNativeEvents(true);

			// --- allow enabling incompatible addons
			// firefoxProfile.addExtension(this.getClass(), "/org/mozilla/addons/add_on_compatibility_reporter-0.8.3-fx+tb+sm.xpi");
			// firefoxProfile.setPreference("extensions.acr.firstrun", false);
			// firefoxProfile.setPreference("extensions.enabledAddons", "fxdriver@googlecode.com,compatibility@addons.mozilla.org:0.8.3,fxdriver@googlecode.com:0.9.7376,{CAFEEFAC-0016-0000-0024-ABCDEFFEDCBA}:6.0.24,{20a82645-c095-46ed-80e3-08825760534b}:0.0.0,meetinglauncher@iconf.net:4.10.12.316,jqs@sun.com:1.0,{972ce4c6-7e08-4474-a285-3208198ce6fd}:4.0");
			// firefoxProfile.setPreference("extensions.checkCompatibility", false);
			// firefoxProfile.setPreference("extensions.checkCompatibility.4.0", false);
			// firefoxProfile.setPreference("extensions.checkCompatibility.4.1", false);

		} catch (IOException e) {
			CdmDataPortalTestBase.logger.error(e);
			System.exit(-1);
		}
		driver = new FirefoxDriver(firefoxProfile);


		return driver;
	}




}
