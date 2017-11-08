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


    public JUnitWebDriverWait(WebDriver driver, long timeOutInSeconds) {
        super(driver, timeOutInSeconds);
    }

    @Override
    protected RuntimeException timeoutException(String message, Throwable lastException) {
        throw new AssertionError(message);
     }

}
