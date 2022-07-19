/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.junit;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.openqa.selenium.WebDriver;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DrupalVars;
import eu.etaxonomy.dataportal.selenium.WebDriverFactory;
import eu.etaxonomy.drush.DrushExecuter;
import eu.etaxonomy.drush.DrushExecutionFailure;

/**
 * @author a.kohlbecker
 */
@RunWith(DataPortalContextSuite.class)
public abstract class CdmDataPortalTestBase extends Assert{

    private static final Logger logger = LogManager.getLogger();

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
     * Return the URL of the currently active
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
        assertTrue(0 <= result.size() && result.size() <= 1);
        if(result.size() == 1 && !drupalVarsBeforeTest.containsKey(varKey)) {
            // stored original values must not be replaced
            drupalVarsBeforeTest.put(varKey, result.get(0));
        }
        result = dex.execute(DrushExecuter.variableSet, varKey, varValue);
    }

    protected void setDrupalVarJson(String varKey, String varValue) throws IOException, InterruptedException, DrushExecutionFailure {
        DrushExecuter dex = getContext().drushExecuter();
        List<Object> result = dex.execute(DrushExecuter.variableGet, varKey);
        assertTrue(0 <= result.size() && result.size() <= 1);
        if(result.size() == 1 && !drupalVarsBeforeTest.containsKey(varKey)) {
            // stored original values must not be replaced
            drupalVarsBeforeTest.put(varKey, result.get(0));
        } else {
            // empty value will unset
            drupalVarsBeforeTest.put(varKey, "");
        }
        result = dex.execute(DrushExecuter.variableSetJson, varKey, varValue);
    }

    protected void restoreOriginalVars() throws IOException, InterruptedException, DrushExecutionFailure {
        DrushExecuter dex = getContext().drushExecuter();
        boolean fail = false;
        for(String varKey : drupalVarsBeforeTest.keySet()) {
            try {
                dex.execute(DrushExecuter.variableSetJson, varKey, drupalVarsBeforeTest.get(varKey).toString());
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
