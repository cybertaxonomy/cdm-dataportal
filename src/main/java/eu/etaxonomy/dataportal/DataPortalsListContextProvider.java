/**
* Copyright (C) 2018 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;

/**
 * Retrieves a list of data portal URLs from the URL passed as paramter to the constructor
 * @author a.kohlbecker
 * @since Apr 10, 2018
 *
 */
public class DataPortalsListContextProvider implements DataPortalContextProvider {

    List<URL> dataPoralUrls = new ArrayList<>();

    public DataPortalsListContextProvider(URL url){

        // FIXME read the list of dataPoralUrls from url and fill dataPoralUrls

    }

    /**
     * {@inheritDoc}
     */
    @Override
    public List<DataPortalContext> contexts() {
        // FIXME create the DataPortalContext for each of the dataPoralUrls
        //       read any additional from the data portal or from the cdm webservices
        //       if this is needed for testing.
        return null;
    }

}
