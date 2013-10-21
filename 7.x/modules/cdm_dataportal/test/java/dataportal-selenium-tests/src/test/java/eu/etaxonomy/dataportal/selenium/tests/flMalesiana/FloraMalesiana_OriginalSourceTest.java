// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.flMalesiana;

import static org.junit.Assert.assertEquals;

import java.io.File;
import java.util.List;
import java.util.UUID;

import org.apache.commons.io.FileUtils;
import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.openqa.selenium.OutputType;
import org.openqa.selenium.TakesScreenshot;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.elements.TaxonListElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.GenericPortalPage;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;
import eu.etaxonomy.dataportal.pages.TaxonSearchResultPage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.floramalesiana})
public class FloraMalesiana_OriginalSourceTest extends CdmDataPortalTestBase{

    private static final UUID UUID_ILLICIUM = UUID.fromString("502e28ca-23d0-44a8-9c13-85fb3d076bff");

    private GenericPortalPage homePage;

    @Before
    public void setUp() throws Exception {

        driver.get(getContext().getBaseUri().toString());
        homePage = new GenericPortalPage(driver, getContext());
    }

    @After
    public void tearDown(){
        logger.debug("@After");
    }


    @Test
    @Ignore // see #3788 (sort order of search results broken in free-text taxon search)
    public void Illicium() throws Exception {

        TaxonSearchResultPage searchResultPage = homePage.submitQuery("Illicium");

        assertEquals(getContext().prepareTitle("Search results"), searchResultPage.getTitle());

        logger.debug("getting first result entry");
        TaxonListElement entryIillicium = searchResultPage.getResultItem(1);

        logger.debug("checking FullTaxonName of first entry: " + entryIillicium.getElement().toString());
        assertEquals("Illicium L. in Syst. Nat. ed. 10: 1050. 1759", entryIillicium.getFullTaxonName());

        logger.debug("clicking TaxonName" + entryIillicium.getElement().toString());
        TaxonProfilePage taxonProfileIillicium = searchResultPage.clickTaxonName(entryIillicium, TaxonProfilePage.class, UUID_ILLICIUM);

//        assertNull("Authorship information should be hidden", taxonProfileIillicium.getAuthorInformationText());  // FF WebDriver hangs here

        List<LinkElement> primaryTabs = taxonProfileIillicium.getPrimaryTabs();

        // due to some obscure "magic" there are sometimes 5 tabs when the tests are run
        // headless, thus we take a screenshot:
        File scrFile = ((TakesScreenshot)driver).getScreenshotAs(OutputType.FILE);
        File destFile = new File(System.getProperty("java.io.tmpdir") + File.separator + "Illicium-tabs.png");
        FileUtils.copyFile(scrFile, destFile);
        logger.info("Screenshot taken and saved as " + destFile.getAbsolutePath());

        assertEquals("Expecting 4 tabs", 4, primaryTabs.size());
        assertEquals("General\n(active tab)", primaryTabs.get(0).getText());
        assertEquals("Synonymy", primaryTabs.get(1).getText());
        assertEquals("Images", primaryTabs.get(2).getText());
        assertEquals("Keys", primaryTabs.get(3).getText());

    }

}
