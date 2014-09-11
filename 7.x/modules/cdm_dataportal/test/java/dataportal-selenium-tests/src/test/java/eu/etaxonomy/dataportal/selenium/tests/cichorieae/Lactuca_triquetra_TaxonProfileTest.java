// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cichorieae;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.ImgElement;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.elements.MultipartDescriptionElementRepresentation;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cichorieae })
public class Lactuca_triquetra_TaxonProfileTest extends CdmDataPortalTestBase{

    public static final Logger logger = Logger.getLogger(Lactuca_triquetra_TaxonProfileTest.class);

    static UUID taxonUuid = UUID.fromString("ecb7a76e-694a-4706-b1ab-a2eb334173ff");

    TaxonProfilePage p = null;

    @Before
    public void setUp() throws MalformedURLException {

        if(p != null && driver != null ){
            logger.debug("TaxonProfilePage p:" + p.getPageURL() + ", driver is at " + driver.getCurrentUrl());
        }
        p = new TaxonProfilePage(driver, getContext(), taxonUuid);

    }


    @Test
    public void testTitleAndTabs() {

        assertEquals(getContext().prepareTitle("Lactuca triquetra"), p.getTitle());
        assertNull("Authorship information should be hidden", p.getAuthorInformationText());

        List<LinkElement> primaryTabs = p.getPrimaryTabs();
        int tabId = 0;
        assertEquals("General\n(active tab)", primaryTabs.get(tabId++).getText());
        assertEquals("Synonymy", primaryTabs.get(tabId++).getText());
        assertEquals("Images", primaryTabs.get(tabId++).getText());
        assertEquals("Specimens", primaryTabs.get(tabId++).getText());
        assertEquals("Expecting " + tabId + " tabs", tabId, primaryTabs.size());

    }

    @Test
    public void testProfileImage() {
        ImgElement profileImage = p.getProfileImage();
        assertNotNull("Expecting profile images to be switched on", profileImage);
        assertTrue("Expecting image Lactuca_triquetra_Bc_01.jpg but was " + profileImage.getSrcUrl().toString(), profileImage.getSrcUrl().toString().matches("(?i).*Lactuca_triquetra_Bc_01\\.jpg.*"));
    }


    @Test
    public void testFeatures() {
        assertEquals("Content", p.getTableOfContentHeader());
        List<LinkElement> links = p.getTableOfContentLinks();
        assertNotNull("Expecting a list of TOC links in the profile page.", links);

        FeatureBlock featureBlock;
        int featureId = 0;

        int descriptionElementFontSize = 12;
        String expectedCssDisplay = "inline";
        String expectedListStyleType = "none";
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 0;

        /* Description */
        String featureClass = "description";
        String featureLabel = "Description";
        String blockTextFull = null;
        String blockTextBegin = featureLabel + "\nHerb, perennial, scoparious, 40-80 cm high. Flowering stems erect, triangular, medullary, glaucous-green, soon leafless, strongly branched; branches erect, slender. Cauline leaves few, glabrous.";
        String blockTextEnd = "Pappus white, 6.0-7.0 mm long, persistent, fragile, shortly barbellate or scabridulous.\n\nbased on: Meikle, R.D. 1985: Flora auf Cyprus 2. - Kew (as Prenanthes triquetra).";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId++,featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "div", "span");

        assertTrue(featureBlock.getText().startsWith(blockTextBegin));
        assertTrue(featureBlock.getText().endsWith(blockTextEnd));
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(0, featureBlock.getOriginalSourcesSections().size());

