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

import org.junit.Before;
import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.RegistrationItemFull;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.RegistrationPage;

/**
 * @author a.kohlbecker
 * @since Feb 5, 2019
 *
 */
@DataPortalContexts( { DataPortalSite.reference })
public class NameRelationshipsTest extends CdmDataPortalTestBase {

    private static final String reg_nodosilinea_sensensia_id = "http://testbank.org/100009";

    String titleSuffix = " | Integration test reference";

    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getBaseUri().toString());
    }


    @Test
    public void testNodosilinea_sensensia_RegPage() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), reg_nodosilinea_sensensia_id);

        assertEquals("Registration Id: http://testbank.org/100009" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Nodosilinea sensensia (Blanco) Heidari & Hauer ex Lem, Nonsens species of the developers Vol1",
                regItem.getNameElement().getText());
        assertEquals(
                "Lem, Nonsens species of the developers Vol1",
                regItem.getCitation().getText());

        List<BaseElement> nameRelationshipElements = regItem.getNameRelationsipsElements();
        assertEquals(5,  nameRelationshipElements.size());
        assertEquals("is new combination for Nepenthes alata Blanco, Fl. Filip., ed. 1: 805. 1837", nameRelationshipElements.get(0).getText());
        assertEquals("is new name for Nepenthes blancoi Blume in Mus. Bot. Lugd.-Bat. 2: 10. 1852", nameRelationshipElements.get(1).getText());
        assertEquals("is validating Nodosilinea radiophila Heidari & Hauer in Fottea 18(2): 142. fig. 5C, D. 1.11.20181", nameRelationshipElements.get(2).getText());
        assertEquals("non Nodosilinea blockensis, New Species in the solar system nec Nodosilinea sensensia, Plantas vasculares de Oz nec Nodosilinea sensensia, Species solaris", nameRelationshipElements.get(3).getText());
        assertEquals("has orthographic variant Nodosilinea sensensi2, 3", nameRelationshipElements.get(4).getText());

        List<BaseElement> nameRelationshipFootnotes = regItem.getNameRelationsipFootnotes();
        assertEquals(3, nameRelationshipFootnotes.size());
        assertEquals("1. Art.99.9 Turland, Wiersema, Barrie, Greuter, D.Hawksw., Herend., S.Knapp, Kusber, D.Z.Li, Marhold, T.W.May, McNeill, A.M.Monro, J.Prado, M.J.Price & Gideon F.Sm., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017:22", nameRelationshipFootnotes .get(0).getText());
        assertEquals("2. Art. 88.9 Turland, Wiersema, Barrie, Greuter, D.Hawksw., Herend., S.Knapp, Kusber, D.Z.Li, Marhold, T.W.May, McNeill, A.M.Monro, J.Prado, M.J.Price & Gideon F.Sm., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017:33", nameRelationshipFootnotes .get(1).getText());
        assertEquals("3. Lem, Nonsens species of the developers Vol1", nameRelationshipFootnotes .get(2).getText());

    }

}
