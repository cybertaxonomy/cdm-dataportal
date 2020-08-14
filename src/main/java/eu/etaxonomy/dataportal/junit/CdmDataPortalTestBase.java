/**
 *
 */
package eu.etaxonomy.dataportal.junit;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.openqa.selenium.WebDriver;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.DrupalVars;
import eu.etaxonomy.dataportal.selenium.WebDriverFactory;
import eu.etaxonomy.drush.DrushExecuter;
import eu.etaxonomy.drush.DrushExecutionFailure;

/**
 * @author a.kohlbecker
 *
 */
@RunWith(DataPortalContextSuite.class)
public abstract class CdmDataPortalTestBase extends Assert{

	public static final Logger logger = Logger.getLogger(CdmDataPortalTestBase.class);

	protected static WebDriver driver;

	private DataPortalContext context;

    private Map<String,Object> drupalVarsBeforeTest = new HashMap<>();

	public DataPortalContext getContext() {
		return context;
	}

	public void setContext(DataPortalContext context) {
		this.context = context;

	}

	/**
     * Return the {@link DataPortalSite#getSiteUri()} of the currently active
     * context as String
     *
     * @return string representation of the DataPortal site URI
     */
    public String getSiteUrl() {
    	return context.getSiteUri().toString();
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

	@After
    public void resetToOriginalState() throws IOException, InterruptedException, DrushExecutionFailure {
        restoreOriginalVars();
    }

    /**
     * Safely set a Drupal variable to a new value. Any changes to the Drupal
     * variables are reset after the test through {@link #resetToOriginalState()}.
     *
     * @param varKey The key of the Drupal variable to set. In {@link DrupalVars}
     * predefined variable key constants can be found.
     *
     * @param varValue The value to set
     *
     * @throws IOException
     * @throws InterruptedException
     * @throws DrushExecutionFailure
     */
    protected void setDrupalVar(String varKey, String varValue) throws IOException, InterruptedException, DrushExecutionFailure {
        DrushExecuter dex = getContext().drushExecuter();
        List<Object> result = dex.execute(DrushExecuter.variableGet, varKey);
        assertEquals(1, result.size());
        if(!drupalVarsBeforeTest.containsKey(varKey)) {
            // stored original values must not be replaced
            drupalVarsBeforeTest.put(varKey, result.get(0));
        }
        result = dex.execute(DrushExecuter.variableSet, varKey, varValue);
    }

    protected void restoreOriginalVars() throws IOException, InterruptedException, DrushExecutionFailure {
        DrushExecuter dex = getContext().drushExecuter();
        boolean fail = false;
        for(String varKey : drupalVarsBeforeTest.keySet()) {
            try {
                List<Object> result = dex.execute(DrushExecuter.variableSet, varKey, drupalVarsBeforeTest.get(varKey).toString());
            } catch (Exception e) {
                logger.error("FATAL ERROR: Restoring the original drupal variable " + varKey + " = " + drupalVarsBeforeTest.get(varKey) + " failed.", e);
                fail = true;
            }
        }
        drupalVarsBeforeTest.clear();
        if(fail) {
            throw new IOException("Restoring a original drupal variable has previously failed. You may want to fix the site settings manually!");
        }
    }


}
