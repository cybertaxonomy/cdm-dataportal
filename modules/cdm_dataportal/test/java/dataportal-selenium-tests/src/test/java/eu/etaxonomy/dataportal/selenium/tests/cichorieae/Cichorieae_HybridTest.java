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
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cichorieae })
public class Cichorieae_HybridTest extends CdmDataPortalTestBase{


	static UUID crepis_malyi_Uuid = UUID.fromString("a4050699-ace9-45fb-a807-249531da5566");

	static UUID lactuca_favratii_Uuid = UUID.fromString("6027e1fa-9fe5-4ddc-a2de-f72bfa7378c0");

	static UUID crepis_oenipontana_Uuid = UUID.fromString("31b8757f-6acb-4826-ba7f-b2d116dc713c");

	static UUID crepis_artificialis_Uuid = UUID.fromString("3eabdf89-ddeb-461c-b6f8-341bb8deb7bf");


	@Test
	public void crepis_malyi() throws MalformedURLException {
		TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepis_malyi_Uuid);
		String expectedName = "Crepis ×malyi";
		assertEquals(getContext().prepareTitle(expectedName), p.getTitle());
		assertEquals("Crepis ×malyi Stadlm. in Oesterr. Bot. Z. 58: 425. 1908", p.getAcceptedName());
	}

	@Test
	public void lactuca_favratii() throws MalformedURLException {
		TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), lactuca_favratii_Uuid);
		assertEquals(getContext().prepareTitle("Lactuca ×\"favratii\""), p.getTitle());
	    assertEquals("Lactuca ×\"favratii\" , nom. provis.", p.getAcceptedName());
		assertEquals("≡ Cicerbita ×favratii Wilczek in Bull. Soc. Vaud. Sci. Nat. 51: 333. 1917", p.getHomotypicalSynonymName(1));
	}

	@Test
	public void crepis_oenipontana() throws MalformedURLException {
		TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepis_oenipontana_Uuid);
		assertEquals(getContext().prepareTitle("Crepis ×oenipontana"), p.getTitle());
	    assertEquals("Crepis ×oenipontana Murr in Österr. Bot. Z. 43: 178. 1893", p.getAcceptedName());
		assertEquals("= Crepis alpestris f. pseudalpestris Murr in Allg. Bot. Z. Syst. 14: 9. 1908", p.getHeterotypicalSynonymName(1, 1));
		assertEquals("≡ Crepis alpestris var. pseudalpestris (Murr) Murr in Allg. Bot. Z. Syst. 14: 9. 1908", p.getHeterotypicalSynonymName(1, 2));
		assertEquals("≡ Crepis ×pseudalpestris (Murr) Murr in Allg. Bot. Z. Syst. 22: 66. 1916", p.getHeterotypicalSynonymName(1, 3));
	}

	@Test
	public void crepis_artificialis() throws MalformedURLException {
		TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepis_artificialis_Uuid);
		assertEquals(getContext().prepareTitle("Crepis x artificialis"), p.getTitle());
	    assertEquals("Crepis x artificialis J. Collins & al. in Genetics 14: 310. 1929", p.getAcceptedName());
	}

}
