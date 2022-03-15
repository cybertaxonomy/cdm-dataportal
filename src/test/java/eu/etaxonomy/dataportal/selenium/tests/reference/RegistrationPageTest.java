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
import org.openqa.selenium.NoSuchElementException;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.GalleryImage;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.elements.RegistrationItemFull;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.PortalPage.MessageType;
import eu.etaxonomy.dataportal.pages.RegistrationPage;

/**
 * @author a.kohlbecker
 * @since Feb 5, 2019
 *
 */
@DataPortalContexts( { DataPortalSite.reference })
public class RegistrationPageTest extends CdmDataPortalTestBase {

    private static final String planothidium_victori_id = "http://testbank.org/100001";

    private static final String planothidium_victori_epitype_id = "http://testbank.org/100002";

    private static final String nodosilinea_id = "http://testbank.org/100003"; // in preparation!

    private static final String nodosilinea_radiophila_id = "http://testbank.org/100004";

    private static final String ramsaria_id = "http://testbank.org/100005";

    private static final String ramsaria_avicennae_id = "http://testbank.org/100006";

    private static final String glenodinium_apiculatum_types_id = "http://testbank.org/100008";

    String titleSuffix = " | Integration test reference";


    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }

    @Test
    public void test100001() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), planothidium_victori_id);

        assertEquals("Registration Id: http://testbank.org/100001" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Planothidium victori Novis, Braidwood & Kilroy in Phytotaxa 64. 20121",
                regItem.getNameElement().getText());
        assertEquals(
                "published in: Novis, P. M., Braidwood, J. & Kilroy, C. 2012: Small diatoms (Bacillariophyta) in cultures from the Styx River, New Zealand, including descriptions of three new species. – Phytotaxa 64: 11-45",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-06 14:21:52",
                regItem.getMetadata().getText());
        List<LinkElement> fkeys = regItem.getFootNoteKeys();
        assertEquals("1", fkeys.get(0).getText());
        List<BaseElement> fns = regItem.getFootNotes();
        assertEquals(1, fns.size());
        assertEquals("1. Please check reference detail", fns.get(0).getText());
    }

    @Test
    public void test100002() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), planothidium_victori_epitype_id);

        assertEquals("Registration Id: http://testbank.org/100002" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Epitype: Berlin, small river Wuhle, 52°31'14.844\"N, 13°34'40.116\"E, Skibbe - collector number D06 (B: 40 0040871)1",
                regItem.getSpecimenTypeDesignations().get(0).getText());
        BaseElement typifiedNameElement = new BaseElement(regItem.getTypifiedNameElement());
        assertEquals(
                "for Planothidium victori Novis, Braidwood & Kilroy in Phytotaxa 64. 20122",
                typifiedNameElement.getText());
        assertEquals("2", typifiedNameElement.getFootNoteKeys().get(0).getText());
        assertEquals("2. Please check reference detail", p.getDataPortalContent().getFootNoteForKey(typifiedNameElement.getFootNoteKeys().get(0)).getText());
        assertEquals(
                "published in: Jahn, R., Abarca, N., Gemeinholzer, B., Mora, D., Skibbe, O., Kulikovskiy, M., Gusev, E., Kusber, W.-H. & Zimmermann, J. 2017: Planothidium lanceolatum and Planothicium frequentissimum reinvestigated wieht molecular methods and morphology: four new species and the taxonomic importance of the sinus and cavum. – Diatom Research 32: 75-107",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-06 14:20:51",
                regItem.getMetadata().getText());
    }

    @Test
    public void test100003() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), nodosilinea_id);

        assertEquals("Registration in preparation" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = null;
        try {
            regItem = p.getRegistrationItem();
        } catch(NoSuchElementException e) {/* IGNORE */}
        assertNull(regItem);

        List<String> statusMessageItems = p.getMessageItems(MessageType.status);
        assertEquals(
                "Status message\nA registration with the identifier http://testbank.org/100003 is in preparation",
                statusMessageItems.get(0));
    }


    @Test
    public void test100004() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), nodosilinea_radiophila_id);

        assertEquals("Registration Id: http://testbank.org/100004" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Nodosilinea radiophila Heidari & Hauer in Fottea 18(2): 142. fig. 5C, D. 1 Nov 2018, nom. illeg.1",
                regItem.getNameElement().getText());
        assertEquals(
                "Holotype: Iran, Talesh Mahalleh, Ramsar., alt. 20 m, 36°52'58.8\"N, 50°40'58.8\"E (CBFS: A–83–1)2",
                regItem.getSpecimenTypeDesignations().get(0).getText());
        assertEquals(
                "published in: Heidari, F., Zima, J., Riahi, H. & Hauer, T. 2018: New simple trichal cyanobacterial taxa isolated from radioactive thermal springs. – Fottea 18(2): 137–149",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-06 14:16:45",
                regItem.getMetadata().getText());
    }

    @Test
    public void test100005() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), ramsaria_id);

        assertEquals("Registration Id: http://testbank.org/100005" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Ramsaria Heidari & Hauer in Fottea 18(2): 146. 1 Nov 2018",
                regItem.getNameElement().getText());
        assertEquals(
                "Orig. des.: Ramsaria avicennae Heidari & Hauer designated by Heidari & Hauer 2018: 1461",
                regItem.getNameTypeDesignations().get(0).getText());
        assertEquals(
                "published in: Heidari, F., Zima, J., Riahi, H. & Hauer, T. 2018: New simple trichal cyanobacterial taxa isolated from radioactive thermal springs. – Fottea 18(2): 137–149",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-06 13:54:35",
                regItem.getMetadata().getText());
        List<LinkElement> footnoteKeys = regItem.getFootNoteKeys();
        assertEquals(1,  footnoteKeys.size());
        assertEquals("1", footnoteKeys.get(0).getText());
        List<BaseElement> footnotes = regItem.getFootNotes();
        assertEquals(1, footnotes.size());
        assertEquals("1. editorial note on the type name of Ramsaria", footnotes.get(0).getText());

    }

    @Test
    public void test100006() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), ramsaria_avicennae_id);

        assertEquals("Registration Id: http://testbank.org/100006" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Ramsaria avicennae Heidari & Hauer in Fottea 18(2): 146, fig. 3F, G. 1 Nov 2018",
                regItem.getNameElement().getText());
        assertEquals(
                "Holotype: Iran, Ramsar, alt. 20 m, 36°52'58.8\"N, 50°40'58.8\"E (CBFS: A–087–1)",
                regItem.getSpecimenTypeDesignations().get(0).getText());
        assertEquals(
                "published in: Heidari, F., Zima, J., Riahi, H. & Hauer, T. 2018: New simple trichal cyanobacterial taxa isolated from radioactive thermal springs. – Fottea 18(2): 137–149",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-06 13:54:29",
                regItem.getMetadata().getText());
    }



    @Test
    public void test100008() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(),  glenodinium_apiculatum_types_id);

        assertEquals("Registration Id: http://testbank.org/100008" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);

        assertEquals(
                "for Glenodinium apiculatum Ehrenb., Infusionsthierchen: 258, pl. XXII. 24 Jul–Aug 1838",
                regItem.getTypifiedNameElement().getText());
        assertEquals(
                "published in: Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 2017: Still curling after all these years: Glenodinium apiculatum Ehrenb. (Peridiniales, Dinophyceae) repeatedly found at its type locality in Berlin (Germany). – Systematics and Biodiversity",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-12 09:08:29",
                regItem.getMetadata().getText());

        // type designations in defined order
        // 1. Epitype
        assertEquals(
                "Epitype: Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047; D. Veloper (CEDiT: 2017E68) http://testid.org/2017E68",
                regItem.getSpecimenTypeDesignations().get(0).getText());

        // 2. Lectotype
        assertTrue(
                regItem.getSpecimenTypeDesignations().get(1).getText().startsWith("Lectotype: Germany, Berlin, 2 Apr 1835 (BHUPM: 671) Naturkundemuseum Berlin (MFN) - Ehrenberg Collection"));
        assertEquals(
                "BHUPM: 671",
                regItem.getSpecimenTypeDesignations().get(1).getLinksInElement().get(0).getText());
        assertTrue(
                regItem.getSpecimenTypeDesignations().get(1).getLinksInElement().get(1).getUrl().endsWith("cdm_dataportal/reference/c5d980ff-8766-4322-9acb-7b0a499de707"));
        List<List<GalleryImage>> galleryImages = ElementUtils.getGalleryImages(regItem.getSpecimenTypeDesignations().get(1).getElement(), p.getWait());
        assertEquals("Expecting one row of images", 1, galleryImages.size());
        assertEquals("Expecting 1 image in row", 1, galleryImages.get(0).size());
        assertEquals(
                "https://upload.wikimedia.org/wikipedia/commons/2/25/PIA01466.jpg",
                galleryImages.get(0).get(0).getImageLink().getUrl());

        // 3. Isolectotype
        assertEquals(
                "Isolectotype: Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047 (M: M-0289351) http://herbarium.bgbm.org/object/B400042045",
                regItem.getSpecimenTypeDesignations().get(2).getText());
        assertEquals(
                "M: M-0289351",
                regItem.getSpecimenTypeDesignations().get(2).getLinksInElement().get(0).getText());
        assertEquals(
                "http://herbarium.bgbm.org/object/B400042045",
                regItem.getSpecimenTypeDesignations().get(2).getLinksInElement().get(1).getText());
    }
}