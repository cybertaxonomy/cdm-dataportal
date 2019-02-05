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

import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.openqa.selenium.NoSuchElementException;

import eu.etaxonomy.dataportal.DataPortalSite;
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
@Ignore
public class RegistrationPageTest extends CdmDataPortalTestBase {

    private static final String planothidium_victori_id = "http://testbank.org/100001";

    private static final String planothidium_victori_epitype_id = "http://testbank.org/100002";

    private static final String nodosilinea_id = "http://testbank.org/100003"; // in preparation!

    private static final String nodosilinea_radiophila_id = "http://testbank.org/100004";

    private static final String ramsaria_id = "http://testbank.org/100005";

    private static final String ramsaria_avicennae_id = "http://testbank.org/100006";

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
        assertEquals("Planothidium victori Braidwood, J. & Kilroy, C. in Phytotaxa 64. 2012", regItem.getNameElement().getText());
        assertEquals("Braidwood, J. & Kilroy, C., Small diatoms (Bacillariophyta) in cultures from the Styx River, New Zealand, including descriptions of three new species. in Phytotaxa 64: 11-45. 2012", regItem.getCitation().getText());
        assertEquals("Registration on 2019-02-04 18:01:53", regItem.getMetadata().getText());
    }

    @Test
    public void test100002() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), planothidium_victori_epitype_id);

        assertEquals("Registration Id: http://testbank.org/100002" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals("Epitype: (B 40 0040871).", regItem.getSpecimenTypeDesignations().get(0).getText());
        assertEquals("Jahn, R, Abarca, N, Gemeinholzer, B & al., Planothidium lanceolatum and Planothicium frequentissimum reinvestigated wieht molecular methods and morphology: four new species and the taxonomic importance of the sinus and cavum in Diatom Research 32: 75-107. 2017", regItem.getCitation().getText());
        assertEquals("Registration on 2019-02-05 15:18:16", regItem.getMetadata().getText());
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

        assertEquals("Status message\nA registration with the identifier http://testbank.org/100003 is in preparation", p.getMessages(MessageType.status));
    }


    @Test
    public void test100004() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), nodosilinea_radiophila_id);

        assertEquals("Registration Id: http://testbank.org/100004" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals("Nodosilinea radiophila Heidari, F., Zima, J., Riahi, H. & Hauer, T. in Fottea 18(2): 142. fig. 5C, D. 1.11.2018", regItem.getNameElement().getText());
        assertEquals("Holotype: (CBFS A–83–1).", regItem.getSpecimenTypeDesignations().get(0).getText());
        assertEquals("Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018: 142. fig. 5C, D", regItem.getCitation().getText());
        assertEquals("Registration on 2019-02-05 15:16:08", regItem.getMetadata().getText());
    }

    @Test
    public void test100005() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), ramsaria_id);

        assertEquals("Registration Id: http://testbank.org/100005" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals("Ramsaria Heidari, F. & Hauer, T. in Fottea 18(2): 146. 1.11.2018", regItem.getNameElement().getText());
        assertEquals("Orig. des.: Ramsaria avicennae Heidari, F. & Hauer, T. Heidari, F. & Hauer, T. - in Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018:146", regItem.getNameTypeDesignations().get(0).getText());
        assertEquals("Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018", regItem.getCitation().getText());
        assertEquals("Registration on 2019-02-05 15:16:14", regItem.getMetadata().getText());
    }

    @Test
    public void test100006() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), ramsaria_avicennae_id);

        assertEquals("Registration Id: http://testbank.org/100006" + titleSuffix, driver.getTitle());
        RegistrationItemFull regItem = p.getRegistrationItem();
        assertNotNull(regItem);
        assertEquals("Ramsaria avicennae Heidari, F. & Hauer, T. in Fottea 18(2): 146, fig. 3F, G. 1.11.2018", regItem.getNameElement().getText());
        assertEquals("Holotype: (CBFS A–087–1).", regItem.getSpecimenTypeDesignations().get(0).getText());
        assertEquals("Heidari, F., Zima, J., Riahi, H. & al., New simple trichal cyanobacterial taxa isolated from radioactive thermal springs in Fottea 18(2): 137–149. 2018", regItem.getCitation().getText());
        assertEquals("Registration on 2019-02-05 15:16:23", regItem.getMetadata().getText());
    }




}
