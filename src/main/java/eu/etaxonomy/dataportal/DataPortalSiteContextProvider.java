/**
* Copyright (C) 2018 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal;

import java.util.ArrayList;
import java.util.List;

/**
 * @author a.kohlbecker
 * @since Apr 10, 2018
 *
 */
public class DataPortalSiteContextProvider implements DataPortalContextProvider {

    DataPortalSite[] dataPortalSites;

    public DataPortalSiteContextProvider(DataPortalSite[] dataPortalSites){
        this.dataPortalSites = dataPortalSites;
    }


    @Override
    public List<DataPortalContext> contexts() {
        List<DataPortalContext> contexts = new ArrayList<>(dataPortalSites.length);
        for (DataPortalSite dataPortalSite : dataPortalSites) {
            contexts.add(dataPortalSite.getContext());
        }
        return contexts;
    }


}
