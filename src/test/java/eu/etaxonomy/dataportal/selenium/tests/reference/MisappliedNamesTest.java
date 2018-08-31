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
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.StringConstants;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * Issues to be covered by this TestClass:
 *
 * #5676
 * #5647 - OK
 * #5492 - OK
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class MisappliedNamesTest extends CdmDataPortalTestBase{

    static final UUID miconia_cubacinerea_Uuid = UUID.fromString("c6716cee-2039-4ba8-a239-4b1b353f9c84");


    static final UUID trichocentrum_undulatum_Uuid = UUID.fromString("7e86b2a4-ba71-4494-b544-ae5656e02ed2");

    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getBaseUri().toString());
    }

    /**
     * Test for correct sensu representation of misapplied names, see #5676 and #5647
     *
     * https://dev.e-taxonomy.eu/redmine/issues/5647
     *
     * NOTE: Species solaris has no authorship!!
     */
    @Test
    public void tesIssue5647() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), miconia_cubacinerea_Uuid);

        WebElement misappliedName = p.getMisappliedName(1);
        assertNotNull(misappliedName);
        assertEquals("–\n\"Ossaea glomerata\" sensu Species solaris; sensu A&S; sensu Lem1", misappliedName.getText());
        List<BaseElement> footnotes = ElementUtils.findFootNotes(p.getTaxonRelationships());
        assertEquals(1, footnotes.size());
        assertEquals("1. Lem, New Species in the solar system", footnotes.get(0).getText());

    }


    /**
     * https://dev.e-taxonomy.eu/redmine/issues/5492
     */
    @Test
    public void testIssue5492() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), trichocentrum_undulatum_Uuid);

        WebElement misappliedName1 = p.getMisappliedName(1);
        assertNotNull(misappliedName1);
        assertEquals("–\n\"Oncidium guttatum\" auct. sensu Greuter, W. & Rankin Rodríguez, R1", misappliedName1.getText());

        WebElement misappliedName2 = p.getMisappliedName(2);
        assertNotNull(misappliedName2);
        assertEquals("–\n" + StringConstants.DOUBTFULMARKER_SPACE +"\"Oncidium carthaginense\" auct. sensu Greuter, W. & Rankin Rodríguez, R1", misappliedName2.getText());


        List<BaseElement> footnotes = ElementUtils.findFootNotes(p.getTaxonRelationships());
        assertEquals(1, footnotes.size());
        assertEquals("1. Greuter, W. & Rankin Rodríguez, R, Plantas vasculares de Cuba: inventario preliminar. Tercera edición, actualizada. Vascular plants of Cuba: a preliminary checklist. Third updated edition.", footnotes.get(0).getText());

    }



}
