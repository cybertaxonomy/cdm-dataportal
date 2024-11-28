/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cichorieae;

import java.net.MalformedURLException;
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.cdm.common.UTF8;
import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * @author a.kohlbecker
 */
@DataPortalContexts( { DataPortalSite.cichorieae })
public class Cichorieae_HybridTest extends CdmDataPortalTestBase{

    static UUID crepis_malyi_Uuid = UUID.fromString("a4050699-ace9-45fb-a807-249531da5566");

    static UUID lactuca_favratii_Uuid = UUID.fromString("6027e1fa-9fe5-4ddc-a2de-f72bfa7378c0");

    static UUID crepis_oenipontana_Uuid = UUID.fromString("31b8757f-6acb-4826-ba7f-b2d116dc713c");

    static UUID crepis_artificialis_Uuid = UUID.fromString("3eabdf89-ddeb-461c-b6f8-341bb8deb7bf");

    private static final String hybridWithSpace = UTF8.HYBRID_SPACE.toString();


    @Test
    public void testCrepis_malyi() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepis_malyi_Uuid);
        String expectedName = "Crepis "+hybridWithSpace+"malyi";
        assertEquals(getContext().prepareTitle(expectedName), p.getTitle());
        assertEquals("Crepis "+hybridWithSpace+"malyi Stadlm. in Oesterr. Bot. Z. 58: 425. 1908", p.getAcceptedNameText());
    }

    @Test
    public void testLactuca_favratii() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), lactuca_favratii_Uuid);
        assertEquals(getContext().prepareTitle("Lactuca "+hybridWithSpace+"\"favratii\""), p.getTitle());
        assertEquals("Lactuca "+hybridWithSpace+"\"favratii\", nom. provis.", p.getAcceptedNameText());
        assertEquals("≡ Cicerbita "+hybridWithSpace+"favratii Wilczek in Bull. Soc. Vaud. Sci. Nat. 51: 333. 1917", p.getHomotypicalGroupSynonymName(1));
    }

    @Test
    public void testCrepis_oenipontana() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepis_oenipontana_Uuid);
        assertEquals(getContext().prepareTitle("Crepis "+hybridWithSpace+"oenipontana"), p.getTitle());
        assertEquals("Crepis "+hybridWithSpace+"oenipontana Murr in Österr. Bot. Z. 43: 178. 1893", p.getAcceptedNameText());
        assertEquals("= Crepis alpestris f. pseudalpestris Murr in Allg. Bot. Z. Syst. 14: 9. 1908", p.getHeterotypicalGroupSynonymName(1, 1));
        assertEquals("≡ Crepis alpestris var. pseudalpestris (Murr) Murr in Allg. Bot. Z. Syst. 14: 9. 1908", p.getHeterotypicalGroupSynonymName(1, 2));
        assertEquals("≡ Crepis "+hybridWithSpace+"pseudalpestris (Murr) Murr in Allg. Bot. Z. Syst. 22: 66. 1916", p.getHeterotypicalGroupSynonymName(1, 3));
    }

    @Test
    public void testCrepis_artificialis() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepis_artificialis_Uuid);
        assertEquals(getContext().prepareTitle("Crepis x artificialis"), p.getTitle());
        assertEquals("Incorrectly created hybrid with x should be kept as entered",
                "Crepis x artificialis J. Collins & al. in Genetics 14: 310. 1929", p.getAcceptedNameText());
    }
}