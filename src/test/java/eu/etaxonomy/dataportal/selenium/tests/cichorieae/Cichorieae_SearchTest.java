/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cichorieae;

import java.util.List;
import java.util.UUID;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.ui.WebDriverWait;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.elements.GalleryImage;
import eu.etaxonomy.dataportal.elements.TaxonListElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.GenericPortalPage;
import eu.etaxonomy.dataportal.pages.PortalPage;
import eu.etaxonomy.dataportal.pages.TaxonSearchResultPage;
import eu.etaxonomy.dataportal.selenium.VisibilityOfElementLocated;

@DataPortalContexts( { DataPortalSite.cichorieae })
public class Cichorieae_SearchTest extends CdmDataPortalTestBase {

    private static final UUID UUID_L_COMMUNIS = UUID.fromString("5d65f017-0c23-43e4-888d-9649de50dd45");
    private static GenericPortalPage homePage = null;


    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
        homePage = new GenericPortalPage(driver, getContext());
    }

    // @Test
    public void testSearchLCommunis() throws Exception {

        TaxonSearchResultPage searchResultPage = homePage.submitQuery("Lapsana com*");

        assertEquals(getContext().prepareTitle("Search results"), searchResultPage.getTitle());

        TaxonListElement lapsanaCommunnis = searchResultPage.getResultItem(1);

        assertEquals("Lapsana communis L., Sp. Pl.: 811. 1753", lapsanaCommunnis.getFullTaxonName());

        WebElement nameElement = lapsanaCommunnis.getElement().findElement(By.className("TaxonName"));

        WebElement namePart1 = nameElement.findElement(By.xpath("span[1]"));
        Assert.assertEquals("Lapsana communis", namePart1.getText());
        Assert.assertEquals("italic", namePart1.getCssValue("font-style"));

        WebElement authorPart = nameElement.findElement(By.xpath("span[2]"));
        Assert.assertEquals("L.", authorPart.getText());
        Assert.assertEquals("normal", authorPart.getCssValue("font-style"));

        WebElement referenceElement = lapsanaCommunnis.getElement().findElement(By.className("reference"));
        Assert.assertEquals("Sp. Pl.: 811. 1753", referenceElement.findElement((By.className("reference"))).getText());

        PortalPage taxonProfilLapsanaCommunnis = searchResultPage.clickTaxonName(lapsanaCommunnis, GenericPortalPage.class, UUID_L_COMMUNIS);
        assertEquals(getContext().prepareTitle("Lapsana communis"), taxonProfilLapsanaCommunnis.getTitle());
    }

    @Test
    public void testSearchLCommunis_ImageLinks() throws Exception {

        TaxonSearchResultPage searchResultPage = homePage.submitQuery("Lapsana com*");

        assertEquals(getContext().prepareTitle("Search results"), searchResultPage.getTitle());

        TaxonListElement lapsanaCommunnis = searchResultPage.getResultItem(0);

        WebElement l_communisElement = lapsanaCommunnis.getElement();
        WebDriverWait wait = searchResultPage.getWait();
        List<List<GalleryImage>> galleryImageRows = ElementUtils.getGalleryImages(l_communisElement, wait);

        assertEquals("Expecting one row of images", 1, galleryImageRows.size());
        assertEquals("Expecting 4 images in row", 4, galleryImageRows.get(0).size());

        GalleryImage firstImage = galleryImageRows.get(0).get(0);
        assertNull("caption should be off", firstImage.getCaptionText());
        if(searchResultPage.isZenTheme()) {
            firstImage.getImageLink().getUrl().equals("http://media.bgbm.org/erez/erez?src=EditWP6/photos/Lactuca_triquetra_Bc_01.JPG");
        } else {
            searchResultPage.clickLink(firstImage.getImageLink(), new VisibilityOfElementLocated(By.id("images")), GenericPortalPage.class);
        }
    }

    @Test
        public void testSearchMisappliedNames() throws Exception {

            TaxonSearchResultPage searchResultPage = homePage.submitQuery("Hieracium gombense*");

            assertEquals(getContext().prepareTitle("Search results"), searchResultPage.getTitle());

            TaxonListElement hieracium_gombense = searchResultPage.getResultItem(2);

            //WebElement h_gombenseElement = hieracium_gombense.getElement();
            WebDriverWait wait = searchResultPage.getWait();
            assertEquals("Hieracium gombense", hieracium_gombense.getFullTaxonName());
            assertEquals("Hieracium gombense as misapplied for Hieracium gombense subsp. purkynei (Čelak.) Zahn", hieracium_gombense.getText());
        }
}