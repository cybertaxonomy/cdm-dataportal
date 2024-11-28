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
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.cdm.common.UTF8;
import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.ClassificationTreeBlock;
import eu.etaxonomy.dataportal.elements.ClassificationTreeElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * @author a.kohlbecker
 */
@DataPortalContexts( { DataPortalSite.cyprus })
public class Cyprus_HybridTest extends CdmDataPortalTestBase{

    private static final UUID orchiserapias_Uuid = UUID.fromString("0aee7eea-84e7-4b61-8cb6-d17313cc9b80");

    private static final UUID epilobium_aschersonianum_Uuid = UUID.fromString("e13ea422-5d45-477b-ade3-8dc84dbc9dbc");

    private static final UUID aegilops_biuncialis_x_geniculata_Uuid = UUID.fromString("88ff0fbb-c0df-46c1-9969-cf318ea97dbb");

    private static final String hybridWithSpace = UTF8.HYBRID_SPACE.toString();

    @Test
    public void testOrchiserapias() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), orchiserapias_Uuid);
        String expectedName = hybridWithSpace + "Orchiserapias";
        assertEquals(getContext().prepareTitle(expectedName), driver.getTitle());
        assertEquals(expectedName, p.getAcceptedNameText());
        assertEquals("≡ Orchis "+UTF8.HYBRID+" Serapias", p.getHomotypicalGroupSynonymName(1));
    }

    @Test
    public void testEpilobium_aschersonianum() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), epilobium_aschersonianum_Uuid);
        assertEquals(getContext().prepareTitle("Epilobium " + hybridWithSpace + "aschersonianum"), driver.getTitle());
        assertEquals("Epilobium " + hybridWithSpace + "aschersonianum Hausskn.", p.getAcceptedNameText());
        assertEquals("≡ Epilobium lanceolatum "+UTF8.HYBRID+" parviflorum", p.getHomotypicalGroupSynonymName(1));
    }

    @Test
    public void testAegilops_biuncialis_x_geniculata() throws MalformedURLException {
        TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), aegilops_biuncialis_x_geniculata_Uuid);
        assertEquals(getContext().prepareTitle("Aegilops biuncialis "+UTF8.HYBRID+" geniculata"), driver.getTitle());
        ClassificationTreeBlock classificationTree = p.getClassificationTree();
        ClassificationTreeElement focusedElement = classificationTree.getFocusedElement();
        assertTrue(classificationTree.isVisibleInViewPort(focusedElement));
        assertEquals("Abbreviated form of name should be used", "A. biuncialis "+UTF8.HYBRID+" geniculata", focusedElement.getTaxonName());
    }
}