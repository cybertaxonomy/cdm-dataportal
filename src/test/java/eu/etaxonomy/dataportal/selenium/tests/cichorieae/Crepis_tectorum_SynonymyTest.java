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
import java.util.List;
import java.util.UUID;

import org.junit.Test;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.cichorieae })
public class Crepis_tectorum_SynonymyTest extends CdmDataPortalTestBase{

    static UUID crepis_tectorum_Uuid = UUID.fromString("c62dff09-3f04-4f05-9aac-904d51ac1b77");

    @Test
    public void crepis_tectorum_synonyms() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepis_tectorum_Uuid);
        assertEquals(getContext().prepareTitle("Crepis tectorum"), driver.getTitle());

        assertEquals("Crepis tectorum L., Sp. Pl.: 807. 1753", p.getAcceptedNameText());

        // HomotypicalGroupSynonyms
        assertEquals("≡ Hedypnois tectorum (L.) Huds., Fl. Angl., ed. 2: 341. 1778", p.getHomotypicalGroupSynonymName(1));
        assertEquals("≡ Hieracium tectorum (L.) Karsch, Fl. Westfalen: 331. 1853", p.getHomotypicalGroupSynonymName(2));
        assertEquals("≡ Hieracioides tectorum (L.) Kuntze, Revis. Gen. Pl. 1: 346. 1891", p.getHomotypicalGroupSynonymName(3));
        assertEquals("≡ Crepis varia Moench, Methodus: 534. 1794, nom. illeg.", p.getHomotypicalGroupSynonymName(4));
        assertEquals("≡ Crepis muralis Salisb., Prodr. Stirp. Chap. Allerton: 182. 1796, nom. illeg.", p.getHomotypicalGroupSynonymName(5));
        assertEquals("≡ Crepis tectoria Dulac, Fl. Hautes-Pyrénées: 491. 1867, nom. illeg.", p.getHomotypicalGroupSynonymName(6));

        // 1st HeterotypicalGroup
        assertEquals("= Crepis tectorum var. segetalis Roth, Tent. Fl. Germ. 2: 254. 17931", p.getHeterotypicalGroupSynonymName(1, 1));
        assertEquals("≡ Crepis stricta Schultz, Prodr. Fl. Starg. Suppl.: 41. 1819 [non Crepis stricta Scop. 1772]", p.getHeterotypicalGroupSynonymName(1, 2));
        assertEquals("≡ Crepis tectorum var. stricta E. Mey. ex Bisch., Beitr. Fl. Deutschl.: 274. 1851, nom. illeg.", p.getHeterotypicalGroupSynonymName(1, 3));
        assertEquals("≡ Crepis tectorum var. minima Schur, Enum. Pl. Transsilv.: 376. 1866, nom. illeg.", p.getHeterotypicalGroupSynonymName(1, 4));
        //this heterotypic group syn secs don't differ to the sec of the accepted taxon
        WebElement synSecContainer = p.getNewHeterotypicalGroupSynSecs(1);
        assertEquals(null, synSecContainer);
        assertEquals("= Crepis lanceolata Kit. [non Crepis lanceolata Sch. Bip. 1854]", p.getHeterotypicalGroupSynonymName(9, 1));


        //second heterotypic group with different syn sec
        assertEquals("= Crepis lachenalii Gochnat, Tent. Pl. Cich.: 19. 1808 [is earlier homonym of Crepis lachenalii C. C. Gmel. 1811]", p.getHeterotypicalGroupSynonymName(2, 1));
        assertEquals("≡ Crepis tectorum var. gracilis Wallr., Sched. Crit.: 430. 1822", p.getHeterotypicalGroupSynonymName(2, 2));
        synSecContainer = p.getNewHeterotypicalGroupSynSecs(2);
        assertEquals("Syn. sec.: 2025: Edit Test Reference2", synSecContainer.getText());

    }



}
