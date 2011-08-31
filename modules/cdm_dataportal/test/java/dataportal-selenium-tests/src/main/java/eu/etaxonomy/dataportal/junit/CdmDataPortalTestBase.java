/**
 *
 */
package eu.etaxonomy.dataportal.junit;

import org.apache.log4j.Logger;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.openqa.selenium.WebDriver;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.selenium.WebDriverFactory;

/**
 * @author a.kohlbecker
 *
 */
@RunWith(DataPortalContextSuite.class)
public abstract class CdmDataPortalTestBase {

	public static final Logger logger = Logger.getLogger(CdmDataPortalTestBase.class);

	protected static WebDriver driver;

	private DataPortalContext context;

	public DataPortalContext getContext() {
		return context;
	}

	public void setContext(DataPortalContext context) {
		this.context = context;

	}

	@BeforeClass
	public static void setUpDriver() {
		logger.debug("@BeforeClass: setUpDriver()");
		driver = WebDriverFactory.newWebDriver();
	}

	@AfterClass
	public static void closeDriver() {
		logger.debug("@AfterClass: closeDriver()");
		if (driver != null) {
			driver.quit();
		}
	}

	/**
	 * Return the {@link DataPortalContext#getBaseUri()} of the currently active
	 * context as String
	 *
	 * @return string representatoin of the DataPortal base URI
	 */
	public String getBaseUrl() {
		return context.getBaseUri().toString();
	}


}
