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

import static org.junit.Assert.*;
import static org.junit.Assert.assertEquals;

import java.util.List;

import junit.framework.Assert;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.GalleryImage;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.elements.TaxonListElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.GenericPortalPage;
import eu.etaxonomy.dataportal.pages.PortalPage;
import eu.etaxonomy.dataportal.pages.TaxonSearchResultPage;
import eu.etaxonomy.dataportal.selenium.VisibilityOfElementLocated;

@DataPortalContexts( { DataPortalContext.cichorieae })
public class CichorieaeSearchTest extends CdmDataPortalTestBase {

    private static GenericPortalPage homePage = null;

    @Before
    public void setUp() throws Exception {

        driver.get(getContext().getBaseUri().toString());
        homePage = new GenericPortalPage(driver, getContext());

    }

    @Test
    public void testSearchLCommunis() throws Exception {

        TaxonSearchResultPage searchResultPage = homePage.submitQuery("Lapsana com*");

        assertEquals(getContext().prepareTitle("Search results"), searchResultPage.getTitle());

        TaxonListElement lapsanaCommunnis = searchResultPage.getResultItem(1);

        assertEquals("Lapsana communis L., Sp. Pl.: 811. 1753", lapsanaCommunnis.getFullTaxonName());

        WebElement nameElement = lapsanaCommunnis.getElement().findElement(By.className("BotanicalName"));

        WebElement namePart1 = nameElement.findElement(By.xpath("span[1]"));
        Assert.assertEquals("Lapsana", namePart1.getText());
        Assert.assertEquals("italic", namePart1.getCssValue("font-style"));

        WebElement namePart2 = nameElement.findElement(By.xpath("span[2]"));
        Assert.assertEquals("communis", namePart2.getText());
        Assert.assertEquals("italic", namePart2.getCssValue("font-style"));

        WebElement authorPart = nameElement.findElement(By.xpath("span[3]"));
        Assert.assertEquals("L.", authorPart.getText());
        Assert.assertEquals("normal", authorPart.getCssValue("font-style"));

        WebElement referenceElement = lapsanaCommunnis.getElement().findElement(By.className("reference"));
        Assert.assertEquals("Sp. Pl.: 811. 1753", referenceElement.findElement((By.className("reference"))).getText());

        PortalPage taxonProfilLapsanaCommunnis = searchResultPage.clickTaxonName(lapsanaCommunnis, GenericPortalPage.class);
        assertEquals(getContext().prepareTitle("Lapsana communis"), taxonProfilLapsanaCommunnis.getTitle());
    }

    @Test
    public void testSearchLCommunis_ImageLinks() throws Exception {

        TaxonSearchResultPage searchResultPage = homePage.submitQuery("Lapsana com*");

        assertEquals(getContext().prepareTitle("Search results"), searchResultPage.getTitle());

        TaxonListElement lapsanaCommunnis = searchResultPage.getResultItem(1);

        List<List<GalleryImage>> galleryImageRows = searchResultPage.getGalleryImagesOf(lapsanaCommunnis);

        assertEquals("Expecting one row of images", 1, galleryImageRows.size());
        assertEquals("Expecting 4 images in row", 4, galleryImageRows.get(0).size());

        GalleryImage firstImage = galleryImageRows.get(0).get(0);
        assertNull("caption should be off", firstImage.getCaptionText());
        searchResultPage.clickLink(firstImage.getImageLink(), new VisibilityOfElementLocated(By.id("images")), GenericPortalPage.class);
    }

}
