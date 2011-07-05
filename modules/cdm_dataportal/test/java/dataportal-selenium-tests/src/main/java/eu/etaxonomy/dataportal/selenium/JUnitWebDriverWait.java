/**
 *
 */
package eu.etaxonomy.dataportal.selenium;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.ui.WebDriverWait;

/**
 * @author andreas
 *
 */
public class JUnitWebDriverWait extends WebDriverWait {


	/**
	 * @param driver
	 * @param timeOutInSeconds
	 */
	public JUnitWebDriverWait(WebDriver driver, long timeOutInSeconds) {
		super(driver, timeOutInSeconds);
	}

	@Override
	protected RuntimeException timeoutException(String message, RuntimeException lastException) {
	    throw new AssertionError(message);
	 }

}
