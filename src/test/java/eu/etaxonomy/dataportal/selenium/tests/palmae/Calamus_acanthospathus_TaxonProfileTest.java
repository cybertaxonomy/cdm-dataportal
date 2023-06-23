/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.palmae;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;

import eu.etaxonomy.dataportal.DataPortalSite;
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

@DataPortalContexts( { DataPortalSite.palmae })
public class Calamus_acanthospathus_TaxonProfileTest extends CdmDataPortalTestBase{

    static UUID taxonUuid = UUID.fromString("bb340c78-880e-4dd0-91ff-81788a482b31");

    static TaxonProfilePage p = null;

    /**
     * NOTE
     *
     * This was formerly formated as '" + evans_et_al_referenceCitation + "' by special citation
     * string generation in the drupal code which need to be removed
     * in order to retain the maintainability of the code.
     */
    String evans_et_al_referenceCitation = "Evans, T., Sengdala, K., Thammavong, B., Viengkham, O.V. & Dransfield, J. 2002:  A Synopsis of the Rattans (Arecaceae: Calamoideae) of Laos and Neighbouring Parts of Indochina. – Kew Bulletin, Vol. 57, No. 1 (2002), pp. 1-84";

    @Before
    public void setUp() throws MalformedURLException {

        if(p == null){
            p = new TaxonProfilePage(driver, getContext(), taxonUuid);
        }

    }


    @Test
    public void testTitleAndTabs() {

        assertEquals(getContext().prepareTitle("Calamus acanthospathus Griff., Calcutta J. Nat. Hist. 5: 39. 1845"), driver.getTitle());
        assertNull("Authorship information should be hidden", p.getAuthorInformationText());

        List<LinkElement> primaryTabs = p.getPrimaryTabs();
        int tabId = 0;
        assertEquals("General\n(active tab)", primaryTabs.get(tabId++).getText());
        assertEquals("Synonymy", primaryTabs.get(tabId++).getText());
        assertEquals("Images", primaryTabs.get(tabId++).getText());
//        assertEquals("Specimens", primaryTabs.get(tabId++).getText()); is disabled by layout settings
        assertEquals("Expecting " + tabId + " tabs", tabId, primaryTabs.size());

    }

    @Test
    public void testProfileImage() {
        ImgElement profileImage = p.getProfileImage();
        assertNotNull("Expecting profile images to be switched on", profileImage);
        assertTrue("Expecting image palm_tc_29336_1.jpg", profileImage.getSrcUrl().toString().endsWith("/palm_tc_29336_1.jpg"));
    }


    @Test
    public void testFeatureToc() {

        assertEquals("Content", p.getTableOfContentHeader());
        List<LinkElement> links = p.getTableOfContentLinks();
        assertNotNull("Expecting a list of TOC links in the profile page.", links);
    }

    @Test
    public void testFeatureDistribution() {

        int featureId = 0;

        int descriptionElementFontSize = 11;
        String expectedListStyleType = "none";
        String expectedCssDisplay = "inline";
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* distribution */
        String featureClass = "distribution";
        String featureLabel = "Distribution";
        String contentTextFull = "Map uses TDWG level 3 distributions (http://www.nhm.ac.uk/hosted_sites/tdwg/geogrphy.html)\nAssam (World Checklist of Arecaceae), China South-Central (World Checklist of Arecaceae), China Southeast (World Checklist of Arecaceae), East Himalaya (World Checklist of Arecaceae), India (World Checklist of Arecaceae), Laos (World Checklist of Arecaceae), Myanmar (World Checklist of Arecaceae), Nepal (World Checklist of Arecaceae), Thailand (World Checklist of Arecaceae), Tibet (World Checklist of Arecaceae)\nIndia (North-east), Bhutan, Myanmar, China (Tibet, South-east and South Yunnan), Thailand (North) and Laos (North). (Evans, T., Sengdala, K., Thammavong, B., Viengkham, O.V. & Dransfield, J. 2002: A Synopsis of the Rattans (Arecaceae: Calamoideae) of Laos and Neighbouring Parts of Indochina)";

        p.testTableOfContentEntry(featureId, featureLabel, featureClass);
        FeatureBlock featureBlockDistribution = p.getFeatureBlockAt(featureId, featureClass, "div", "span");

        assertEquals(featureLabel, featureBlockDistribution.getTitle().getText());
        assertEquals(contentTextFull, featureBlockDistribution.getContentText().trim());

        featureBlockDistribution.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlockDistribution.getOriginalSourcesSections().size());
        // NOTE: the first source has been deleted in latest data
        // assertTrue(LinkElement.testIfLinkElement(featureBlockDistribution.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "World Checklist of Monocotyledons", getContext().getBaseUri().toString()));
        assertEquals(evans_et_al_referenceCitation, featureBlockDistribution.getOriginalSourcesSections().get(0).getElement().findElement(By.className("reference")).getText());
        assertTrue(LinkElement.testIfLinkElement(featureBlockDistribution.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));

