/**
 * Copyright (C) 2014 EDIT
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

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.GalleryImage;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 *
 */
@DataPortalContexts( { DataPortalContext.cichorieae })
public class FeatureBlock_Media_Test extends CdmDataPortalTestBase {

    static UUID erythroseris_amabilis_Uuid = UUID.fromString("b335ceee-d6c1-4c93-841d-3b4bd279d855");

    @Test
    public void erythroseris_amabilis() throws MalformedURLException {
        TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), erythroseris_amabilis_Uuid);
        String expectedName = "Erythroseris amabilis";
        assertEquals(getContext().prepareTitle(expectedName), driver.getTitle());

        FeatureBlock palynologyBlock = p.getFeatureBlockAt(1, "palynology", "div", "span");

        List<List<GalleryImage>> galleryImageRows = ElementUtils.getGalleryImages(palynologyBlock.getElement(), p.getWait());

        assertEquals("Expecting one row of images", 1, galleryImageRows.size());
        assertEquals("Expecting 3 images in row", 3, galleryImageRows.get(0).size());

        GalleryImage firstImage = galleryImageRows.get(0).get(0);
        assertNotNull("caption should be on", firstImage.getCaptionText());

    }


}
