/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.selenium.tests.reference;

import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.cdm.common.UTF8;
import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.RegistrationItemFull;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.RegistrationPage;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * @author a.kohlbecker
 * @since Feb 5, 2019
 *
 */
@DataPortalContexts( { DataPortalSite.reference })
public class NameRelationshipsTest extends CdmDataPortalTestBase {

    private static final String reg_nodosilinea_sensensia_id = "http://testbank.org/100009";

    private static final UUID taxon_nodosilinea_sensensia_uuid = UUID.fromString("7094ea13-2a95-46d9-bfca-8c0e0848e44c");

    private static final UUID taxon_bulbostylis_pauciflora_uuid = UUID.fromString("27f2ad59-0e11-44f4-a931-c69053260321");

    private static final UUID taxon_nepenthes_gracilis_uuid = UUID.fromString("5d9af5a8-c8ad-45e8-85df-ce6a01011fb9");

    String titleSuffix = " | Integration test reference";

    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }


    @Test
    public void testNodosilinea_sensensia_RegPage() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), reg_nodosilinea_sensensia_id);

        assertEquals("Registration Id: http://testbank.org/100009" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Nodosilinea sensensia (Blanco) Heidari & Hauer ex Lem, Nonsens species of the developers Vol1. 2001, comb. nov.1,2",
                regItem.getNameElement().getText());
        assertEquals(
                "published in: Lem 2001: Nonsens species of the developers Vol1",
                regItem.getCitation().getText());

        List<BaseElement> nameRelationshipElements = regItem.getNameRelationsipsElements();
        assertEquals(5,  nameRelationshipElements.size());
        assertEquals("is new combination for Nepenthes alata Blanco, Fl. Filip., ed. 1: 805. 1837", nameRelationshipElements.get(0).getText());
        assertEquals("is new name for Nepenthes blancoi Blume in Mus. Bot. Lugd.-Bat. 2: 10. 1852", nameRelationshipElements.get(1).getText());
        assertEquals("is validating3 Nodosilinea radiophila Heidari & Hauer in Fottea 18(2): 142. fig. 5C, D. 1 Nov 2018, nom. illeg.4", nameRelationshipElements.get(2).getText());
        assertEquals("non Nodosilinea blockensis, New Species in the solar system5 nec Nodosilinea sensensia, Plantas vasculares de Oz6 nec Nodosilinea sensensia, Species solaris7", nameRelationshipElements.get(3).getText());
        BaseElement orthVarElement = nameRelationshipElements.get(4);
       // assertEquals("orth. var.8 Nodosilinea sensensi9", orthVarElement.getText()); //has orthographic variant6,7 Nodosilinea sensensi 20018
       assertEquals("orth. var.8 Nodosilinea sensensi9", orthVarElement.getText());
        assertEquals("has orthographic variant", orthVarElement.getElement().findElement(By.className("symbol")).getAttribute("title"));

        List<BaseElement> footnotes = regItem.getRegistrationFootnotes();
        assertEquals(9, footnotes.size());
        assertEquals(
                "1. Art. 99.9; Turland, N.J., Wiersema, J.H., Barrie, F.R., Greuter, W., Hawksworth, D.L., Herendeen, P.S., Knapp, S., Kusber, W.-H., Li, D.-Z., Marhold, K., May, T.W., McNeill, J., Monro, A.M., Prado, J., Price, M.J. & Smith, G.F. (eds.) 2018: International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code), adopted by the Nineteenth International Botanical Congress, Shenzhen, China, July 2017. Regnum Vegetabile 159. – Glashütten: Koeltz Botanical Books",
                footnotes .get(0).getText());
        assertEquals(
                "2. Editorial annotation on Nodosilinea sensensia (Blanco) Heidari & Hauer ex Lem",
                footnotes .get(1).getText());
        assertEquals(
                "3. Art. 77.7",
                footnotes .get(2).getText());
        assertEquals(
                "4. Editorial note on Nodosilinea radiophila",
                footnotes .get(3).getText());
        assertEquals(
                "5. Editorial annotation on Nodosilinea blockensis, New Species in the solar system",
                footnotes .get(4).getText());
        assertEquals(
                "6. Editorial annotation on Nodosilinea sensensia, Plantas vasculares de Oz",
                footnotes .get(5).getText());
        assertEquals(
                "7. Editorial annotation on Nodosilinea sensensia, Species solaris",
                footnotes .get(6).getText());
        assertEquals(
                "8. Art. 88.9",
                footnotes .get(7).getText());
        assertEquals(
                "9. Editorial annotation on Nodosilinea sensensi",
                footnotes .get(8).getText());

    }

    @Test
    public void testNodosilinea_sensensia_SynonymPage() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), taxon_nodosilinea_sensensia_uuid);

        WebElement accName = p.getAcceptedName();
        assertEquals("Nodosilinea sensensia (Blanco) Heidari & Hauer ex Lem, Nonsens species of the developers Vol1. 2001, comb. nov.1,2 [non Nodosilinea sensensia3 "
                + "nec Nodosilinea sensensia4 nec Nodosilinea blockensis5 has orthographic variant6,7 Nodosilinea sensensi 20018]", accName.getText());

        List<BaseElement> footnotes = p.getHomotypicalGroupFootNotes();
        assertEquals(7, footnotes.size());
        assertEquals(
                "1. Art. 99.9; Turland, N.J., Wiersema, J.H., Barrie, F.R., Greuter, W., Hawksworth, D.L., Herendeen, P.S., Knapp, S., Kusber, W.-H., Li, D.-Z., Marhold, K., May, T.W., McNeill, J., Monro, A.M., Prado, J., Price, M.J. & Smith, G.F. (eds.) 2018: International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code), adopted by the Nineteenth International Botanical Congress, Shenzhen, China, July 2017. Regnum Vegetabile 159. – Glashütten: Koeltz Botanical Books",
                footnotes .get(0).getText());
        assertEquals(
                "2. Editorial annotation on Nodosilinea sensensia (Blanco) Heidari & Hauer ex Lem",
                footnotes.get(1).getText());
        assertEquals(
                "3. Editorial annotation on Nodosilinea sensensia, Plantas vasculares de Oz",
                footnotes.get(2).getText());
        assertEquals(
                "4. Editorial annotation on Nodosilinea sensensia, Species solaris",
                footnotes.get(3).getText());
        assertEquals(
                "5. Editorial annotation on Nodosilinea blockensis, New Species in the solar system",
                footnotes.get(4).getText());
        assertEquals(
                "6. Art. 88.9",
                footnotes.get(5).getText());
        assertEquals(
                "7. Editorial annotation on Nodosilinea sensensi",
                footnotes.get(6).getText());

    }

    /**
     * Test for https://dev.e-taxonomy.eu/redmine/issues/5697
     */
    @Test
    public void testIssue5697() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), taxon_bulbostylis_pauciflora_uuid);

        WebElement accName = p.getAcceptedName();
        assertEquals("Bulbostylis pauciflora (Liebm.) C. B. Clarke, nom. cons. [non Bulbostylis pauciflora (Kunth) D.C.]", accName.getText());
        assertEquals("is conserved against", accName.findElement(By.className("symbol")).getAttribute("title"));
    }

    /**
     * Test for https://dev.e-taxonomy.eu/redmine/issues/6523
     */
    @Test
    public void testIssue6523() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), taxon_nepenthes_gracilis_uuid);

        WebElement accName = p.getAcceptedName();
        assertEquals("Nepenthes gracilis Korth., Verh. Nat. Gesch. Ned. Bezitt., Bot. 19: 22, t. 1 & 4. 1840 [& (J. T. K.) Spock & Scotty in Bot. Jahrb. Sol. 211: 591. 21941,2], [some typification note].", accName.getText());

        WebElement synonym1 = p.getHeterotypicalGroupSynonym(1, 1);
        assertEquals("=\nNepenthes teysmanniana Miq., Fl. Ned. Ind. 1(1): 1073. 1858", synonym1.getText());
        WebElement synonym2 = p.getHeterotypicalGroupSynonym(1, 2);
        assertEquals(UTF8.EN_DASH+"\nNepenthes tupmanniana Bonstedt in Parey Blumeng. 1: 663. 1931 [non Nepenthes teysmanniana Miq., Fl. Ned. Ind. 1(1): 1073. 1858]", synonym2.getText());
        assertEquals("is misspelling for", synonym2.findElement(By.className("symbol")).getAttribute("title"));
    }

}
