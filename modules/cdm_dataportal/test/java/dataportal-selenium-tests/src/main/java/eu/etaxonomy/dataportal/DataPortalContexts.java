/**
 * 
 */
package eu.etaxonomy.dataportal;

import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Inherited
public @interface DataPortalContexts {
	/**
	 * @return an array of DataPortalContext to which the annotated test
	 *         class is applicable
	 */
	DataPortalContext[] value();
}