        /* Distribution */
        expectedCssDisplay = "block";
        featureClass = "distribution";
        featureLabel = "Distribution";
        // below some assertions have been adapted to a bug in the library: TODO remove below comments when ticket is reviewed
        //       see  #3475 (DescriptionServiceImpl.getOrderedDistributions loses distributions)
        //
        // original expectation disabled as bug occured:
//        blockTextFull = featureLabel + "\nAsia-Temperate:\nCyprus 1,2; Lebanon-Syria (Lebanon 3,4,5); Palestine (Israel 5,6).\n1. Meikle, R. D., Flora of Cyprus 2. 1985, 2. Osorio-Tafall, B. H. & Serafim, G. M., List of the vascular plants of Cyprus. 1973, 3. Mouterde, P., Nouvelle flore du Liban et de la Syrie. Texte 3. 1978-1984, 4. Boissier, E., Flora Orientalis 3. 1875, 5. Post, G. E. , Flora of Syria, Palestine, and Sinai 2. 1933, 6. Zohary, M. & Feinbrun-Dothan, N., Flora Palaestina 3. 1978)";
        // interim expectation as long bug was not fixed:
//        blockTextFull = featureLabel + "\n\n\nAsia-Temperate:\nCyprus 1,2; Lebanon-Syria (Lebanon 3,4,5); Palestine (Israel 6).\n1. Meikle, R. D., Flora of Cyprus 2. 1985, 2. Osorio-Tafall, B. H. & Serafim, G. M., List of the vascular plants of Cyprus. 1973, 3. Mouterde, P., Nouvelle flore du Liban et de la Syrie. Texte 3. 1978-1984, 4. Boissier, E., Flora Orientalis 3. 1875, 5. Post, G. E. , Flora of Syria, Palestine, and Sinai 2. 1933, 6. Zohary, M. & Feinbrun-Dothan, N., Flora Palaestina 3. 1978";
        // after fixig the bug #3475, more sources are now displayed than ever before:
        // after layout changes there is an additional space: blockTextFull = featureLabel + "\n\nAsia-Temperate:\nCyprus 1,2; Lebanon-Syria (Lebanon 3,4,5); Palestine (Israel 5,6,7).\n1. Meikle, R. D., Flora of Cyprus 2. 1985, 2. Osorio-Tafall, B. H. & Serafim, G. M., List of the vascular plants of Cyprus. 1973, 3. Mouterde, P., Nouvelle flore du Liban et de la Syrie. Texte 3. 1978-1984, 4. Boissier, E., Flora Orientalis 3. 1875, 5. Post, G. E. , Flora of Syria, Palestine, and Sinai 2. 1933, 7. Zohary, M. & Feinbrun-Dothan, N., Flora Palaestina 3. 19786. (N)";
        blockTextFull = featureLabel + "\n\nAsia-Temperate:\nCyprus 1,2; Lebanon-Syria (Lebanon 3,4,5); Palestine (Israel 5,6,7).\n1. Meikle, R. D., Flora of Cyprus 2. 1985, 2. Osorio-Tafall, B. H. & Serafim, G. M., List of the vascular plants of Cyprus. 1973, 3. Mouterde, P., Nouvelle flore du Liban et de la Syrie. Texte 3. 1978-1984, 4. Boissier, E., Flora Orientalis 3. 1875, 5. Post, G. E. , Flora of Syria, Palestine, and Sinai 2. 1933, 7. Zohary, M. & Feinbrun-Dothan, N., Flora Palaestina 3. 1978\n6. (N)";
        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "div", "dt", "dd");

        // TODO  after #4166 (Map legend causes inconsistent spacing below map) is fixed the below trick which relaxes this test can be removed
        String relaxedBlockText = featureBlock.getText().replaceFirst("\n\n\n", "\n\n");
        assertEquals(blockTextFull, relaxedBlockText);

        MultipartDescriptionElementRepresentation descriptionElement = (MultipartDescriptionElementRepresentation) featureBlock.getDescriptionElements().get(0);
        logger.info(descriptionElement.getText());
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(0, featureBlock.getOriginalSourcesSections().size());
//        assertEquals("Expecting 7 FootnoteKeys", 7, featureBlock.getFootNoteKeys().size()); //original version
//        assertEquals("Expecting 6 FootnoteKeys", 6, featureBlock.getFootNoteKeys().size());   //version after bug #3475
        assertEquals("Expecting 8 FootnoteKeys", 8, featureBlock.getFootNoteKeys().size()); // new version #3475 fixed
        assertEquals("Expecting 7 Footnotes", 7, featureBlock.getFootNotes().size());

        assertNotNull("Expecting an OpenLayers map", featureBlock.getElement().findElement(By.id("openlayers_map")));
        WebElement mapCaptionElement = null;
        try {
            mapCaptionElement = featureBlock.getElement().findElement(By.className("distribution_map_caption"));
        } catch (NoSuchElementException e){
            /* IGNORE */
        }
        assertNull(mapCaptionElement);

        /* Uses */
        featureClass = "ecology";
        featureLabel = "Ecology";
        blockTextFull = featureLabel + "\n500 m. On chalky cliffs or in flushes on serpentine.\n\nfrom: Meikle, R. D. 1985: Flora of Cyprus 2. – Kew. (as Prenanthes triquetra)";
        expectedCssDisplay = "inline";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "div", "span");

        assertEquals(blockTextFull, featureBlock.getText());
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(0, featureBlock.getOriginalSourcesSections().size());
        assertEquals("Expecting no FootnoteKeys", 0, featureBlock.getFootNoteKeys().size());
        assertEquals("Expecting no Footnotes", 0, featureBlock.getFootNotes().size());


        /* Common names */
        featureClass = "common-names";
        featureLabel = "Common names";
        expectedCssDisplay = "block";
        // after bug #3475 was fixed the number of footnotes increased by one
        blockTextFull = featureLabel + "\nArabic (Lebanon): سْكَرْيولَة ثُلاثِيَّة الأَرْكان8,9\n8. recommended, 9. Nehmé, M., Dictionnaire Etymologique de la Flore du Liban. 2000 (as Scariola triquetra (Labill.) Soják)";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "div", "li");

        assertEquals(blockTextFull, featureBlock.getText());


        /* Uses */
        featureClass = "credits";
        featureLabel = "Credits";
        blockTextFull = featureLabel + "\nChristodoulou C. S. 2009: Images (1 added).\nMakris C. 2009: Images (1 added).";
        expectedCssDisplay = "block";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "div", "span");

        assertEquals(blockTextFull, featureBlock.getText());
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(0, featureBlock.getOriginalSourcesSections().size());
        assertEquals("Expecting no FootnoteKeys", 0, featureBlock.getFootNoteKeys().size());
        assertEquals("Expecting no Footnotes", 0, featureBlock.getFootNotes().size());

    }

}
