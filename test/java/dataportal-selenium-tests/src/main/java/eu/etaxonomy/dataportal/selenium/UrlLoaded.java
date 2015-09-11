/**
 *
 */
package eu.etaxonomy.dataportal.selenium;

import org.openqa.selenium.WebDriver;

import com.google.common.base.Function;

/**
 * @author andreas
 *
 */
public class UrlLoaded implements Function<WebDriver, Boolean> {

	String url;

	public UrlLoaded(String url) {
		this.url = url;
	}

	/* (non-Javadoc)
	 * @see com.google.common.base.Function#apply(java.lang.Object)
	 */
	public Boolean apply(WebDriver driver) {
		return driver.getCurrentUrl().equals(url);
	}

}
