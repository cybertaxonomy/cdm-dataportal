// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.palmae;

import static org.junit.Assert.*;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.apache.commons.lang.StringUtils;
import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.DescriptionElementRepresentation;
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

@DataPortalContexts( { DataPortalContext.palmae })
public class Calamus_acanthospathus_TaxonProfileTest extends CdmDataPortalTestBase{

    static UUID taxonUuid = UUID.fromString("bb340c78-880e-4dd0-91ff-81788a482b31");

    TaxonProfilePage p = null;

    @Before
    public void setUp() throws MalformedURLException {

        p = new TaxonProfilePage(driver, getContext(), taxonUuid);

    }


    @Test
    public void testTitleAndTabs() {

        assertEquals(getContext().prepareTitle("Calamus acanthospathus Griff., Calcutta J. Nat. Hist. 5: 39. 1845"), p.getTitle());
        assertNull("Authorship information should be hidden", p.getAuthorInformationText());

        List<LinkElement> primaryTabs = p.getPrimaryTabs();
        int tabId = 0;
        assertEquals("General", primaryTabs.get(tabId++).getText());
        assertEquals("Synonymy", primaryTabs.get(tabId++).getText());
        assertEquals("Images", primaryTabs.get(tabId++).getText());
        assertEquals("Specimens", primaryTabs.get(tabId++).getText());
        assertEquals("Expecting " + tabId + " tabs", tabId, primaryTabs.size());

    }

    @Test
    public void testProfileImage() {
        ImgElement profileImage = p.getProfileImage();
        assertNotNull("Expecting profile images to be switched on", profileImage);
        assertTrue("Expoecting image palm_tc_29336_1.jpg", profileImage.getSrcUrl().toString().endsWith("/palm_tc_29336_1.jpg"));
    }


    @Test
    @Ignore /* ignoring until the change of T. Evans et al. 2002 to T. Evans, K. Sengdala, B. Thammavong, O.V. Viengkham and J. Dransfield. 2002 is clarified */
    public void testFeatures() {
        assertEquals("Content", p.getTableOfContentHeader());
        List<LinkElement> links = p.getTableOfContentLinks();
        assertNotNull("Expecting a list of TOC links in the profile page.", links);

        FeatureBlock featureBlock;
        int featureId = 0;

        int descriptionElementFontSize = 11;
        String expectedCssDisplay = "inline";
        String expectedListStyleType = "none";
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* distribution */
        String featureClass = "distribution";
        String featureLabel = "Distribution";
        String blockTextFull = featureLabel + "\n\n\nMap accurate to TDWG level 3 distributions\n\nAssam, China South-Central, China Southeast, East Himalaya, India, Laos, Myanmar, Nepal, Thailand, Tibet (World Checklist of Monocotyledons)\nIndia (North-east), Bhutan, Myanmar, China (Tibet, South-east and South Yunnan), Thailand (North) and Laos (North). (T. Evans et al. 2002)";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "p", "span");

