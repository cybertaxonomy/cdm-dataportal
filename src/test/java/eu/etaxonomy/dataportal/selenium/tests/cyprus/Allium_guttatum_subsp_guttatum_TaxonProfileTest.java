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

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.interactions.Actions;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.ImgElement;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 */
@DataPortalContexts( { DataPortalSite.cyprus })
public class Allium_guttatum_subsp_guttatum_TaxonProfileTest extends CdmDataPortalTestBase{

    private static final Logger logger = LogManager.getLogger();

    private static final UUID taxonUuid = UUID.fromString("6d04598b-3852-4038-91c9-13c7581b21a6");

    private TaxonProfilePage page = null;

    @Before
    public void setUp() throws MalformedURLException {

        page = new TaxonProfilePage(driver, getContext(), taxonUuid);

    }

    @After
    public void tearDown(){
        logger.debug("@After");
    }


    @Test
    public void testPage() {

        assertEquals(getContext().prepareTitle("Allium guttatum subsp. guttatum"), page.getTitle());
        assertNull("Authorship information should be hidden", page.getAuthorInformationText());

        List<LinkElement> primaryTabs = page.getPrimaryTabs();
        assertEquals("Expecting 3 tabs", 3, primaryTabs.size());
        assertEquals("General\n(active tab)", primaryTabs.get(0).getText());
        assertEquals("Synonymy", primaryTabs.get(1).getText());
        assertEquals("Images", primaryTabs.get(2).getText());

        ImgElement profileImage = page.getProfileImage();
        assertNotNull("Expecting profile images to be switched on", profileImage);
//		assertEquals("http://media.bgbm.org/erez/erez?src=EditWP6/zypern/photos/Allium_guttatum_guttatum_A1.jpg", profileImage.getUrl().toString());
//		assertEquals(400, profileImage.getDimension().getHeight(), 0.5);
//		assertEquals(250, profileImage.getDimension().getWidth(), 0.5);

        assertEquals("Content", page.getTableOfContentHeader());
        List<LinkElement> links = page.getTableOfContentLinks();
        assertNotNull("Expecting a list of TOC links in the profile page.", links);
        page.testTableOfContentEntry(0, "Status", "status");
        page.testTableOfContentEntry(1, "Endemism", "endemism");
        page.testTableOfContentEntry(2, "Red Data Book category", "red_data_book_category");
        page.testTableOfContentEntry(3, "Systematics", "systematics");
        page.testTableOfContentEntry(4, "Distribution", "distribution");

        FeatureBlock featureBlock;

        featureBlock = page.getFeatureBlockAt(0, "status", "div", "div");
        assertEquals("Status\nIndigenous (IN)", featureBlock.getText());

        featureBlock = page.getFeatureBlockAt(2, "endemism", "div", "div");
        assertEquals("Endemism\nnot endemic", featureBlock.getText());

        featureBlock = page.getFeatureBlockAt(2, "red-data-book-category", "div", "div");
        assertEquals("Red Data Book category\nData deficient (DD)", featureBlock.getText());

        //FIXME
//		featureBlock = p.getFeatureBlockAt(3, "systematics", "div", null);
//		assertEquals("Systematics\nTaxonomy and nomenclature follow Mathew (1996).\nMathew B. 1996: A review of Allium section Allium . - Kew.", featureBlock.getText());

//

        featureBlock = page.getFeatureBlockAt(4, "distribution", "div", "span");

        assertEquals("Distribution\nDivision 2A,B\nA. Hand, R. 2009: Supplementary notes to the flora of Cyprus VI. – Willdenowia 39: 301-325, B. Meikle, R.D. 1985: Flora of Cyprus 2. – Kew: The Bentham-Moxon Trust", featureBlock.getText());
        assertEquals("Distribution", featureBlock.getHeaderText());
        assertEquals("expecting two footnote keys", 2, featureBlock.countFootNoteKeys());

//        ------- prepared for  bibliography ---------
//        FeatureBlock bibliography = p.getFeatureBlockAt(5, "bibliography", "div", "div");
//        List<BaseElement> bibliographyEntries = bibliography.getFootNotes();
//        assertEquals("A. R. D. Meikle, Flora of Cyprus 2. 1985", bibliographyEntries.get(1));
//        assertEquals("B. R. Hand, Supplementary notes to the flora of Cyprus VI. in Willdenowia 39. 2009", bibliographyEntries.get(1));

        LinkElement footNoteKey_1 = featureBlock.getFootNoteKey(0);
        BaseElement footNote_1 = featureBlock.getFootNote(0);
        assertNotNull(footNoteKey_1);
        assertNotNull(footNote_1);
        assertTrue("expecting one footnote 0 to be the footnote for key 0",footNote_1.getText().startsWith(footNoteKey_1.getText()));

        WebElement footNoteKey_1_element = footNoteKey_1.getElement();
        Actions actions = new Actions(driver);
        actions.moveToElement(footNoteKey_1_element).click().perform();

        page.hover(footNoteKey_1_element);
        assertEquals("rgba(255, 255, 0, 1)", footNoteKey_1_element.getCssValue("background-color"));
        assertEquals("rgba(255, 255, 0, 1)", footNote_1.getElement().getCssValue("background-color"));

        assertEquals("A. Hand, R. 2009: Supplementary notes to the flora of Cyprus VI. – Willdenowia 39: 301-325", footNote_1.getText());

        WebElement distributionMapImage = featureBlock.getElement().findElement(By.className("distribution_map"));
//		assertEquals("http://edit.br.fgov.be/edit_wp5/v1.1/rest_gen.php?title=a:indigenous&ad=cyprusdivs:bdcode:a:2&as=z:ffffff,606060,,|y:1874CD,,|a:339966,,0.1,&l=background_gis:y,cyprusdivs:z&ms=500,380&bbox=32,34,35,36&label=1&img=true&legend=1&mlp=3&mc_s=Georgia,15,blue&mc=&recalculate=false", distributionMapImage.getAttribute("src"));

    }

}
