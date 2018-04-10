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
import java.util.UUID;

/**
 *
 * @author a.kohlbecker
 *
 */
public class DataPortalContext {

    URI baseUri;
    URI cdmServerUri;
    UUID classificationUUID;
    String siteName; //TODO could be read with drush: $ drush vget site_name
    String themeName;
    private String siteLabel;

    public DataPortalContext(URI baseUri, URI cdmServerUri, UUID classificationUUID, String siteName, String siteLabel) {
            this.baseUri = baseUri;
            this.cdmServerUri = cdmServerUri;
            this.classificationUUID = classificationUUID;
            this.siteName = siteName;
            this.siteLabel = siteLabel;
    }

    public URI getBaseUri() {
        return baseUri;
    }

    /**
     * <code>UNUSED</code>
     * @return
     */
    public URI getCdmServerUri() {
        return cdmServerUri;
    }

    /**
     * <code>UNUSED</code>
     * @return
     */
    public UUID getClassificationUUID() {
        return classificationUUID;
    }

    public String getSiteName() {
        return siteName;
    }

    public String getSiteLabel() {
        return siteLabel;
    }



    /**
     *
     * @param pageHeader
     *
     * @return The drupal site title as it is produced by drupal
     */
    public String prepareTitle(String pageHeader) {
        return pageHeader + " | " + getSiteName();
    }

}
