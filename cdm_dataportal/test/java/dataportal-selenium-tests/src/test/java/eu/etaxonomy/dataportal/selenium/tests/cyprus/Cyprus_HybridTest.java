// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cyprus;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.net.MalformedURLException;
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.ClassificationTreeBlock;
import eu.etaxonomy.dataportal.elements.ClassificationTreeElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cyprus })
public class Cyprus_HybridTest extends CdmDataPortalTestBase{


    static UUID orchiserapias_Uuid = UUID.fromString("0aee7eea-84e7-4b61-8cb6-d17313cc9b80");

    static UUID epilobium_aschersonianum_Uuid = UUID.fromString("e13ea422-5d45-477b-ade3-8dc84dbc9dbc");

    //
    static UUID aegilops_biuncialis_x_geniculata_Uuid = UUID.fromString("88ff0fbb-c0df-46c1-9969-cf318ea97dbb");


    @Test
    public void orchiserapias() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), orchiserapias_Uuid);
        String expectedName = "×Orchiserapias";
        assertEquals(getContext().prepareTitle(expectedName), p.getTitle());
        assertEquals(expectedName, p.getAcceptedNameText());
        assertEquals("≡ Orchis × Serapias", p.getHomotypicalGroupSynonymName(1));
    }

    @Test
    public void epilobium_aschersonianum() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), epilobium_aschersonianum_Uuid);
        assertEquals(getContext().prepareTitle("Epilobium ×aschersonianum"), p.getTitle());
        assertEquals("Epilobium ×aschersonianum Hausskn.", p.getAcceptedNameText());
        assertEquals("≡ Epilobium lanceolatum × parviflorum", p.getHomotypicalGroupSynonymName(1));
    }

    @Test
    public void aegilops_biuncialis_x_geniculata() throws MalformedURLException {
        TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), aegilops_biuncialis_x_geniculata_Uuid);
        assertEquals(getContext().prepareTitle("Aegilops biuncialis × geniculata"), p.getTitle());
        ClassificationTreeBlock classificationTree = p.getClassificationTree();
        ClassificationTreeElement focusedElement = classificationTree.getFocusedElement();
        assertTrue(classificationTree.isVisibleInViewPort(focusedElement));
        assertEquals("Abbreviated form of name should be used", "A. biuncialis × geniculata", focusedElement.getTaxonName());
    }

}
