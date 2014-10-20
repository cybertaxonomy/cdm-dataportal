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

import static org.junit.Assert.*;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.ImgElement;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cyprus })
public class Bassia_indica_TaxonProfileTest extends CdmDataPortalTestBase{

    static UUID taxonUuid = UUID.fromString("5250a30a-9e6f-4f2f-9663-93127a1a3829");

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

        assertEquals(getContext().prepareTitle("Bassia indica"), p.getTitle());
        assertNull("Authorship information should be hidden", p.getAuthorInformationText());

        List<LinkElement> primaryTabs = p.getPrimaryTabs();
        assertEquals("Expecting 2 tabs", 2, primaryTabs.size());
        assertEquals("General\n(active tab)", primaryTabs.get(0).getText());
        assertEquals("Synonymy", primaryTabs.get(1).getText());

        assertEquals("Content", p.getTableOfContentHeader());
        List<LinkElement> links = p.getTableOfContentLinks();
        assertNotNull("Expecting a list of TOC links in the profile page.", links);
        p.testTableOfContentEntry(0, "Status", "status");
        p.testTableOfContentEntry(1, "Endemism", "endemism");
        p.testTableOfContentEntry(2, "Distribution", "distribution");

        FeatureBlock featureBlock;

        featureBlock = p.getFeatureBlockAt(0, "status", "div", "div");
        assertEquals("Status\nNaturalized invasive (NA)", featureBlock.getText());

        featureBlock = p.getFeatureBlockAt(1, "endemism", "div", "div");
        assertEquals("Endemism\nnot endemic", featureBlock.getText());

        featureBlock = p.getFeatureBlockAt(2, "distribution", "div", "span");

        assertEquals("Distribution\nDivision 41 Division 52 Division 63\nThe record for division 5 may refer to division 6.\n1. J. Chrtek & B. Slavík, Contribution to the flora of Cyprus. 4. in Fl. Medit. 10. 2001, 2. A. Della & G. Iatrou, New plant records from Cyprus in Kew Bull. 50. 1995, 3. R. Hand, Supplementary notes to the flora of Cyprus III. in Willdenowia 33. 2003", featureBlock.getText());
        assertEquals("Distribution", featureBlock.getHeader());
        assertEquals("expecting two footnote keys", 3, featureBlock.getFootNoteKeys().size());

//        ------- prepared for  bibliography ---------
//        FeatureBlock bibliography = p.getFeatureBlockAt(5, "bibliography", "div", "div");
//        List<BaseElement> bibliographyEntries = bibliography.getFootNotes();
//        assertEquals("A. R. D. Meikle, Flora of Cyprus 2. 1985", bibliographyEntries.get(1));
//        assertEquals("B. R. Hand, Supplementary notes to the flora of Cyprus VI. in Willdenowia 39. 2009", bibliographyEntries.get(1));


    }

}
