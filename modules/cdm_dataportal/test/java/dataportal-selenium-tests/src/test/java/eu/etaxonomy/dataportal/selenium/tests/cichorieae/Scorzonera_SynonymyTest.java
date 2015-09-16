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
import java.util.List;
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.TypeDesignationElement;
import eu.etaxonomy.dataportal.elements.TypeDesignationType;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cichorieae })
public class Scorzonera_SynonymyTest extends CdmDataPortalTestBase{

    static UUID scorzonera_Uuid = UUID.fromString("c1e8a3f2-2b65-4aad-ad25-1cf9df92e290");

    @Test
    public void scorzonera_typeDesignations() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), scorzonera_Uuid);
        assertEquals(getContext().prepareTitle("Scorzonera"), p.getTitle());
        assertEquals("Scorzonera L., Sp. Pl.: 790. 1753", p.getAcceptedNameText());


        // check type designation for accepted taxon
        List<TypeDesignationElement> typeDesignations = p.getHomotypicalGroupTypeDesignations();
        assertEquals("Expecting one Typedesignation", 1, typeDesignations.size());
        assertEquals(TypeDesignationType.nameTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
        assertEquals("Lectotype (designated by Green 1929: 1771): Scorzonera humilis L.", typeDesignations.get(0).getText()); // last digit of 1771 is footnote key !!!
        assertEquals("should have one footnote from type designation", 1, p.getAcceptedNameFootNotes().size());
        assertEquals("", "1. Green, Proposals by British Botanists. 1929", p.getAcceptedNameFootNotes().get(0).getText());
//        assertNull("The typified name should not have a name description (protologue)", typeDesignations.get(0).getNameDescription()); // FIXME

        assertEquals("= Gelasia Cass. in Bull. Sci. Soc. Philom. Paris 1818: 33. 1818", p.getHeterotypicalGroupSynonymName(1, 1));
        List<TypeDesignationElement> heterotypicalGroupTypeDesignations = p.getHeterotypicalGroupTypeDesignations(1);
        assertEquals("Type: Gelasia villosa (Scop.) Cass.", heterotypicalGroupTypeDesignations.get(0).getText());
        assertEquals(TypeDesignationType.nameTypeDesignation, heterotypicalGroupTypeDesignations.get(0).getTypeDesignationType());

//        test case only available in production portal
//        assertEquals("≡ Scorzonera subg. Lasiospora (Cass.) Peterm., Deutschl. Fl.: 334. 1846-1849", p.getHeterotypicalGroupSynonymName(2, 3));
//        heterotypicalGroupTypeDesignations = p.getHeterotypicalGroupTypeDesignations(2);
//        assertEquals("Lectotype (designated by Tzvelev 1989: 452): Lasiospora hirsuta (Gouan) Cass.", heterotypicalGroupTypeDesignations.get(0).getText());
//        assertEquals(TypeDesignationType.nameTypeDesignation, heterotypicalGroupTypeDesignations.get(0).getTypeDesignationType());
//        assertEquals("Should have one foot note", 1, p.getHeterotypicalGroupFootNotes(2).size()); // FIXME


        // also to some synonymy checks
        assertEquals("– Scorzonera sect. Euscorzonera DC., Prodr. 7: 117. 1838, nom. inval.", p.getHeterotypicalGroupSynonymName(58, 1));
        assertEquals("– Scorzonera subsect. Euvierhapperia Lipsch., Fragm. Monogr. Scorzonera 2: 89. 1939, nom. inval.", p.getHeterotypicalGroupSynonymName(59, 1));
    }
}
