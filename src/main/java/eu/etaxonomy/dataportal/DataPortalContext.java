/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal;

import java.io.File;
import java.io.IOException;
import java.net.URI;

import eu.etaxonomy.drush.DrushExecuter;
import eu.etaxonomy.drush.DrushExecutionFailure;

/**
 *
 * @author a.kohlbecker
 *
 */
public class DataPortalContext {

    URI siteUri;
    String siteName; //TODO read with drush: $ drush vget site_name
    String themeName;
    File drupalRoot;
    String sshHost;
    String sshUser;

    public DataPortalContext(URI siteUri, String siteName, String drupalRoot, String sshHost, String sshUser) {
            this.siteUri = siteUri;
            this.siteName = siteName;
            this.drupalRoot = new File(drupalRoot);
            this.sshHost = sshHost;
            this.sshUser = sshUser;
    }

    public URI getSiteUri() {
        return siteUri;
    }

    public String getSiteName() {
        return siteName;
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

    public DrushExecuter drushExecuter() throws IOException, InterruptedException, DrushExecutionFailure {
        DrushExecuter dex = new DrushExecuter();
        dex.setDrupalRoot(drupalRoot);
        dex.setSiteURI(siteUri);
        dex.setSshHost(sshHost);
        dex.setSshUser(sshUser);
        return dex;
    }

}
