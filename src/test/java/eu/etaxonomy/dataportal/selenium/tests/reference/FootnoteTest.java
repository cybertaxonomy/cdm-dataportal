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

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.elements.RegistrationItemFull;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.NamePage;
import eu.etaxonomy.dataportal.pages.RegistrationPage;

/**
 * @author a.kohlbecker
 * @since Feb 5, 2019
 *
 */
@DataPortalContexts( { DataPortalSite.reference })
public class FootnoteTest extends CdmDataPortalTestBase {


    private static final String nodosilinea_radiophila_regid = "http://testbank.org/100004";

    private static final String ramsaria_regid = "http://testbank.org/100005";

    private static final UUID nodosilinea_radiophila_name_UUID = UUID.fromString("e97cc25b-ec11-4bb8-88d7-ab40a023f3fb");

    private static final UUID ramsaria_name_UUID = UUID.fromString("3a6d4bf2-5c89-4525-9e87-0bacac96990b");


    String titleSuffix = " | Integration test reference";



    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getBaseUri().toString());
    }

    @Test
    public void registationPageFieldUnitAnnotation() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), nodosilinea_radiophila_regid);

        assertEquals("Registration Id: http://testbank.org/100004" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);

        List<LinkElement> footnoteKeys = regItem.getFootNoteKeys();
        assertEquals(3, footnoteKeys.size());
        // check footnote keys are in right order
        assertEquals("1", footnoteKeys.get(0).getText());
        assertEquals("2", footnoteKeys.get(1).getText());
        assertEquals("3", footnoteKeys.get(2).getText());

        // check content of footnotes
        List<BaseElement> footnotes = regItem.getFootNotes();
        assertEquals(3, footnotes.size());
        assertEquals("1. editorial note on the fieldunit", footnotes.get(0).getText());
        assertEquals("2. Art. 77.7 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 22",
                footnotes.get(1).getText());
        assertEquals("3. Art. 99.9 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 11",
                footnotes.get(2).getText());
    }

    @Test
    public void namePageFieldUnitAnnotation() throws MalformedURLException{

        NamePage p = new NamePage(driver, getContext(), nodosilinea_radiophila_name_UUID);

        assertEquals("Nodosilinea radiophila Heidari & Hauer in Fottea 18(2): 142. fig. 5C, D. 1 Nov 2018" + titleSuffix, driver.getTitle());
        BaseElement pageContent = p.getDataPortalContent();
        assertNotNull(pageContent);

        List<LinkElement> footnoteKeys = pageContent.getFootNoteKeys();
        assertEquals(4, footnoteKeys.size());
        // check footnote keys are in right order
        assertEquals("1", footnoteKeys.get(0).getText());
        assertEquals("2", footnoteKeys.get(1).getText());
        assertEquals("3", footnoteKeys.get(2).getText());
        assertEquals("4", footnoteKeys.get(3).getText());

        // check content of footnotes
        List<BaseElement> footnotes = pageContent.getFootNotes();
        assertEquals(4, footnotes.size());
        assertEquals("1. Art. 77.7 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 22",
                footnotes.get(0).getText());
        assertEquals("2. Art. 99.9 Turland, N.J., Wiersema, J.H., Barrie, F.R. & al., International Code of Nomenclature for algae, fungi, and plants (Shenzhen Code) adopted by the Nineteenth International Botanical Congress Shenzhen, China, July 2017: 11",
                footnotes.get(1).getText());
        BaseElement footnote3 = footnotes.get(2);
        assertEquals("3. Heidari, F. & Hauer, T. - in Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018 Registration: http://testbank.org/100004, 2019-02-06",
                        footnote3.getText());
        List<LinkElement> linksInFootnote3 = footnote3.getLinksInElement();
        assertEquals(2, linksInFootnote3.size());
        assertTrue(linksInFootnote3.get(0).getUrl().endsWith("/cdm_dataportal/reference/f2e43411-2564-42c7-816e-7e6046adbefa"));
        assertEquals("", linksInFootnote3.get(0).getText());
        assertEquals("http://testbank.org/100004", linksInFootnote3.get(1).getText());
        assertTrue(linksInFootnote3.get(1).getUrl().endsWith("cdm_dataportal/registration/http%253A%252F%252Ftestbank.org%252F100004"));
        assertEquals("4. editorial note on the fieldunit", footnotes.get(3).getText());
    }

    @Test
    public void registationPageTypeNameAnnotation() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), ramsaria_regid);

        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);

        List<LinkElement> footnoteKeys = regItem.getFootNoteKeys();
        assertEquals(1, footnoteKeys.size());
        // check footnote keys are in right order
        assertEquals("1", footnoteKeys.get(0).getText());

        // check content of footnotes
        List<BaseElement> footnotes = regItem.getFootNotes();
        assertEquals(1, footnotes.size());
        assertEquals("1. editorial note on the type name of Ramsaria", footnotes.get(0).getText());

    }

    @Test
    public void namePageTypeNameAnnotation() throws MalformedURLException{

        NamePage p = new NamePage(driver, getContext(), ramsaria_name_UUID);

        assertEquals("Ramsaria Heidari & Hauer in Fottea 18(2): 146. 1 Nov 2018" + titleSuffix, driver.getTitle());
        BaseElement pageContent = p.getDataPortalContent();
        assertNotNull(pageContent);

        List<LinkElement> footnoteKeys = pageContent.getFootNoteKeys();
        assertEquals(2, footnoteKeys.size());
        // check footnote keys are in right order
        assertEquals("1", footnoteKeys.get(0).getText());
        assertEquals("2", footnoteKeys.get(1).getText());

        // check content of footnotes
        List<BaseElement> footnotes = pageContent.getFootNotes();
        assertEquals(2, footnotes.size());
        BaseElement footnote1 = footnotes.get(1);
        assertEquals("1. editorial note on the type name of Ramsaria", footnotes.get(0).getText());
        assertEquals("2. Species solaris", footnote1.getText());
        List<LinkElement> linksInFootnote3 = footnote1.getLinksInElement();
        assertEquals(1, linksInFootnote3.size());
        assertTrue(linksInFootnote3.get(0).getUrl().endsWith("cdm_dataportal/reference/5e5d9d08-8c28-4b22-b30a-6214c8641163"));
    }

}
