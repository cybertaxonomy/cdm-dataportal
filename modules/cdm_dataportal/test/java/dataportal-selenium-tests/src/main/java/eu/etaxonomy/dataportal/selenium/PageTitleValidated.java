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
public class PageTitleValidated implements Function<WebDriver, Boolean> {

	private String title;

	public PageTitleValidated(String title) {
		this.title = title;
	}

	/* (non-Javadoc)
	 * @see com.google.common.base.Function#apply(java.lang.Object)
	 */
	public Boolean apply(WebDriver driver) {
		boolean validated = driver.findElement(By.tagName("title")).getText().equals(title);
		return Boolean.valueOf(validated);
	}

}
