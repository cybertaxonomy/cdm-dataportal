/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.junit;

import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import eu.etaxonomy.dataportal.DataPortalSite;

/**
 * Holder for {@link DataPortalContexts}. The former JUnit 4 {@code Suite} runner
 * was replaced by {@link DataPortalContextExtension} / {@link DataPortalTest}.
 *
 * @author a.kohlbecker
 */
public final class DataPortalContextSuite {

    public static final String SYSTEM_PROPERTY_SITE_LIST_URL = "SiteListUrl";

    private DataPortalContextSuite() {
    }

    /**
     * Only to be used for test classes which extend {@link CdmDataPortalTestBase}.
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
         * <p>
         * When enabled, the system property {@code SiteListUrl} must contain a URL
         * which points to a resource listing Data Portal base URLs (one per line).
         */
        boolean siteListUrl() default false;
    }
}
