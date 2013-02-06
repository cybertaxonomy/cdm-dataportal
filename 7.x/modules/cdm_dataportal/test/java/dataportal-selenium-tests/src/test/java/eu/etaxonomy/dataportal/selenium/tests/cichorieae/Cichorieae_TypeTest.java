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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.TypeDesignationElement;
import eu.etaxonomy.dataportal.elements.TypeDesignationType;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * tests to avoid regression of #2306 (holotype is only displayed as type) and related bugs
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cichorieae })
public class Cichorieae_TypeTest extends CdmDataPortalTestBase{


    static UUID cichorium_uuid = UUID.fromString("21d7161a-455e-4f4d-9d61-7f0100c38ff3");

    static UUID scorzonera_tuzgoluensis_Uuid = UUID.fromString("296b4758-048a-47bb-a394-affca64dfc40");


    @Test
    public void cichorium() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), cichorium_uuid);
        assertEquals(getContext().prepareTitle("Cichorium"), p.getTitle());
        assertEquals("Cichorium L., Sp. Pl.: 813. 1753", p.getAcceptedNameText());
        List<TypeDesignationElement> typeDesignations = p.getAcceptedNameTypeDesignations();
        assertEquals("Expecting one Typedesignation", 1, typeDesignations.size());
        assertEquals(TypeDesignationType.nameTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
        assertEquals("Lectotype (designated by Green 1929: 1781): Cichorium intybus L.\n1. Green, Proposals by British Botanists. 1929", typeDesignations.get(0).getText());
    }

    @Test
    public void scorzonera_tuzgoluensis() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), scorzonera_tuzgoluensis_Uuid);
        assertEquals(getContext().prepareTitle("Scorzonera tuzgoluensis"), p.getTitle());
        List<TypeDesignationElement> typeDesignations = p.getAcceptedNameTypeDesignations();
        assertEquals("Expecting one Typedesignation", 1, typeDesignations.size());
        assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
        assertEquals("Holotype: Turkey, B4 Konya, Cihanbeyli, between Gölyazı-Tuzgölü, alt. 908 m, 38°32'33.12\"N, 33°21'11.28\"E, A. Duran, B. Doğan & S. Makbul (KNYA).", typeDesignations.get(0).getText());
    }

}
