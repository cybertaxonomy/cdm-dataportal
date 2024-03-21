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
import java.util.List;
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.GenericPortalPage;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;
import eu.etaxonomy.dataportal.elements.TypeDesignationElement;

/**
 * Issues to be covered by this TestClass:
 *
 * #9230
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class NamePageTest extends CdmDataPortalTestBase{

    static final UUID name_nodosilinea_radiophila_uuid = UUID.fromString("e97cc25b-ec11-4bb8-88d7-ab40a023f3fb");
    static final UUID taxon_nodosilinea_sensenia_uuid = UUID.fromString("7094ea13-2a95-46d9-bfca-8c0e0848e44c");

    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }

    /**
     * related to https://dev.e-taxonomy.eu/redmine/issues/9230
     */
    @Test
    public void testUnpublishedTypeDesignationsHidden() throws MalformedURLException {

        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "name/" + name_nodosilinea_radiophila_uuid.toString() + "/null/null");
        // expecting to land on name page, see NamePageRedirectTest for other cases
        assertTrue(p.getDrupalPagePath().startsWith("cdm_dataportal/name/" + name_nodosilinea_radiophila_uuid.toString()));
        WebElement typeDesignationsContainer = p.getDataPortalContent().getElement().findElement(By.cssSelector("div.typeDesignations"));
        List<WebElement> typeDesignations = typeDesignationsContainer.findElements(By.xpath("./div"));
        //change to 3 because 2 paratypes are added to test #10482
        assertEquals(3, typeDesignations.size());
        typeDesignations.get(0);
        assertTrue(typeDesignations.get(2).getAttribute("class").contains("cdm:SpecimenTypeDesignation uuid:dbf91118-1c09-40f6-a3d0-2d9d4b88ac34"));
    }
    /**
         * related to https://dev.e-taxonomy.eu/redmine/issues/9967
         */
    @Test
    public void testNotDesignatedTypeDesignation() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), taxon_nodosilinea_sensenia_uuid);
        List<TypeDesignationElement> typeDesignations = p.getHomotypicalGroupTypeDesignations();
        //GenericPortalPage p = new GenericPortalPage(driver, getContext(), "taxon/" + name_nodosilinea_sensenia_uuid.toString() + "/synonymy");
        // expecting to land on name page, see NamePageRedirectTest for other cases
       // assertTrue(p.getDrupalPagePath().startsWith("cdm_dataportal/taxon/" + name_nodosilinea_sensenia_uuid.toString()));
        assertEquals(1, typeDesignations.size());
        assertTrue(typeDesignations.get(0).getText().contains("Type: not designated"));

    }


}
