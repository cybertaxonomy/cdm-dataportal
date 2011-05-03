package eu.etaxonomy.dataportal.selenium;

import org.junit.BeforeClass;
import org.openqa.selenium.WebDriverBackedSelenium;

import com.thoughtworks.selenium.Selenium;

import eu.etaxonomy.dataportal.DataPortalManager;

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
