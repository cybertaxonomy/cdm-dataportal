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
                "Nodosilinea sensensia (Blanco) Heidari & Hauer ex Lem, Nonsens species of the developers Vol1. 2001, comb. nov.1",
                regItem.getNameElement().getText());
        assertEquals(
                "published in: Lem, Nonsens species of the developers Vol1. 2001",
                regItem.getCitation().getText());

        List<BaseElement> nameRelationshipElements = regItem.getNameRelationsipsElements();
        assertEquals(5,  nameRelationshipElements.size());
        assertEquals("is new combination for Nepenthes alata Blanco, Fl. Filip., ed. 1: 805. 1837", nameRelationshipElements.get(0).getText());
        assertEquals("is new name for Nepenthes blancoi Blume in Mus. Bot. Lugd.-Bat. 2: 10. 1852", nameRelationshipElements.get(1).getText());
        assertEquals("is validating2 Nodosilinea radiophila Heidari & Hauer in Fottea 18(2): 142. fig. 5C, D. 1 Nov 2018, nom. illeg.", nameRelationshipElements.get(2).getText());
        assertEquals("non Nodosilinea blockensis, New Species in the solar system nec Nodosilinea sensensia, Plantas vasculares de Oz nec Nodosilinea sensensia, Species solaris", nameRelationshipElements.get(3).getText());
        BaseElement orthVarElement = nameRelationshipElements.get(4);
        assertEquals("orth. var.3 Nodosilinea sensensi4", orthVarElement.getText());
        assertEquals("has orthographic variant", orthVarElement.getElement().findElement(By.className("symbol")).getAttribute("title"));

        List<BaseElement> footnotes = regItem.getRegistrationFootnotes();
        assertEquals(4, footnotes.size());
        assertEquals(
                "1. Art. 99.9 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 11",
                footnotes .get(0).getText());
        assertEquals(
                "2. Art. 77.7 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 22",
                footnotes .get(1).getText());
        assertEquals(
                "3. Art. 88.9 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 33",
                footnotes .get(2).getText());
        assertEquals(
                "4. Lem, Nonsens species of the developers Vol1. 2001",
                footnotes .get(3).getText());
    }

    @Test
    public void testNodosilinea_sensensia_SynonymPage() throws MalformedURLException, UnsupportedEncodingException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), taxon_nodosilinea_sensensia_uuid);

        WebElement accName = p.getAcceptedName();
        assertEquals("Nodosilinea sensensia (Blanco) Heidari & Hauer ex Lem, Nonsens species of the developers Vol1. 2001, comb. nov.1 [non Nodosilinea sensensia nec Nodosilinea sensensia nec Nodosilinea blockensis orth. var.2 Nodosilinea sensensi3]", accName.getText());

        List<BaseElement> footnotes = p.getHomotypicalGroupFootNotes();
        assertEquals(3, footnotes.size());
        assertEquals(
                "1. Art. 99.9 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 11",
                footnotes .get(0).getText());
        assertEquals(
                "2. Art. 88.9 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 33",
                footnotes.get(1).getText());
        assertEquals(
                "3. Lem, Nonsens species of the developers Vol1. 2001",
                footnotes.get(2).getText());

    }

    /**
     * Test for https://dev.e-taxonomy.eu/redmine/issues/5697
     *
     * @throws MalformedURLException
     * @throws UnsupportedEncodingException
     */
    @Test
    public void testIssue5697() throws MalformedURLException, UnsupportedEncodingException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), taxon_bulbostylis_pauciflora_uuid);

        WebElement accName = p.getAcceptedName();
        assertEquals("Bulbostylis pauciflora (Liebm.) C. B. Clarke, nom. cons. [non Bulbostylis pauciflora (Kunth) D.C.]", accName.getText());
        assertEquals("is conserved against", accName.findElement(By.className("symbol")).getAttribute("title"));
    }

    /**
     * Test for https://dev.e-taxonomy.eu/redmine/issues/6523
     */
    @Test
    public void testIssue6523() throws MalformedURLException, UnsupportedEncodingException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), taxon_nepenthes_gracilis_uuid);

        WebElement accName = p.getAcceptedName();
        assertEquals("Nepenthes gracilis Korth., Verh. Nat. Gesch. Ned. Bezitt., Bot. 19: 22, t. 1 & 4. 1840", accName.getText());

        WebElement synonym1 = p.getHeterotypicalGroupSynonym(1, 1);
        assertEquals("=\nNepenthes teysmanniana Miq., Fl. Ned. Ind. 1(1): 1073. 1858", synonym1.getText());
        WebElement synonym2 = p.getHeterotypicalGroupSynonym(1, 2);
        assertEquals("≡\nNepenthes tupmanniana Bonstedt in Parey Blumeng. 1: 663. 1931 [non Nepenthes teysmanniana Miq., Fl. Ned. Ind. 1(1): 1073. 1858]", synonym2.getText());
        assertEquals("is misspelling for", synonym2.findElement(By.className("symbol")).getAttribute("title"));
    }

}
