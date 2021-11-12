Lem[,] New Species in the/**
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
import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * Issues to be covered by this TestClass:
 *
 * #3616
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class DescriptionElementsTest extends CdmDataPortalTestBase{

    static final UUID achilllea_santolina_uuid = UUID.fromString("c246856f-c03e-4cb7-ac92-d9b2864084cd");


    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }

    /**
     * Test to reproduce issue https://dev.e-taxonomy.eu/redmine/issues/3616
     *
     */
    @Test
    public void tesIssue3616() throws MalformedURLException {


        TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), achilllea_santolina_uuid);

        FeatureBlock fb = p.getFeatureBlockAt(0, "biology-and-ecology", "div", "span");
        assertNotNull(fb);
        assertEquals(3, fb.getFeatureBlockElements().size());
        assertEquals("Flowers with white blossoms on Testisland A. 0000-05 to 0000-06 (Lem: New Species in the solar system: p.99)", fb.getFeatureBlockElements().get(0).getText());
        assertEquals("Flowers with white blossoms on Testisland B. Mai to June (Lem: New Species in the solar system: p.99)", fb.getFeatureBlockElements().get(1).getText());
        assertEquals("Flowers with white blossoms on Testisland C. 2000 to 2000-05", fb.getFeatureBlockElements().get(2).getText());

    }

}
