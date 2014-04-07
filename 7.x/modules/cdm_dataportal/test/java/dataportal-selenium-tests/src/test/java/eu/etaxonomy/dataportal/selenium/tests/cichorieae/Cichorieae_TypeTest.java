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

    static UUID lactuca_glandulifera_Uuid = UUID.fromString("6ece0be7-ba4a-4363-b103-4e60429988e5");


    @Test
    public void cichorium() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), cichorium_uuid);
        assertEquals(getContext().prepareTitle("Cichorium"), driver.getTitle());
        assertEquals("Cichorium L., Sp. Pl.: 813. 1753", p.getAcceptedNameText());
        List<TypeDesignationElement> typeDesignations = p.getHomotypicalGroupTypeDesignations();
        assertEquals("Expecting one Typedesignation", 1, typeDesignations.size());
        assertEquals(TypeDesignationType.nameTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
        assertEquals("Lectotype (designated by Green 1929: 1781): Cichorium intybus L.", typeDesignations.get(0).getText());
    }

    @Test
    public void scorzonera_tuzgoluensis() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), scorzonera_tuzgoluensis_Uuid);
        assertEquals(getContext().prepareTitle("Scorzonera tuzgoluensis"), driver.getTitle());
        List<TypeDesignationElement> typeDesignations = p.getHomotypicalGroupTypeDesignations();
        assertEquals("Expecting one Typedesignation", 1, typeDesignations.size());
        assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
        assertEquals("Holotype: Turkey, B4 Konya, Cihanbeyli, between Gölyazı-Tuzgölü, alt. 908 m, 38°32'33.12\"N, 33°21'11.28\"E, A. Duran, B. Doğan & S. Makbul (KNYA).", typeDesignations.get(0).getText());
    }

    @Test
    public void lactuca_glandulifera() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), lactuca_glandulifera_Uuid);
        assertEquals(getContext().prepareTitle("Lactuca glandulifera"), driver.getTitle());
        List<TypeDesignationElement> typeDesignations = p.getHeterotypicalGroupTypeDesignations(1);
        assertEquals("Expecting 7 Typedesignation", 7, typeDesignations.size());

        assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
        assertEquals("Syntype: [Cameroon] \"Bamenda\", Ledermann 1889", typeDesignations.get(0).getText());
        assertEquals("Syntype: [Tanzania] \"Kilimandscharo\", Volkens 1238", typeDesignations.get(1).getText());
        assertEquals("Syntype: [Malawi] \"Kyimbila\", Stolz 306", typeDesignations.get(2).getText());
        assertEquals("Syntype: [Kenya] \"Mt. Aberdare: Ostseite\", 12 Mar 1922, R. E. Fries 2172", typeDesignations.get(3).getText());
        assertEquals("Syntype: [Kenya] \"Mt. Kenia: Nordostseite bei Meru\", 17 Feb 1922, R. E. Fries 1677", typeDesignations.get(4).getText());
        assertEquals("Syntype: [Tanzania] \"Karagwe\", Stuhlmann 1660", typeDesignations.get(5).getText());
        assertEquals("Syntype: \"Uganda\", Scott Elliot 7328", typeDesignations.get(6).getText());

        typeDesignations = p.getHeterotypicalGroupTypeDesignations(2);
        assertEquals("Expecting 3 Typedesignation", 3, typeDesignations.size());
        assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
        assertEquals("Syntype: [Cameroon], Maitland 226", typeDesignations.get(0).getText());
        assertEquals("Syntype: [Cameroon], Mildbraed 10814", typeDesignations.get(1).getText());
        assertEquals("Syntype: [Cameroon] \"Cameroons Mt., 6,000 ft.\", Dunlap 47", typeDesignations.get(2).getText());


    }

}
