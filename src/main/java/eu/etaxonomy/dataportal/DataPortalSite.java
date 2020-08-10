/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal;

import java.net.URI;

import org.apache.log4j.Logger;

/**
 *
 * @author a.kohlbecker
 *
 */
public enum DataPortalSite {

    cichorieae,
    palmae,
    cyprus,
    floramalesiana,
    reference;

    DataPortalContext context;

    public final Logger logger = Logger.getLogger(DataPortalSite.class);

    private DataPortalSite() {

        try {

            URI siteUri = TestConfiguration.getProperty(composePropertyKey("siteUri"), URI.class, true);
            String siteName = TestConfiguration.getProperty(composePropertyKey("siteName"), true);
            String drupalRoot = TestConfiguration.getProperty(composePropertyKey("drupalRoot"), true);
            String sshHost = TestConfiguration.getProperty(composePropertyKey("sshHost"));
            String sshUser = TestConfiguration.getProperty(composePropertyKey("sshUser"));
            context = new DataPortalContext(siteUri, siteName, drupalRoot, sshHost, sshUser);
        } catch (TestConfigurationException e) {
            logger.error("Configuration Error: ", e);
            System.exit(-1);
        }
    }

    private String composePropertyKey(String fieldName) {
        String key = DataPortalContext.class.getSimpleName().substring(0, 1).toLowerCase() + DataPortalContext.class.getSimpleName().substring(1) + "." + this.name() + "." + fieldName;
        return key;
    }

    /**
     * @return
     */
    public DataPortalContext getContext() {
        return context;
    }

}
