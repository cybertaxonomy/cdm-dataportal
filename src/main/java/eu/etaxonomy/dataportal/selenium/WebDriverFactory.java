/**
 *
 */
package eu.etaxonomy.dataportal.selenium;

import java.io.IOException;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.chrome.ChromeOptions;
import org.openqa.selenium.firefox.FirefoxDriver;
import org.openqa.selenium.firefox.FirefoxProfile;
import org.openqa.selenium.ie.InternetExplorerDriver;

import eu.etaxonomy.dataportal.Browser;
import eu.etaxonomy.dataportal.SystemUtils;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;

/**
 * @author andreas
 */
public class WebDriverFactory {

    private static final Logger logger = LogManager.getLogger();

    public static final String SYSTEM_PROPERTY_NAME_BROWSER = "browser";

    private static final String FIREBUG_VERSION = "1.10.5";
    private static final String FIREXPATH_VERSION = "0.9.7";
    private static final String DISABLE_ADD_ON_COMPATIBILITY_CHECKS_VERSION = "1.3";

    private static final long IMPLICIT_WAIT_DEFAULT = 5;


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

//        newDriver.manage().timeouts().implicitlyWait(IMPLICIT_WAIT_DEFAULT, TimeUnit.SECONDS);
//        logger.info("Implicit wait set to : " + IMPLICIT_WAIT_DEFAULT + " " + TimeUnit.SECONDS);

        return newDriver;
    }


    private static WebDriver initChromeDriver() {

        ChromeOptions options = new ChromeOptions();

        String chromeBinary = System.getProperty("webdriver.chrome.bin");
        if(chromeBinary != null) {
            logger.info(("webdriver.chrome.bin = " + chromeBinary));
            options.setBinary(chromeBinary);

        }
        return new ChromeDriver(options);
    }

    private static WebDriver initInternetExplorerDriver() {
        return new InternetExplorerDriver();
    }

    /**
     * -Dwebdriver.firefox.bin=/usr/lib/iceweasel/firefox-bin
     *
     * See http://code.google.com/p/selenium/wiki/FirefoxDriverInternals
     *
     */
    private static WebDriver initFirefoxDriver() {

        WebDriver driver;

        logger.info(("webdriver.firefox.bin = " + System.getProperty("webdriver.firefox.bin")));
        logger.info(("webdriver.firefox.library.path = " + System.getProperty("webdriver.firefox.library.path")));

        FirefoxProfile firefoxProfile = new FirefoxProfile();

        boolean addons = true;

        if(addons){
            try {
                firefoxProfile.addExtension(CdmDataPortalTestBase.class, "/org/mozilla/addons/disable_add_on_compatibility_checks-" + DISABLE_ADD_ON_COMPATIBILITY_CHECKS_VERSION + ".xpi");
                firefoxProfile.setPreference("extensions.checkCompatibility", "false");

//                CdmDataPortalTestBase.logger.debug("FirefoxProfile: " + firefoxProfile.getClass().getSimpleName() + "(" + firefoxProfile.hashCode() + ")");
//                firefoxProfile.addExtension(CdmDataPortalTestBase.class, "/org/mozilla/addons/firebug-" + FIREBUG_VERSION + ".xpi");
//                firefoxProfile.setPreference("extensions.firebug.currentVersion", FIREBUG_VERSION); // avoid displaying first run page
//
//                firefoxProfile.addExtension(CdmDataPortalTestBase.class, "/org/mozilla/addons/firepath-" + FIREXPATH_VERSION + "-fx.xpi");

            } catch (IOException e) {
                logger.error(e);
                System.exit(-1);
            }
        }

        // NativeEvents can only be used with official binary releases of firefox !!!
        // firefoxProfile.setEnableNativeEvents(true);
        // see http://groups.google.com/group/webdriver/browse_thread/thread/ab68c413f17ae1ba/b5cbdcebe859aa56?lnk=gst&q=setEnableNativeEvents+firefox#msg_339ac1870da6d975

        driver = new FirefoxDriver(firefoxProfile);

        return driver;
    }

     public static void main(String[] args){

         initFirefoxDriver();
         try {
            Thread.currentThread();
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            System.err.println("InterruptedException");
        }
    }
}