        assertNotNull("Expecting an OpenLayers map", featureBlockDistribution.getElement().findElement(By.id("openlayers-map-distribution")));
        assertEquals("Map uses TDWG level 3 distributions (http://www.nhm.ac.uk/hosted_sites/tdwg/geogrphy.html)", featureBlockDistribution.getElement().findElement(By.className("distribution_map_caption")).getText());

    }

    @Test
    public void testFeatureBiologyAndEcology() {

        int featureId = 2;
        int descriptionElementFontSize = 11;
        String expectedCssDisplay = "inline";
        String expectedListStyleType = "none";
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* Biology And Ecology */
        String featureClass = "biology-and-ecology";
        String featureLabel = "Biology And Ecology";
        String blockTextFull = featureLabel + "\nEvergreen forest. In Laos at 1800 m, in Thailand at 1500 - 1700 m, in South Yunnan at 1600 m. (" + evans_et_al_referenceCitation + ")";
        expectedCssDisplay = "list-item";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId,featureLabel, featureClass);
        FeatureBlock featureBlockBioEco = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlockBioEco.getText());
        featureBlockBioEco.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlockBioEco.getOriginalSourcesSections().size());
        assertEquals(evans_et_al_referenceCitation, featureBlockBioEco.getOriginalSourcesSections().get(0).getElement().findElement(By.className("reference")).getText());
        assertTrue(LinkElement.testIfLinkElement(featureBlockBioEco.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));
    }

    @Test
    public void testFeatureConservation() {

        int featureId = 3;
        int descriptionElementFontSize = 11;
        String expectedListStyleType = "none";
        String expectedCssDisplay = "list-item";
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* Conservation */
        String featureClass = "conservation";
        String featureLabel = "Conservation";
        String blockTextFull = featureLabel + "\nOf moderate concern. In Indochina it apparently produces at most one or two additional stems and so probably regenerates poorly after harvesting, putting it at elevated risk even though it is widespread and occurs in high altitude forests, which are less threatened by agriculture and logging. It had declined severely due to harvesting in Sikkim over 100 years ago (Anderson 1869). (" + evans_et_al_referenceCitation + ")";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        FeatureBlock featureBlockConservation = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlockConservation.getText());
        featureBlockConservation.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlockConservation.getOriginalSourcesSections().size());
        assertEquals(evans_et_al_referenceCitation, featureBlockConservation.getOriginalSourcesSections().get(0).getElement().findElement(By.className("reference")).getText());
        assertTrue(LinkElement.testIfLinkElement(featureBlockConservation.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));
    }

    @Test
    public void testFeatureCommonName() {

        int featureId = 4;
        String expectedCssDisplay = "list-item";
        String expectedListStyleType = "none";
        int descriptionElementFontSize = 11;
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* Common Name */
        String featureClass = "common-name";
        String featureLabel = "Common Name";
        String blockTextFull = featureLabel + "\nwai hom (Lao Loum), blong eur (Khamu), wai hawm (Thailand) (" + evans_et_al_referenceCitation + ")";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        FeatureBlock featureBlockCommonName = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlockCommonName.getText());
        featureBlockCommonName.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlockCommonName.getOriginalSourcesSections().size());
        assertEquals(evans_et_al_referenceCitation, featureBlockCommonName.getOriginalSourcesSections().get(0).getElement().findElement(By.className("reference")).getText());
        assertTrue(LinkElement.testIfLinkElement(featureBlockCommonName.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));

    }

    @Test
    public void testFeatureUses() {

        int featureId = 5;
        String expectedCssDisplay = "list-item";
        String expectedListStyleType = "none";
        int descriptionElementFontSize = 11;
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* Uses */
        String featureClass = "uses";
        String featureLabel = "Uses";
        String  blockTextFull = featureLabel + "\nThis species is highly valued for its excellent quality small-diameter cane throughout its range. There are small trial plantations in South Yunnan. (" + evans_et_al_referenceCitation + ")";

        p.testTableOfContentEntry(featureId, featureLabel, featureClass);
        FeatureBlock featureBlockUses = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlockUses.getText());
        featureBlockUses.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlockUses.getOriginalSourcesSections().size());
        assertEquals(evans_et_al_referenceCitation, featureBlockUses.getOriginalSourcesSections().get(0).getElement().findElement(By.className("reference")).getText());
        assertTrue(LinkElement.testIfLinkElement(featureBlockUses.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));
    }

    @Test
    public void testFeatureDiscussion() {

        int featureId = 1;
        String expectedCssDisplay = "list-item";
        String expectedListStyleType = "none";
        int descriptionElementFontSize = 11;
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* Discussion */
        String featureClass = "discussion";
        String featureLabel = "Discussion";
        String blockTextBegin = featureLabel + "\nBeccari (1908) confidently synonymised C. montanus under C. acanthospathus on the basis of the protologue and some detached fruits at K. They match well but";
        String blockTextEnd = "represent arbitrary divisions in a continuum between more and less robust individuals. There is no doubt about their identity with C. acanthospathus, a conclusion also reached by Wei (1986). (" + evans_et_al_referenceCitation + ")";

        p.testTableOfContentEntry(featureId, featureLabel, featureClass);
        FeatureBlock featureBlockDiscussion = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertTrue(featureBlockDiscussion.getText().startsWith(blockTextBegin));
        assertTrue(featureBlockDiscussion.getText().endsWith(blockTextEnd));
        featureBlockDiscussion.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlockDiscussion.getOriginalSourcesSections().size());
        assertEquals(evans_et_al_referenceCitation, featureBlockDiscussion.getOriginalSourcesSections().get(0).getElement().findElement(By.className("reference")).getText());
        assertTrue(LinkElement.testIfLinkElement(featureBlockDiscussion.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));
    }

    @Test
    public void testFeatureMaterialsExamined() {

        int featureId = 6;
        String expectedCssDisplay = "list-item";
        String expectedListStyleType = "none";
        int descriptionElementFontSize = 11;
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* Materials Examined */
        String featureClass = "materials-examined";
        String featureLabel = "Materials Examined";
        String blockTextBegin = featureLabel + "\nINDIA (NORTH-EAST): Sikkim, undated, (fr.), Hooker s.n. E72 (K); Khasia, undated, (pist.), Griffith 503 (K). BHUTAN: Sarbhang Distr., 2.5 km below Getchu on Chirang Road, 12 March 1982, (ster.)";
        String blockTextEnd = "A. 5293 (K, BM, BK). LAOS (NORTH): Huaphanh Province, Viengthong Distr., Ban Sakok, Phou Loeuy Noy, 21 June 1999, (fr.), Oulathong OL 231 (FRCL, K). (" + evans_et_al_referenceCitation + ")";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        FeatureBlock featureBlockMaterials = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertTrue(featureBlockMaterials.getText().startsWith(blockTextBegin));
        assertTrue(featureBlockMaterials.getText().endsWith(blockTextEnd));
        featureBlockMaterials.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlockMaterials.getOriginalSourcesSections().size());
        assertEquals(evans_et_al_referenceCitation, featureBlockMaterials.getOriginalSourcesSections().get(0).getElement().findElement(By.className("reference")).getText());
    }

}
