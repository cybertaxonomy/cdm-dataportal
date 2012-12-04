// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.diptera;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.diptera})
public class Diptera_OriginalSourceTest extends CdmDataPortalTestBase{

    // Milichia speciosa
    static UUID taxonUuid = UUID.fromString("1f1f8356-a172-4f7d-ad98-e8a37489ce9f");

    TaxonProfilePage p = null;

    @Before
    public void setUp() throws MalformedURLException {

        p = new TaxonProfilePage(driver, getContext(), taxonUuid);

    }

    @After
    public void tearDown(){
        logger.debug("@After");
    }


    @Test
    public void testPage() {

        assertEquals(getContext().prepareTitle("Milichia speciosa"), p.getTitle());
//		assertNull("Authorship information should be hidden", p.getAuthorInformationText());

        List<LinkElement> primaryTabs = p.getPrimaryTabs();
        assertEquals("Expecting 3 tabs", 3, primaryTabs.size());
        assertEquals("General", primaryTabs.get(0).getText());
        assertEquals("Nomenclature", primaryTabs.get(1).getText());
        assertEquals("Specimens", primaryTabs.get(2).getText());

        assertEquals("Content", p.getTableOfContentHeader());
        List<LinkElement> tocLinks = p.getTableOfContentLinks();
        assertNotNull("Expecting a list of TOC links in the profile page.", tocLinks);

        p.testTableOfContentEntry(0, "Citations", "citation");
        p.testTableOfContentEntry(1, "Distribution", "distribution");
        p.testTableOfContentEntry(2, "Occurrence", "occurrence");

        /*  no longer compatible with test framework in Drupal 7 branch ignoring
         * reason_ change of block id naming convention
         *
        FeatureBlock featureBlock = p.getFeatureBlockAt(0, "citation", "ul", "li");
        assertEquals("expecting no footnote keys", 0, featureBlock.getFootNoteKeys().size());

        List<WebElement> listElements = featureBlock.getElement().findElements(By.tagName("li"));
        assertEquals("Expecting 48 listElements tags in \"Citations\"", 48, listElements.size());

        // ---
        assertEquals("Argyrites speciosa (Meigen, 1830): Croatia", listElements.get(0).getText());
        List<WebElement> anchorTags = listElements.get(0).findElements(By.tagName("a"));
        assertEquals("Expecting one link", 1, anchorTags.size());
        assertTrue(anchorTags.get(0).getAttribute("href").contains("?q=cdm_dataportal/name/"));

        // ---
        assertEquals("Milichia speciosa Meigen, 1830: type information (Becker 1902: 314)", listElements.get(2).getText());
        anchorTags = listElements.get(2).findElements(By.tagName("a"));
        assertEquals("Expecting two links", 2, anchorTags.size());
        assertEquals("Milichia speciosa Meigen, 1830", anchorTags.get(0).getText());
        assertTrue(anchorTags.get(0).getAttribute("href").contains("?q=cdm_dataportal/name/"));
        assertEquals("Becker 1902", anchorTags.get(1).getText());
        assertTrue(anchorTags.get(1).getAttribute("href").contains("?q=cdm_dataportal/reference/"));

        // ---
        assertEquals("Milichia speciosa Meigen, 1830: checklist, Italy (Canzoneri, Gorodkov, Krivosheina, Munari, Nartshuk, Papp & Süss 1995: 25)", listElements.get(9).getText());
        anchorTags = listElements.get(9).findElements(By.tagName("a"));
        assertEquals("Expecting two links", 2, anchorTags.size());
        assertEquals("Milichia speciosa Meigen, 1830", anchorTags.get(0).getText());
        assertTrue(anchorTags.get(0).getAttribute("href").contains("?q=cdm_dataportal/name/"));
        assertEquals("Canzoneri, Gorodkov, Krivosheina, Munari, Nartshuk, Papp & Süss 1995", anchorTags.get(1).getText());
        assertTrue(anchorTags.get(1).getAttribute("href").contains("?q=cdm_dataportal/reference/"));
         */
    }

}
