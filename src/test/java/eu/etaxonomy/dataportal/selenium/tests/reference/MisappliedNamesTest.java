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

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.reference })
public class MisappliedNamesTest extends CdmDataPortalTestBase{

    static final UUID miconia_cubacinerea_Uuid = UUID.fromString("c6716cee-2039-4ba8-a239-4b1b353f9c84");

    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getBaseUri().toString());
    }

    /**
     * Test for correct sensu representation of misapplied names, see #5676
     */
    @Test
    public void testTitleAndTabs() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), miconia_cubacinerea_Uuid);

        WebElement misappliedName = p.getMisappliedName(1);
        assertNotNull(misappliedName);
        assertEquals("–\n\"Ossaea glomerata\" sensu Species solaris; sensu A&S; sensu Lem1", misappliedName.getText());
        List<BaseElement> footnotes = ElementUtils.findFootNotes(p.getTaxonRelationships());
        assertEquals(1, footnotes.size());
        assertEquals("1. Lem, New Species in the solar system", footnotes.get(0).getText());

    }



}
