package eu.etaxonomy.dataportal;

import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import org.junit.internal.AssumptionViolatedException;
import org.junit.internal.runners.model.EachTestNotifier;
import org.junit.runner.notification.RunNotifier;
import org.junit.runner.notification.StoppedByUserException;
import org.junit.runners.BlockJUnit4ClassRunner;
import org.junit.runners.model.InitializationError;
import org.junit.runners.model.Statement;

public class DataPortalContextAwareRunner extends BlockJUnit4ClassRunner {
	

	@Retention(RetentionPolicy.RUNTIME)
	@Target(ElementType.TYPE)
	@Inherited
	public @interface DataPortalContexts {
		/**
		 * @return an array of DataPortalContext to which the annotated test class is applicable
		 */
		DataPortalContext[] value();
	}
	
	private DataPortalContext dataPortalContext; 
	
	public DataPortalContextAwareRunner(Class<?> klass)
			throws InitializationError {
		super(klass);
		dataPortalContext = DataPortalManager.currentDataPortalContext();
	}
	
	@Override
	public void run(final RunNotifier notifier) {
		EachTestNotifier testNotifier= new EachTestNotifier(notifier,
				getDescription());
		
		boolean isApplicableToContext = false;
		DataPortalContexts dataPortalContextsAnotation = getTestClass().getJavaClass().getAnnotation(DataPortalContexts.class);
		for(DataPortalContext cntxt : dataPortalContextsAnotation.value()){
			if(dataPortalContext.equals(cntxt)){
				isApplicableToContext = true;
			}
		}
		
		if(!isApplicableToContext){
			testNotifier.fireTestIgnored();
			return;
		}
			
		try {
			Statement statement= classBlock(notifier);
			statement.evaluate();
		} catch (AssumptionViolatedException e) {
			testNotifier.fireTestIgnored();
		} catch (StoppedByUserException e) {
			throw e;
		} catch (Throwable e) {
			testNotifier.addFailure(e);
		}
	}

}
