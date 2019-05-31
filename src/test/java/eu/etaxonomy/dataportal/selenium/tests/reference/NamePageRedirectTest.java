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
import java.util.UUID;

import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

import com.thoughtworks.selenium.webdriven.commands.WaitForPageToLoad;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.GenericPortalPage;

/**
 * Issues to be covered by this TestClass:
 *
 * #3616
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class NamePageRedirectTest extends CdmDataPortalTestBase{

    static final UUID taxon_achilllea_santolina_uuid = UUID.fromString("c246856f-c03e-4cb7-ac92-d9b2864084cd");
    static final UUID name_achilllea_santolina_uuid = UUID.fromString("2ff1fb18-7055-420f-8c10-5105b66974de");


    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getBaseUri().toString());
    }

    /**
     * related to https://dev.e-taxonomy.eu/redmine/issues/8304
     */
    @Test
    @Ignore
    public void testNoRedirect() throws MalformedURLException {

        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "name/" + name_achilllea_santolina_uuid.toString() + "/null/null");
        assertTrue(
                "The target page should be a name page, no redirect must have happened.",
                p.getDrupalPagePath().startsWith("cdm_dataportal/name/" + name_achilllea_santolina_uuid.toString()));

        p = new GenericPortalPage(driver, getContext(), "name/" + name_achilllea_santolina_uuid.toString() + "/null/null/null");
        assertTrue(
                "The target page should be a name page, no redirect must have happened.",
                p.getDrupalPagePath().startsWith("cdm_dataportal/name/" + name_achilllea_santolina_uuid.toString()));

        p = new GenericPortalPage(driver, getContext(), "name/" + name_achilllea_santolina_uuid.toString() + "///");
        assertTrue(
                "The target page should be a name page, no redirect must have happened.",
                p.getDrupalPagePath().startsWith("cdm_dataportal/name/" + name_achilllea_santolina_uuid.toString()));

    }


    /**
     * related to https://dev.e-taxonomy.eu/redmine/issues/8304
     */
    @Test
    public void testRedirectToTaxon() throws MalformedURLException {


        WaitForPageToLoad wait = new WaitForPageToLoad();
        String timeout = "5";
        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "name/" + name_achilllea_santolina_uuid.toString() + "/null/null/redirect_to_taxon");
        wait.apply(driver, new String[] {timeout});
        wait.apply(driver, new String[] {timeout});
        logger.debug(p.getDrupalPagePath());
        assertTrue(
                "The target page should be a taxon page, the name page must have been redirected to the according taxon page.",
                p.getDrupalPagePath().startsWith("cdm_dataportal/taxon/" + taxon_achilllea_santolina_uuid.toString()));

        p = new GenericPortalPage(driver, getContext(), "name/" + name_achilllea_santolina_uuid.toString() + "///redirect_to_taxon");
        wait.apply(driver, new String[] {timeout});
        assertTrue(
                "The target page should be a taxon page, the name page must have been redirected to the according taxon page.",
                p.getDrupalPagePath().startsWith("cdm_dataportal/taxon/" + taxon_achilllea_santolina_uuid.toString()));

    }

}
