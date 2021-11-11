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

import java.net.MalformedURLException;
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 *
 */
@DataPortalContexts( { DataPortalSite.cichorieae })
public class Chichorieae_Feature_ImageSourcesTest extends CdmDataPortalTestBase {

    static UUID hieracium_bupleuroides_aggr = UUID.fromString("f85a5f90-fc61-4622-939f-ba6e50500b0d");
    static UUID pilosella_anchusoides = UUID.fromString("b384de9c-0a70-48e9-8fcc-1f772a331544");

    @Test
    public void hieracium_bupleuroides_aggr() throws MalformedURLException {
        TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), hieracium_bupleuroides_aggr);
        String expectedName = "Hieracium bupleuroides aggr.";
        assertEquals(getContext().prepareTitle(expectedName), driver.getTitle());

        FeatureBlock imageSourcesBlock = p.getFeatureBlockAt(0, "image-sources", "div", "span");

        assertEquals("Gottschlich 2009: t. 20-21 (specimen photos)A", imageSourcesBlock.getFeatureBlockElements().get(0).getText());
        assertEquals("A. Gottschlich, G. 2009: Die Gattung Hieracium (Compositae) in der Region Abruzzen (Italien). – Stapfia 89 (as Hieracium bupleuroides C. C. Gmel.)", imageSourcesBlock.getFootNote(0).getText());
    }

    @Test
    public void pilosella_anchusoides() throws MalformedURLException {
        TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), pilosella_anchusoides);
        String expectedName = "Pilosella anchusoides";
        assertEquals(getContext().prepareTitle(expectedName), driver.getTitle());

        FeatureBlock imageSourcesBlock = p.getFeatureBlockAt(0, "image-sources", "div", "span");

        assertEquals("Gottschlich 2009: t. 14 (specimen photo)A", imageSourcesBlock.getFeatureBlockElements().get(0).getText());
        assertEquals("A. Gottschlich, G. 2009: Die Gattung Hieracium (Compositae) in der Region Abruzzen (Italien) in Stapfia 89 (as Hieracium anchusoides (Arv.-Touv.) St.-Lag.)", imageSourcesBlock.getFootNote(0).getText());
    }


}