        assertEquals(blockTextFull, featureBlock.getText());
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(2, featureBlock.getOriginalSourcesSections().size());
        assertTrue(LinkElement.testIfLinkElement(featureBlock.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "World Checklist of Monocotyledons", this.getContext().getBaseUri().toString()));
        assertTrue(LinkElement.testIfLinkElement(featureBlock.getOriginalSourcesSections().get(1).getLinksInElement().get(0), "T. Evans et al. 2002", this.getContext().getBaseUri().toString() + "?q=cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));

        assertNotNull("Expecting an OpenLayers map", featureBlock.getElement().findElement(By.id("openlayers_map")));
        assertEquals("Map accurate to TDWG level 3 distributions", featureBlock.getElement().findElement(By.className("distribution_map_caption")).getText());


        /* Biology And Ecology */
        featureClass = "biology_and_ecology";
        featureLabel = "Biology And Ecology";
        blockTextFull = featureLabel + "\nEvergreen forest. In Laos at 1800 m, in Thailand at 1500 - 1700 m, in South Yunnan at 1600 m. (T. Evans et al. 2002)";
        expectedCssDisplay = "list-item";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId++,featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlock.getText());
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlock.getOriginalSourcesSections().size());
        assertTrue(LinkElement.testIfLinkElement(featureBlock.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "T. Evans et al. 2002", getContext().getBaseUri().toString() + "?q=cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));


        /* Conservation */
        featureClass = "conservation";
        featureLabel = "Conservation";
        blockTextFull = featureLabel + "\nOf moderate concern. In Indochina it apparently produces at most one or two additional stems and so probably regenerates poorly after harvesting, putting it at elevated risk even though it is widespread and occurs in high altitude forests, which are less threatened by agriculture and logging. It had declined severely due to harvesting in Sikkim over 100 years ago (Anderson 1869). (T. Evans et al. 2002)";
        expectedCssDisplay = "list-item";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlock.getText());
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlock.getOriginalSourcesSections().size());
        assertTrue(LinkElement.testIfLinkElement(featureBlock.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "T. Evans et al. 2002", getContext().getBaseUri().toString() + "?q=cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));

        /* Common Name */
        featureClass = "common_name";
        featureLabel = "Common Name";
        blockTextFull = featureLabel + "\nwai hom (Lao Loum), blong eur (Khamu), wai hawm (Thailand) (T. Evans et al. 2002)";
        expectedCssDisplay = "list-item";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlock.getText());
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlock.getOriginalSourcesSections().size());
        assertTrue(LinkElement.testIfLinkElement(featureBlock.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "T. Evans et al. 2002", getContext().getBaseUri().toString() + "?q=cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));

        /* Uses */
        featureClass = "uses";
        featureLabel = "Uses";
        blockTextFull = featureLabel + "\nThis species is highly valued for its excellent quality small-diameter cane throughout its range. There are small trial plantations in South Yunnan. (T. Evans et al. 2002)";
        expectedCssDisplay = "list-item";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlock.getText());
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlock.getOriginalSourcesSections().size());
        assertTrue(LinkElement.testIfLinkElement(featureBlock.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "T. Evans et al. 2002", getContext().getBaseUri().toString() + "?q=cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));

        /* Discussion */
        featureClass = "discussion";
        featureLabel = "Discussion";
        String blockTextBegin = featureLabel + "\nBeccari (1908) confidently synonymised C. montanus under C. acanthospathus on the basis of the protologue and some detached fruits at K. They match well but";
        String blockTextEnd = "represent arbitrary divisions in a continuum between more and less robust individuals. There is no doubt about their identity with C. acanthospathus, a conclusion also reached by Wei (1986). (T. Evans et al. 2002)";
        expectedCssDisplay = "list-item";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertTrue(featureBlock.getText().startsWith(blockTextBegin));
        assertTrue(featureBlock.getText().endsWith(blockTextEnd));
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlock.getOriginalSourcesSections().size());
        assertTrue(LinkElement.testIfLinkElement(featureBlock.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "T. Evans et al. 2002", getContext().getBaseUri().toString() + "?q=cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));

        /* Discussion */
        featureClass = "materials_examined";
        featureLabel = "Materials Examined";
        blockTextBegin = featureLabel + "\nINDIA (NORTH-EAST): Sikkim, undated, (fr.), Hooker s.n. E72 (K); Khasia, undated, (pist.), Griffith 503 (K). BHUTAN: Sarbhang Distr., 2.5 km below Getchu on Chirang Road, 12 March 1982, (ster.)";
        blockTextEnd = "A. 5293 (K, BM, BK). LAOS (NORTH): Huaphanh Province, Viengthong Distr., Ban Sakok, Phou Loeuy Noy, 21 June 1999, (fr.), Oulathong OL 231 (FRCL, K). (T. Evans et al. 2002)";
        expectedCssDisplay = "list-item";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId++, featureLabel, featureClass);
        featureBlock = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertTrue(featureBlock.getText().startsWith(blockTextBegin));
        assertTrue(featureBlock.getText().endsWith(blockTextEnd));
        featureBlock.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);
        assertEquals(1, featureBlock.getOriginalSourcesSections().size());
        assertTrue(LinkElement.testIfLinkElement(featureBlock.getOriginalSourcesSections().get(0).getLinksInElement().get(0), "T. Evans et al. 2002", getContext().getBaseUri().toString() + "?q=cdm_dataportal/reference/706c5e5e-1dac-4fb2-b849-8e99ad7d63aa"));

    }

}
