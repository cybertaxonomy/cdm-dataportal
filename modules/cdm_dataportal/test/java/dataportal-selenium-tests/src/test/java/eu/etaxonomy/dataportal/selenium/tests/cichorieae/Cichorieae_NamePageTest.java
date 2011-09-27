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
import eu.etaxonomy.dataportal.pages.GenericPortalPage;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cichorieae })
public class Cichorieae_NamePageTest extends CdmDataPortalTestBase{


	static UUID intybellia_rosea_cass_Uuid = UUID.fromString("b8f725f0-320a-49e5-aa9a-82cef1c47c17");

//	static UUID lactuca_favratii_Uuid = UUID.fromString("6027e1fa-9fe5-4ddc-a2de-f72bfa7378c0");
//
//	static UUID crepis_oenipontana_Uuid = UUID.fromString("31b8757f-6acb-4826-ba7f-b2d116dc713c");
//
//	static UUID crepis_artificialis_Uuid = UUID.fromString("3eabdf89-ddeb-461c-b6f8-341bb8deb7bf");


	@Test
	public void intybellia_rosea_cass() throws MalformedURLException {
		GenericPortalPage p = new GenericPortalPage(driver, getContext(), "name/" + intybellia_rosea_cass_Uuid);
		assertEquals(getContext().prepareTitle("Crepis purpurea"), p.getTitle());
	}


}
