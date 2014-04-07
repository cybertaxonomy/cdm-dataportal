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
public class HomonymTest extends CdmDataPortalTestBase{

    static UUID scorzonera_Uuid = UUID.fromString("6d711fa0-77c3-42df-9d44-83fdc78f3482");

    @Test
    public void scorzonera_typeDesignations() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), scorzonera_Uuid);
        assertEquals(getContext().prepareTitle("Pilosella guthnikiana"), p.getTitle());
        assertEquals("Pilosella guthnikiana (Hegetschw.) Soják in Preslia 43: 185. 1971", p.getAcceptedNameText());


        assertEquals("= Hieracium multiflorum Gaudin, Fl. Helv. 5: 87. 1829 [non Hieracium multiflorum Gray 1821]", p.getHeterotypicalGroupSynonymName(1, 1));
        assertEquals("= Hieracium cruentum Nägeli & Peter, Hierac. Mitt.-Eur. 1: 455, 811. 1885, nom. illeg. [non Hieracium cruentum Jord. 1849]", p.getHeterotypicalGroupSynonymName(6, 1));
    }

    // TODO find taxon with 'nec' homonym and implement test for this

    //TODO implement test for basionym of "Hieracium sparsum subsp. macrolepis" this taxon should also have homonym name rels !!!
}
