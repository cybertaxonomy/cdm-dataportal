/**
 *
 */
package eu.etaxonomy.dataportal.junit;

import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.junit.runner.Runner;
import org.junit.runner.notification.RunNotifier;
import org.junit.runners.BlockJUnit4ClassRunner;
import org.junit.runners.Suite;
import org.junit.runners.model.FrameworkMethod;
import org.junit.runners.model.InitializationError;
import org.junit.runners.model.Statement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DataPortalContextProvider;
import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.DataPortalSiteContextProvider;
import eu.etaxonomy.dataportal.DataPortalsListContextProvider;


/**
 * @author a.kohlbecker
 *
 */
public class DataPortalContextSuite extends Suite{

    public static final String SYSTEM_PROPERTY_SITE_LIST_URL = "SiteListUrl";

	/**
	 * Only to be used for test classes which extend {@link CdmDataPortalTestBase}
	 *
	 * @author a.kohlbecker
	 */
	@Retention(RetentionPolicy.RUNTIME)
	@Target(ElementType.TYPE)
	@Inherited
	public @interface DataPortalContexts {

		/**
		 * @return an array of DataPortalSite to which the annotated test
		 *         class is applicable
		 */
		DataPortalSite[] value() default {};

		/**
		 * Alternative configuration option to the default {@link #value()}.
		 *
		 * In case this mode is active the system property <code>SiteListUrl</code> must contain a URL which
		 * points to a resource containing a list of Data Portals base URLs to be tested. Each dataportal URL must be in a
		 * separate line of the text returned by the URL in <code>SiteListUrl</code.
		 */
		boolean siteListUrl() default false;
	}

	private final List<Runner> runners = new ArrayList<Runner>();


	private class TestClassRunnerWithDataPortalContext extends BlockJUnit4ClassRunner {

		private final DataPortalContext context;


		public TestClassRunnerWithDataPortalContext(Class<?> klass, DataPortalContext context) throws InitializationError {
			super(klass);
			this.context = context;
		}

		@Override
		public Object createTest() throws Exception {
			Object testClass = getTestClass().getOnlyConstructor().newInstance();
			((CdmDataPortalTestBase)testClass).setContext(context);
			return testClass;
		}

		@Override
		protected String getName() {
			return String.format("%s@%s", getTestClass().getName(), context.getSiteLabel());
		}

		@Override
		protected String testName(final FrameworkMethod method) {
			return String.format("%s@%s", method.getName(), context.getSiteLabel());

		}

		@Override
		protected Statement classBlock(RunNotifier notifier) {
			return childrenInvoker(notifier);
		}

		@Override
		protected void validateZeroArgConstructor(List<Throwable> errors) {
			super.validateZeroArgConstructor(errors);
			validateCdmDataPortalTestBase(errors);
		}

		protected void validateCdmDataPortalTestBase(List<Throwable> errors) {
			// constructor should have exactly one arg
			if ( ! CdmDataPortalTestBase.class.isAssignableFrom(getTestClass().getJavaClass()) ){
				String gripe= "Test class must be a subclass of " + CdmDataPortalTestBase.class.getName();
				errors.add(new Exception(gripe));
			}
		}
	}

	/**
	 * Only called reflectively. Do not use programmatically.
	 */
	public DataPortalContextSuite(Class<?> klass) throws InitializationError {
		super(klass, Collections.<Runner>emptyList());
		DataPortalContexts dataPortalContextsAnotation = getTestClass().getJavaClass().getAnnotation(DataPortalContexts.class);
		DataPortalContextProvider contextProvider = null;

		if(dataPortalContextsAnotation.siteListUrl()){
            String siteListUrlString = System.getProperty(SYSTEM_PROPERTY_SITE_LIST_URL);
            if(System.getProperty(SYSTEM_PROPERTY_SITE_LIST_URL) == null) {
                throw new RuntimeException("The system property " + SYSTEM_PROPERTY_SITE_LIST_URL + " must be set if 'siteListUrl' is enabled");
            }
            try {
                contextProvider = new DataPortalsListContextProvider(new URL(siteListUrlString));
            } catch (MalformedURLException e) {
                throw new RuntimeException("Error parsing the provided URL", e);
            }
		} else {
		    contextProvider = new DataPortalSiteContextProvider(dataPortalContextsAnotation.value());
		}

		assert contextProvider != null;
		for (DataPortalContext dataPortalContext : contextProvider.contexts()) {
		    runners.add(new TestClassRunnerWithDataPortalContext(klass, dataPortalContext));
		}
	}

	@Override
	protected List<Runner> getChildren() {
		return runners;
	}

}
