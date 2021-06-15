/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.reference;

import java.net.MalformedURLException;

import org.junit.Before;
import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.GenericPortalPage;

/**
 */

@DataPortalContexts( {
    DataPortalSite.reference,
    DataPortalSite.cichorieae,
    DataPortalSite.palmae,
    DataPortalSite.cyprus
    })
public class AdvancedSearchFormTest extends CdmDataPortalTestBase{


    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }

    @Test
    public void testTaxonSearchForm() throws MalformedURLException {

        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "search");
        assertTrue(driver.getTitle().startsWith("Advanced search | "));

    }

    @Test
    public void testFactualDataSearchForm() throws MalformedURLException {

        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "search/taxon_by_description");
        assertTrue(driver.getTitle().startsWith("Search by factual data | "));

    }

}
