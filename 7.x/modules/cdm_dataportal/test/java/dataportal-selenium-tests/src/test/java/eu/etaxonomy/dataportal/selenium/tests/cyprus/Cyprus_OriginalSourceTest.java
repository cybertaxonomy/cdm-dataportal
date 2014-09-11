// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cyprus;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
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

@DataPortalContexts( { DataPortalContext.cyprus })
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
        assertEquals("Chromosome numbers", featureBlock.getHeader());

        // see  #2288 (Cyprus: ChromosomeNumbers: formating nameInSource)
        assertEquals("Chromosome numbers\n2n = 18 (B. Slavík & V. Jarolímová & J. Chrtek, Chromosome counts of some plants from Cyprus in Candollea 48. 1993 (as Cistus creticus L.)) (B. Slavík & V. Jarolímová & J. Chrtek, Chromosome counts of some plants from Cyprus. 2 in Acta Univ. Carol., Biol. 46. 2002 (as Cistus creticus L.))", featureBlock.getText());

        assertEquals("expecting no footnote keys", 0, featureBlock.getFootNoteKeys().size());
        List<WebElement> linksInFeatureBlock = featureBlock.getElement().findElements(By.tagName("a"));
        assertEquals("Expecting 3 anchor tags in \"Chromosome Numbers\"", 3, linksInFeatureBlock.size());
        assertEquals("chromosome_numbers", linksInFeatureBlock.get(0).getAttribute("name"));
        assertTrue(linksInFeatureBlock.get(1).getAttribute("href").endsWith("?q=cdm_dataportal/reference/863b9b1b-6c2a-4066-af90-ea9a3775598c"));
        assertTrue(linksInFeatureBlock.get(2).getAttribute("href").endsWith("?q=cdm_dataportal/reference/07a97be7-b3fa-4f76-838d-ac7e1e6e9d70"));
    }

}
