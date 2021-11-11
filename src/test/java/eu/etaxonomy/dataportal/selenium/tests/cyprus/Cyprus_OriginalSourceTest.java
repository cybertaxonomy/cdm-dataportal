/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cyprus;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.cyprus })
public class Cyprus_OriginalSourceTest extends CdmDataPortalTestBase{

    // Taxon Cistus creticus subsp. creticus
    static UUID taxonUuid = UUID.fromString("2e58b1ab-03a9-4693-bcec-3b8e7f04b572");

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

        assertEquals(getContext().prepareTitle("Cistus creticus subsp. creticus"), p.getTitle());
        assertNull("Authorship information should be hidden", p.getAuthorInformationText());

        List<LinkElement> primaryTabs = p.getPrimaryTabs();
        int tabIndex = 0;
        assertEquals("General\n(active tab)", primaryTabs.get(tabIndex++).getText());
        assertEquals("Synonymy", primaryTabs.get(tabIndex++).getText());
        assertEquals("Images", primaryTabs.get(tabIndex++).getText());
        assertEquals("Expecting " + tabIndex + " tabs", tabIndex++, primaryTabs.size());

        assertEquals("Content", p.getTableOfContentHeader());
        List<LinkElement> tocLinks = p.getTableOfContentLinks();
        assertNotNull("Expecting a list of TOC links in the profile page.", tocLinks);
        int tocIndex = 0;
        p.testTableOfContentEntry(tocIndex++, "Status", "status");
        p.testTableOfContentEntry(tocIndex++, "Endemism", "endemism");
        p.testTableOfContentEntry(tocIndex++, "Systematics", "systematics");
        p.testTableOfContentEntry(tocIndex++, "Chromosome numbers", "chromosome_numbers");
        p.testTableOfContentEntry(tocIndex++, "Distribution", "distribution");

        FeatureBlock featureBlock = p.getFeatureBlockAt(2, "chromosome-numbers", "ul", "li");
        assertEquals("Chromosome numbers", featureBlock.getHeaderText());

        // see  #2288 (Cyprus: ChromosomeNumbers: formating nameInSource)
        assertEquals("Chromosome numbers\n2n = 18 (B. Slavík, Jarolímová, V. & Chrtek, J. 1993: Chromosome counts of some plants from Cyprus. – Candollea 48: 221-230 (as Cistus creticus L.)) (B. Slavík, Jarolímová, V. & Chrtek, J. 2002: Chromosome counts of some plants from Cyprus. 2. – Acta Univ. Carol., Biol. 46: 295-302 (as Cistus creticus L.))", featureBlock.getText());

        assertFalse("expecting no footnote keys", featureBlock.hasFootNoteKeys());
        List<WebElement> linksInFeatureBlock = featureBlock.getElement().findElements(By.tagName("a"));
        assertEquals("Expecting 3 anchor tags in \"Chromosome Numbers\"", 5, linksInFeatureBlock.size());
        assertEquals("chromosome_numbers", linksInFeatureBlock.get(0).getAttribute("name"));
        assertTrue(linksInFeatureBlock.get(1).getAttribute("href").endsWith("cdm_dataportal/reference/863b9b1b-6c2a-4066-af90-ea9a3775598c"));
        assertTrue(linksInFeatureBlock.get(2).getAttribute("href").contains("cdm_dataportal/name/a1dfcc80-2121-46bb-b8b2-c267a9e0725b"));
        assertTrue(linksInFeatureBlock.get(3).getAttribute("href").endsWith("cdm_dataportal/reference/07a97be7-b3fa-4f76-838d-ac7e1e6e9d70"));
        assertTrue(linksInFeatureBlock.get(4).getAttribute("href").contains("cdm_dataportal/name/a1dfcc80-2121-46bb-b8b2-c267a9e0725b"));
    }

}
