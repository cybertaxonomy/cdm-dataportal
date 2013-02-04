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
public class Crepis_tectorum_Synonymy extends CdmDataPortalTestBase{

    static UUID crepis_tectorum_Uuid = UUID.fromString("c62dff09-3f04-4f05-9aac-904d51ac1b77");

    @Test
    public void crepis_oenipontana() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepis_tectorum_Uuid);
        assertEquals(getContext().prepareTitle("Crepis tectorum"), p.getTitle());
        assertEquals("Crepis tectorum L., Sp. Pl.: 807. 1753", p.getAcceptedNameText());
        assertEquals("≡ Crepis varia Moench, Methodus: 534. 1794, nom. illeg.", p.getHomotypicalGroupSynonymName(2));
        assertEquals("≡ Crepis stricta Schultz, Prodr. Fl. Starg. Suppl.: 41. 1819 [non Crepis stricta Scop. 1772]", p.getHeterotypicalGroupSynonymName(1, 2));
        assertEquals("= Crepis lanceolata Kit. [non Crepis lanceolata Sch. Bip. 1854]", p.getHeterotypicalGroupSynonymName(9, 1));
    }



}
