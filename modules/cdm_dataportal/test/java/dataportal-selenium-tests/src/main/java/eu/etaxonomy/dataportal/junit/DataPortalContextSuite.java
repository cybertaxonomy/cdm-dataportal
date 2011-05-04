/**
 *
 */
package eu.etaxonomy.dataportal.junit;

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
import org.junit.runners.model.TestClass;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DataPortalContexts;
import eu.etaxonomy.dataportal.selenium.CdmDataPortalTestBase;


/**
 * @author a.kohlbecker
 *
 */
public class DataPortalContextSuite extends Suite{

	private final List<Runner> runners = new ArrayList<Runner>();


	private class TestClassRunnerWithDataPortalContext extends
	BlockJUnit4ClassRunner {

		private final DataPortalContext context;

		/**
		 * @param klass
		 * @throws InitializationError
		 */
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
			return String.format("%s@%s", getTestClass().getName(), context.name());
		}

		@Override
		protected String testName(final FrameworkMethod method) {
			return String.format("%s@%s", method.getName(), context.name());

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
		for (DataPortalContext cntxt : dataPortalContextsAnotation.value()) {
			runners.add(new TestClassRunnerWithDataPortalContext(klass, cntxt));
		}
	}

	@Override
	protected List<Runner> getChildren() {
		return runners;
	}

}
