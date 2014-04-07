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

    static UUID pilosella_guthnikiana_Uuid = UUID.fromString("6d711fa0-77c3-42df-9d44-83fdc78f3482");
    static UUID lactuca_glandulifera_Uuid = UUID.fromString("6ece0be7-ba4a-4363-b103-4e60429988e5");
    static UUID dubyaea_hispida_Uuid = UUID.fromString("e72f3bc5-70d7-404c-bfd7-125fec7387bb");


    @Test
    public void pilosella_guthnikiana_homonyms() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), pilosella_guthnikiana_Uuid);
        assertEquals(getContext().prepareTitle("Pilosella guthnikiana"), driver.getTitle());
        assertEquals("Pilosella guthnikiana (Hegetschw.) Soják in Preslia 43: 185. 1971", p.getAcceptedNameText());

        assertEquals("= Hieracium multiflorum Gaudin, Fl. Helv. 5: 87. 1829 [non Hieracium multiflorum Gray 1821]", p.getHeterotypicalGroupSynonymName(1, 1));
        assertEquals("= Hieracium cruentum Nägeli & Peter, Hierac. Mitt.-Eur. 1: 455, 811. 1885, nom. illeg. [non Hieracium cruentum Jord. 1849]", p.getHeterotypicalGroupSynonymName(6, 1));

   }

    @Test
    public void lactuca_glandulifera_homonyms() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), lactuca_glandulifera_Uuid);
        assertEquals(getContext().prepareTitle("Lactuca glandulifera"), driver.getTitle());
        assertEquals("Lactuca glandulifera Hook. f. in J. Linn. Soc., Bot. 7: 203. 1864", p.getAcceptedNameText());

        assertEquals("= Lactuca integrifolia De Wild., Pl. Bequaert. 5: 456. 1932, nom. illeg. [non Lactuca integrifolia Nutt. 1818 nec Lactuca integrifolia Bigelow 1824]", p.getHeterotypicalGroupSynonymName(3, 1));
   }

    @Test
    public void dubyaea_hispida_homonyms() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), dubyaea_hispida_Uuid);
        assertEquals(getContext().prepareTitle("Dubyaea hispida"), driver.getTitle());
        assertEquals("Dubyaea hispida DC., Prodr. 7: 247. 1838, nom. nov.", p.getAcceptedNameText());

        assertEquals("≡ Hieracium hispidum D. Don, Prodr. Fl. Nepal.: 165. 1825 [non Hieracium hispidum Forssk. 1775]", p.getHomotypicalGroupSynonymName(1));
        assertEquals("≡ Lactuca dubyaea C. B. Clarke, Compos. Ind.: 271. 1876 [non Lactuca hispida DC.]", p.getHomotypicalGroupSynonymName(3));
   }
}
