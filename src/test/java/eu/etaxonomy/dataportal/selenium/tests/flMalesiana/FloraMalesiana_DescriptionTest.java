/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.flMalesiana;

import java.util.List;
import java.util.UUID;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 */
@DataPortalContexts( { DataPortalSite.floramalesiana})
public class FloraMalesiana_DescriptionTest extends CdmDataPortalTestBase{

    private static final Logger logger = LogManager.getLogger();

    private static final UUID tristiropsis_acutangula_uuid = UUID.fromString("87e76e43-e4b7-44a1-a195-2c36a63b34bb");

    @Before
    public void setUp() throws Exception {

        driver.get(getContext().getSiteUri().toString());
    }

    @After
    public void tearDown(){
        logger.debug("@After");
    }

    @Test
    public void tristiropsis_acutangula() throws Exception {

        TaxonProfilePage p =  new TaxonProfilePage(driver, getContext(), tristiropsis_acutangula_uuid);

        List<LinkElement> primaryTabs = p.getPrimaryTabs();

        assertEquals("Expecting 4 tabs", 4, primaryTabs.size());
        assertEquals("General\n(active tab)", primaryTabs.get(0).getText());
        assertEquals("Synonymy", primaryTabs.get(1).getText());
        assertEquals("Images", primaryTabs.get(2).getText());
        assertEquals("Specimens", primaryTabs.get(3).getText());

        FeatureBlock descriptionBlock = p.getFeatureBlockAt(0, "description", "div", "div");
        assertNotNull(descriptionBlock);
        List<WebElement> featureBlockElements = descriptionBlock.getFeatureBlockElements();
//annotations of nested factual data were not shown, now they are handled like the annotations of all other factual data
        assertEquals("Tree, up to 35(-53) m, dbh up to at least 6 cm, often with buttresses up to 3 m high, 4 m wide, and 3 cm thick.1", featureBlockElements.get(0).getText());
        assertEquals("Branchlets 4-10 mm thick, fulvous -tomentose, glabrescent and then shiny purple-brown, older parts more or less pustular lenti-cellate.2", featureBlockElements.get(1).getText());
        assertEquals("Leaves up to 2 m long;3", featureBlockElements.get(2).getText());
        assertEquals("Sepals cream to greenish, persistent and black in fruit,4", featureBlockElements.get(3).getText());
        assertEquals("Petals cuneate at base, broad-elliptic to broad-ovate, 2.5-3.5 b) 2.2-2.5 mm, creamy-white, margin below the insertion of the scale long-ciliate, furthermore sparsely ciliate, apex crenulate, inside glabrous;5", featureBlockElements.get(4).getText());
        assertEquals("Stamens:6", featureBlockElements.get(5).getText());
        assertEquals("Fruits ellipsoid to subglobular, widest about or above the middle, narrowed to short-stipitate at base, 3-4-angular to 3-4-ribbed in cross section, 20-30 by 14-25 mm, yellowish green to dark-yellow when ripe, patently short-hairy inside, often sterile but well developed.7",featureBlockElements.get(6).getText());
    }
}