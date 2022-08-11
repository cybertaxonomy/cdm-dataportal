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

//import org.apache.log4j.Logger;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.ImgElement;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.elements.MultipartDescriptionElementRepresentation;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class TaxonProfileTest extends CdmDataPortalTestBase{

    //public static final Logger logger = Logger.getLogger(TaxonProfileTest.class);
    //Iconella splendida (Ehrenb.) Ruck & Nakov sec. Specimen Import 2020-10-12 (sub Iconella splendi)
    static UUID taxonUuid = UUID.fromString("cf5775b5-71a1-4776-955d-91516dc63318");

    TaxonSynonymyPage p = null;

    @Before
    public void setUp() throws MalformedURLException {

        driver.get(getContext().getSiteUri().toString());
        if(p == null) {
            p = new TaxonSynonymyPage(driver, getContext(), taxonUuid);
        }

    }


    @Test
    public void testSecundumNameInSource() {


        WebElement acceptedName = p.getAcceptedName();
        assertEquals("Iconella splendida (Ehrenb.) Ruck & Nakov sec. Specimen Import 2020-10-12 (sub Iconella splendi)", acceptedName.getText());
        WebElement referenceElement = acceptedName.findElement(By.cssSelector(".secReference"));
        assertEquals("Specimen Import 2020-10-12 (sub Iconella splendi)", referenceElement.getText());

    }

}
