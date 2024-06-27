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

import org.junit.Test;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.DrupalVars;
import eu.etaxonomy.dataportal.elements.TypeDesignationElement;
import eu.etaxonomy.dataportal.elements.TypeDesignationType;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * Tests to avoid regression of #2306 (holotype is only displayed as type) and related bugs
 *
 * @author a.kohlbecker
 */
@DataPortalContexts( { DataPortalSite.cichorieae })
public class Cichorieae_TypeTest extends CdmDataPortalTestBase{

    static UUID cichorium_uuid = UUID.fromString("21d7161a-455e-4f4d-9d61-7f0100c38ff3");

    static UUID scorzonera_tuzgoluensis_Uuid = UUID.fromString("296b4758-048a-47bb-a394-affca64dfc40");

    static UUID lactuca_glandulifera_Uuid = UUID.fromString("6ece0be7-ba4a-4363-b103-4e60429988e5");

    static UUID hypochaeris_uuid = UUID.fromString("79d6b29a-7a73-42b6-a024-2ab35fbd60ff");

    static UUID hypochaeris_maculata_uuid = UUID.fromString("90943959-f2ef-4a3a-8744-c8bcd935c8c2");

    @Test
    public void cichorium() throws Exception {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), cichorium_uuid);
        assertEquals(getContext().prepareTitle("Cichorium"), driver.getTitle());
        assertEquals("Cichorium L., Sp. Pl.: 813. 1753", p.getAcceptedNameText());
        WebElement typeDesignationsContainer;
        List<TypeDesignationElement> typeDesignations;
        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
            typeDesignationsContainer = p.getNewHomotypicalGroupTypeDesignations();
            assertEquals("Lectotype (designated by Green 1929: 1781): Cichorium intybus L.", typeDesignationsContainer.getText());
        }else{
            typeDesignations = p.getHomotypicalGroupTypeDesignations();
            assertEquals(TypeDesignationType.nameTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
            assertNotNull("Expecting at least one Typedesignation", typeDesignations);
        }
    }

    @Test
    public void scorzonera_tuzgoluensis() throws Exception {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), scorzonera_tuzgoluensis_Uuid);
        assertEquals(getContext().prepareTitle("Scorzonera tuzgoluensis"), driver.getTitle());
        WebElement typeDesignationsContainer;
        List<TypeDesignationElement> typeDesignations;
        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
            typeDesignationsContainer = p.getNewHomotypicalGroupTypeDesignations();
            assertEquals("Holotype: Turkey, B4 Konya, Cihanbeyli, between Gölyazı-Tuzgölü, alt. 908 m, 38°32'33.12\"N, 33°21'11.28\"E, A. Duran, B. Doğan & S. Makbul (KNYA)", typeDesignationsContainer.getText());
        }else{
            typeDesignations = p.getHomotypicalGroupTypeDesignations();
            assertEquals("Expecting one Typedesignation", 1, typeDesignations.size());
            assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
        }
    }

    @Test
    public void lactuca_glandulifera() throws Exception {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), lactuca_glandulifera_Uuid);
        assertEquals(getContext().prepareTitle("Lactuca glandulifera"), driver.getTitle());
        WebElement typeDesignationsContainer;
        List<TypeDesignationElement> typeDesignations;
        int i = 0;
        String expectedString = "";
        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
            typeDesignationsContainer = p.getNewHeterotypicalGroupTypeDesignations(1);
            expectedString = "Syntype: \"Uganda\", Scott Elliot 7328; Syntype: [Cameroon] \"Bamenda\", Ledermann 1889; " +
                            "Syntype: [Kenya] \"Mt. Aberdare: Ostseite\", 12 Mar 1922, R. E. Fries 2172; " +
                            "Syntype: [Kenya] \"Mt. Kenia: Nordostseite bei Meru\", 17 Feb 1922, R. E. Fries 1677; " +
                            "Syntype: [Malawi] \"Kyimbila\", Stolz 306; " +
                            "Syntype: [Tanzania] \"Karagwe\", Stuhlmann 1660; " +
                            "Syntype: [Tanzania] \"Kilimandscharo\", Volkens 1238";
            assertEquals(expectedString, typeDesignationsContainer.getText());
        }else{
            typeDesignations = p.getHeterotypicalGroupTypeDesignations(1);
            assertEquals("Expecting 7 Typedesignation", 7, typeDesignations.size());
            assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(0).getTypeDesignationType());

            assertEquals("Syntype: [Kenya] \"Mt. Aberdare: Ostseite\", 12 Mar 1922, R. E. Fries 2172", typeDesignations.get(i++).getText());
            assertEquals("Syntype: [Kenya] \"Mt. Kenia: Nordostseite bei Meru\", 17 Feb 1922, R. E. Fries 1677", typeDesignations.get(i++).getText());
            assertEquals("Syntype: [Malawi] \"Kyimbila\", Stolz 306", typeDesignations.get(i++).getText());
            assertEquals("Syntype: [Tanzania] \"Karagwe\", Stuhlmann 1660", typeDesignations.get(i++).getText());
            assertEquals("Syntype: [Tanzania] \"Kilimandscharo\", Volkens 1238", typeDesignations.get(i++).getText());
            assertEquals("Syntype: \"Uganda\", Scott Elliot 7328", typeDesignations.get(i++).getText());
        }

        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
            typeDesignationsContainer = p.getNewHeterotypicalGroupTypeDesignations(2);
            expectedString = "Syntype: [Cameroon] \"Cameroons Mt., 6,000 ft.\", Dunlap 47; Syntype: [Cameroon], Maitland 226; Syntype: [Cameroon], Mildbraed 10814";
            assertEquals(expectedString, typeDesignationsContainer.getText());
        }else{
            typeDesignations = p.getHeterotypicalGroupTypeDesignations(2);
            assertEquals("Expecting 3 Typedesignation", 3, typeDesignations.size());
            assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
            i = 0;
            assertEquals("Syntype: [Cameroon], Maitland 226", typeDesignations.get(i++).getText());
            assertEquals("Syntype: [Cameroon], Mildbraed 10814", typeDesignations.get(i++).getText());
        }
    }

    @Test
    public void hypochaeris_maculata() throws Exception {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), hypochaeris_maculata_uuid);
        assertEquals(getContext().prepareTitle("Hypochaeris maculata"), driver.getTitle());
        assertEquals("Hypochaeris maculata L., Sp. Pl.: 810. 1753", p.getAcceptedNameText());
        WebElement typeDesignationsContainer;
        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
            typeDesignationsContainer = p.getNewHomotypicalGroupTypeDesignations();
            String expectedString = "Type: \"Habitat in Europae frigidioris pratis asperis.\"; Lectotype (designated by Iamonico 2012: ??1): [s. loc.], Herb. Linnaeus, no. 959.1";
            assertEquals(expectedString, typeDesignationsContainer.getText());
        }else{
            List<TypeDesignationElement> typeDesignations = p.getHomotypicalGroupTypeDesignations();
            assertEquals("Expecting two Typedesignation", 2, typeDesignations.size());
            assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
            assertEquals(TypeDesignationType.specimenTypeDesignation, typeDesignations.get(1).getTypeDesignationType());
            assertEquals("Lectotype (designated by Iamonico, D. 2012: ??1): [s. loc.], Herb. Linnaeus, no. 959.1", typeDesignations.get(1).getText());
        }
    }

    @Test
    public void hypochaeris() throws Exception {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), hypochaeris_uuid);
        assertEquals(getContext().prepareTitle("Hypochaeris"), driver.getTitle());
        assertEquals("Hypochaeris L., Sp. Pl.: 810. 17531", p.getAcceptedNameText());
        WebElement typeDesignationsContainer;
        List<TypeDesignationElement> typeDesignations ;
        String expectedString = "";
        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
            typeDesignationsContainer = p.getNewHomotypicalGroupTypeDesignations();
            expectedString = "Lectotype (designated by Green 1929: 1783): Hypochaeris radicata L.";
            assertEquals(expectedString, typeDesignationsContainer.getText());
        }else{
            typeDesignations = p.getHomotypicalGroupTypeDesignations();
            assertEquals("Expecting one Typedesignation", 1, typeDesignations.size());
            assertEquals(TypeDesignationType.nameTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
            assertEquals("Lectotype (designated by Green, M.L.: 1783): Hypochaeris radicata L.", typeDesignations.get(0).getText());
        }

        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
            typeDesignationsContainer = p.getNewHeterotypicalGroupTypeDesignations(2);
            expectedString = "Lectotype (designated by Steudel 1841: 5685): Seriola laevigata L.";
            assertEquals(expectedString, typeDesignationsContainer.getText());
        }else{
            typeDesignations = p.getHeterotypicalGroupTypeDesignations(2);
            assertEquals("Expecting one Typedesignation", 1, typeDesignations.size());
            assertEquals(TypeDesignationType.nameTypeDesignation, typeDesignations.get(0).getTypeDesignationType());

            expectedString = "Lectotype (designated by Steudel 1841: 5686): Seriola laevigata L.";
            assertEquals(expectedString, typeDesignations.get(0).getText());
        }
    }
}