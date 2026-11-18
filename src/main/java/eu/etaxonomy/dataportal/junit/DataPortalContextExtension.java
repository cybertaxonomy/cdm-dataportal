/**
* Copyright (C) 2026 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.junit;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.Collections;
import java.util.List;
import java.util.stream.Stream;

import org.junit.jupiter.api.extension.Extension;
import org.junit.jupiter.api.extension.ExtensionContext;
import org.junit.jupiter.api.extension.TestInstancePostProcessor;
import org.junit.jupiter.api.extension.TestTemplateInvocationContext;
import org.junit.jupiter.api.extension.TestTemplateInvocationContextProvider;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DataPortalContextProvider;
import eu.etaxonomy.dataportal.DataPortalSiteContextProvider;
import eu.etaxonomy.dataportal.DataPortalsListContextProvider;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;

/**
 * JUnit 5 replacement for the former {@code DataPortalContextSuite} runner:
 * expands each {@link DataPortalTest} method once per configured portal context
 * and injects that context into {@link CdmDataPortalTestBase}.
 */
public class DataPortalContextExtension implements TestTemplateInvocationContextProvider {

    public static final String SYSTEM_PROPERTY_SITE_LIST_URL = DataPortalContextSuite.SYSTEM_PROPERTY_SITE_LIST_URL;

    @Override
    public boolean supportsTestTemplate(ExtensionContext context) {
        return context.getTestMethod().isPresent()
                && context.getTestMethod().get().isAnnotationPresent(DataPortalTest.class);
    }

    @Override
    public Stream<TestTemplateInvocationContext> provideTestTemplateInvocationContexts(ExtensionContext context) {
        Class<?> testClass = context.getRequiredTestClass();
        if (!CdmDataPortalTestBase.class.isAssignableFrom(testClass)) {
            throw new IllegalStateException(
                    "Test class must be a subclass of " + CdmDataPortalTestBase.class.getName());
        }
        List<DataPortalContext> portals = resolveContexts(testClass);
        if (portals.isEmpty()) {
            throw new IllegalStateException(
                    "No DataPortalContext resolved for " + testClass.getName()
                            + " — annotate with @" + DataPortalContexts.class.getSimpleName());
        }
        return portals.stream().map(DataPortalInvocationContext::new);
    }

    static List<DataPortalContext> resolveContexts(Class<?> testClass) {
        DataPortalContexts annotation = testClass.getAnnotation(DataPortalContexts.class);
        if (annotation == null) {
            return Collections.emptyList();
        }
        DataPortalContextProvider contextProvider;
        if (annotation.siteListUrl()) {
            String siteListUrlString = System.getProperty(SYSTEM_PROPERTY_SITE_LIST_URL);
            if (siteListUrlString == null) {
                throw new IllegalStateException(
                        "The system property " + SYSTEM_PROPERTY_SITE_LIST_URL
                                + " must be set if 'siteListUrl' is enabled");
            }
            try {
                contextProvider = new DataPortalsListContextProvider(new URL(siteListUrlString));
            } catch (MalformedURLException e) {
                throw new IllegalStateException("Error parsing the provided URL", e);
            }
        } else {
            contextProvider = new DataPortalSiteContextProvider(annotation.value());
        }
        return contextProvider.contexts();
    }

    private static final class DataPortalInvocationContext implements TestTemplateInvocationContext {

        private final DataPortalContext portalContext;

        private DataPortalInvocationContext(DataPortalContext portalContext) {
            this.portalContext = portalContext;
        }

        @Override
        public String getDisplayName(int invocationIndex) {
            String name = portalContext.getSiteName();
            if (name == null || name.isBlank()) {
                name = String.valueOf(portalContext.getSiteUri());
            }
            return name;
        }

        @Override
        public List<Extension> getAdditionalExtensions() {
            return List.of((TestInstancePostProcessor) (testInstance, context) -> {
                ((CdmDataPortalTestBase) testInstance).setContext(portalContext);
            });
        }
    }
}
