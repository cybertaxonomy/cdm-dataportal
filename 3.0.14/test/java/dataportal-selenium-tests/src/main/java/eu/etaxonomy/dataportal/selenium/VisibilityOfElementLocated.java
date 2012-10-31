/**
 *
 */
package eu.etaxonomy.dataportal.selenium;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;

import com.google.common.base.Function;

/**
 * @author andreas
 *
 */
public class VisibilityOfElementLocated implements Function<WebDriver, Boolean> {

	By findCondition;

	public VisibilityOfElementLocated(By by) {
		this.findCondition = by;
	}

	/* (non-Javadoc)
	 * @see com.google.common.base.Function#apply(java.lang.Object)
	 */
	public Boolean apply(WebDriver driver) {
		driver.findElement(this.findCondition);
		return Boolean.valueOf(true);
	}

}
