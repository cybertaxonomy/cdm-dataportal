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
import eu.etaxonomy.dataportal.elements.GalleryImage;
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
        driver.get(getContext().getBaseUri().toString());
    }


    @Test
    public void test100001() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), planothidium_victori_id);

        assertEquals("Registration Id: http://testbank.org/100001" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Planothidium victori Novis, Braidwood & Kilroy in Phytotaxa 64. 2012",
                regItem.getNameElement().getText());
        assertEquals(
                "published in: Novis, P. M., Braidwood, J. & Kilroy, C., Small diatoms (Bacillariophyta) in cultures from the Styx River, New Zealand, including descriptions of three new species in Phytotaxa 64: 11-45. 2012",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-06 14:21:52",
                regItem.getMetadata().getText());
    }

    @Test
    public void test100002() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), planothidium_victori_epitype_id);

        assertEquals("Registration Id: http://testbank.org/100002" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Epitype: Berlin, small river Wuhle, 52°31'14.844\"N, 13°34'40.116\"E, Skibbe - collector number D06 (B 40 0040871).",
                regItem.getSpecimenTypeDesignations().get(0).getText());
        assertEquals(
                "for Planothidium victori Novis, Braidwood & Kilroy in Phytotaxa 64. 2012",
                regItem.getTypifiedNameElement().getText());
        assertEquals(
                "published in: Jahn, R., Abarca, N., Gemeinholzer, B. & al., Planothidium lanceolatum and Planothicium frequentissimum reinvestigated wieht molecular methods and morphology: four new species and the taxonomic importance of the sinus and cavum in Diatom Research 32: 75-107. 2017",
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

        assertEquals(
                "Status message\nA registration with the identifier http://testbank.org/100003 is in preparation",
                p.getMessageItems(MessageType.status).get(0));
    }


    @Test
    public void test100004() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), nodosilinea_radiophila_id);

        assertEquals("Registration Id: http://testbank.org/100004" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals(
                "Nodosilinea radiophila Heidari & Hauer in Fottea 18(2): 142. fig. 5C, D. 1 Nov 2018, nom. illeg.",
                regItem.getNameElement().getText());
        assertEquals(
                "Holotype: Iran, Islamic Republic of, Talesh Mahalleh, Ramsar., alt. 20 m, 36°52'58.8\"N, 50°40'58.8\"E (CBFS A–83–1).",
                regItem.getSpecimenTypeDesignations().get(0).getText());
        assertEquals(
                "published in: Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018",
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
                "Orig. des.: Ramsaria avicennae Heidari & Hauer Heidari, F. & Hauer, T. - in Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018:146",
                regItem.getNameTypeDesignations().get(0).getText());
        assertEquals(
                "published in: Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-06 13:54:35",
                regItem.getMetadata().getText());
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
                "Holotype: Iran, Islamic Republic of, Ramsar, alt. 20 m, 36°52'58.8\"N, 50°40'58.8\"E (CBFS A–087–1).",
                regItem.getSpecimenTypeDesignations().get(0).getText());
        assertEquals(
                "published in: Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018",
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
                "for Glenodinium apiculatum Ehrenb., Infusionsthierchen: 258, pl. XXII. 24 Jul 1838–Aug 1838",
                regItem.getTypifiedNameElement().getText());
        assertEquals(
                "published in: Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H., Still curling after all these years: Glenodinium apiculatum Ehrenb. (Peridiniales, Dinophyceae) repeatedly found at its type locality in Berlin (Germany) in Systematics and Biodiversity. 2017",
                regItem.getCitation().getText());
        assertEquals(
                "Registration on 2019-02-12 09:08:29",
                regItem.getMetadata().getText());

        // type designations in defined order
        // 1. Epitype
        assertEquals(
                "Epitype: Germany, Berlin, 52°31'1.2\"N, 13°21'E, 28.3.2016, D047 (CEDiT 2017E68).",
                regItem.getSpecimenTypeDesignations().get(0).getText());

        // 2. Lectotype
        assertTrue(
                regItem.getSpecimenTypeDesignations().get(1).getText().startsWith("Lectotype: Germany, Berlin, 2.4.1835 (BHUPM 671). Naturkundemuseum Berlin (MFN) - Ehrenberg Collection"));
        assertEquals(
                "BHUPM 671",
                regItem.getSpecimenTypeDesignations().get(1).getLinksInElement().get(0).getText());
        assertTrue(
                regItem.getSpecimenTypeDesignations().get(1).getLinksInElement().get(1).getUrl().endsWith("cdm_dataportal/reference/c5d980ff-8766-4322-9acb-7b0a499de707"));
        List<List<GalleryImage>> galleryImages = ElementUtils.getGalleryImages(regItem.getSpecimenTypeDesignations().get(1).getElement(), p.getWait());
        assertEquals("Expecting one row of images", 1, galleryImages.size());
        assertEquals("Expecting 1 image in row", 1, galleryImages.get(0).size());
        assertEquals(
                "http://download.naturkundemuseum-berlin.de/Ehrenberg/Ec%20Drawings/Ec%20draw%20001-999/Ec%20draw%20600-699/ECdraw671.jpg",
                galleryImages.get(0).get(0).getImageLink().getUrl());

        // 3. Isolectotype
        assertEquals(
                "Isolectotype: Germany, Berlin, 52°31'1.2\"N, 13°21'E, 28.3.2016, D047 (M M-0289351). http://herbarium.bgbm.org/object/B400042045",
                regItem.getSpecimenTypeDesignations().get(2).getText());
        assertEquals(
                "M M-0289351",
                regItem.getSpecimenTypeDesignations().get(2).getLinksInElement().get(0).getText());
        assertEquals(
                "http://herbarium.bgbm.org/object/B400042045",
                regItem.getSpecimenTypeDesignations().get(2).getLinksInElement().get(1).getText());


    }




}
