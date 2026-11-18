/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cyprus;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Assertions;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import eu.etaxonomy.dataportal.junit.DataPortalTest;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 */
@DataPortalContexts( { DataPortalSite.cyprus })
public class Bassia_indica_TaxonProfileTest extends CdmDataPortalTestBase{

    private static final Logger logger = LogManager.getLogger();

    private static UUID taxonUuid = UUID.fromString("5250a30a-9e6f-4f2f-9663-93127a1a3829");

    private TaxonProfilePage p = null;

    @BeforeEach
    public void setUp() throws MalformedURLException {

        p = new TaxonProfilePage(driver, getContext(), taxonUuid);

    }

    @AfterEach
    public void tearDown(){
        logger.debug("@After");
    }


    @DataPortalTest
    public void testPage() {

        assertEquals(getContext().prepareTitle("Bassia indica"), p.getTitle());
        assertNull(p.getAuthorInformationText(), "Authorship information should be hidden");

        List<LinkElement> primaryTabs = p.getPrimaryTabs();
        assertEquals(2, primaryTabs.size(), "Expecting 2 tabs");
        assertEquals("General\n(active tab)", primaryTabs.get(0).getText());
        assertEquals("Synonymy", primaryTabs.get(1).getText());

        assertEquals("Content", p.getTableOfContentHeader());
        List<LinkElement> links = p.getTableOfContentLinks();
        assertNotNull(links, "Expecting a list of TOC links in the profile page.");
        p.testTableOfContentEntry(0, "Status", "status");
        p.testTableOfContentEntry(1, "Endemism", "endemism");
        p.testTableOfContentEntry(2, "Distribution", "distribution");

        FeatureBlock featureBlock;

        featureBlock = p.getFeatureBlockAt(0, "status", "div", "div");
        assertEquals("Status\nNaturalized invasive (NA)", featureBlock.getText());

        featureBlock = p.getFeatureBlockAt(1, "endemism", "div", "div");
        assertEquals("Endemism\nnot endemic", featureBlock.getText());

        featureBlock = p.getFeatureBlockAt(2, "distribution", "div", "span");

        assertEquals("Distribution\n"
                + "Division 4A\n"
                + "Division 5B\n"
                + "Division 6C\n"
                + "The record for division 5 may refer to division 6.\nA. Chrtek, J. & B. Slavík 2001: Contribution to the flora of Cyprus. 4. – Fl. Medit. 10: 235-259, B. Della, A. & Iatrou, G. 1995: New plant records from Cyprus. – Kew Bull. 50: 387-396, C. Hand, R. 2003: Supplementary notes to the flora of Cyprus III. – Willdenowia 33: 305-325", featureBlock.getText());
        assertEquals("Distribution", featureBlock.getHeaderText());
        assertEquals(3, featureBlock.countFootNoteKeys(), "expecting two footnote keys");

//        ------- prepared for  bibliography ---------
//        FeatureBlock bibliography = p.getFeatureBlockAt(5, "bibliography", "div", "div");
//        List<BaseElement> bibliographyEntries = bibliography.getFootNotes();
//        assertEquals("A. R. D. Meikle, Flora of Cyprus 2. 1985", bibliographyEntries.get(1));
//        assertEquals("B. R. Hand, Supplementary notes to the flora of Cyprus VI. in Willdenowia 39. 2009", bibliographyEntries.get(1));


    }

}
