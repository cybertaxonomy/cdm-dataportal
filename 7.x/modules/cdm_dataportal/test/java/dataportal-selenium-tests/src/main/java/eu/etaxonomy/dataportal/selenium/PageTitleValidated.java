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
public class PageTitleValidated implements Function<WebDriver, Boolean> {

    private final String title;

    public PageTitleValidated(String title) {
        this.title = title;
    }

    /* (non-Javadoc)
     * @see com.google.common.base.Function#apply(java.lang.Object)
     */
    @Override
    public Boolean apply(WebDriver driver) {
        boolean validated = driver.getTitle().equals(title);
        return Boolean.valueOf(validated);
    }

}
