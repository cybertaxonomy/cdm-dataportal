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

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DataPortalContexts;


/**
 * @author a.kohlbecker
 *
 */
public class DataPortalContextSuite extends Suite{

	private final List<Runner> runners = new ArrayList<Runner>();
	
	
	private class TestClassRunnerWithDataPortalContext extends
	BlockJUnit4ClassRunner {
		
		private DataPortalContext context;

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
			return getTestClass().getOnlyConstructor().newInstance(context);
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
			// constructor should have exactly one arg
			if (hasOneConstructor()
					&& !(getTestClass().getOnlyConstructor().getParameterTypes().length == 1)) {
				String gripe= "Test class should have exactly one public constructor with DataPortalContext as argument";
				errors.add(new Exception(gripe));
			}
		}
		
		private boolean hasOneConstructor() {
			return getTestClass().getJavaClass().getConstructors().length == 1;
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
