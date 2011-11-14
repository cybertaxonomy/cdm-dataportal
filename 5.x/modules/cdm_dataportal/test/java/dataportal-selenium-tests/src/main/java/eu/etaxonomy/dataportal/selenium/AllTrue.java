/**
 *
 */
package eu.etaxonomy.dataportal.selenium;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.openqa.selenium.WebDriver;

import com.google.common.base.Function;

/**
 * @author andreas
 *
 */
public class AllTrue implements Function<WebDriver, Boolean> {

	List<Function<WebDriver, Boolean>> functions = new ArrayList<Function<WebDriver, Boolean>>();

	public AllTrue(Function<WebDriver, Boolean> ... functions) {
		if(functions == null){
			throw new NullPointerException("Constructor parameter mus not be null");
		}
		this.functions = Arrays.asList(functions);
	}

	/* (non-Javadoc)
	 * @see com.google.common.base.Function#apply(java.lang.Object)
	 */
	public Boolean apply(WebDriver driver) {
		Boolean allTrue = true;
		for(Function<WebDriver, Boolean> f : functions){
			allTrue &= f.apply(driver);
		}
		return allTrue;
	}

}
