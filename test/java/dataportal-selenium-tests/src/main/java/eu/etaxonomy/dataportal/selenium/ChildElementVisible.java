/**
 *
 */
package eu.etaxonomy.dataportal.selenium;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;

import com.google.common.base.Function;

/**
 * @author andreas
 *
 */
public class ChildElementVisible implements Function<WebDriver, Boolean> {

	By findCondition;
	WebElement parent;

	public ChildElementVisible(WebElement parent, By by) {
		this.findCondition = by;
		this.parent = parent;
	}

	/* (non-Javadoc)
	 * @see com.google.common.base.Function#apply(java.lang.Object)
	 */
	public Boolean apply(WebDriver driver) {
		parent.findElement(this.findCondition);
		return Boolean.valueOf(true);
	}

}
