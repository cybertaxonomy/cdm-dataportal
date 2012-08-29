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

import static org.junit.Assert.*;
import static org.junit.Assert.assertEquals;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Ignore;
import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cichorieae })
public class Cichorieae_FootnoteTest extends CdmDataPortalTestBase{


    static UUID crepidiastrum_chelidoniifolium_uuid = UUID.fromString("3e0cdb93-020c-4e2f-be85-e9eb1ec107be");


    /**
     * test for issue #2772 (some footnotes in synonymy missing)
     *
     * @throws MalformedURLException
     */
    @Test
    @Ignore // test data has been lost somehow, thus this test is ignored. Valid test data is available in the production portal
    public void crepidiastrum_chelidoniifolium_issue_2772() throws MalformedURLException {
        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), crepidiastrum_chelidoniifolium_uuid);
        String expectedName = "Crepidiastrum chelidoniifolium";
        assertEquals(getContext().prepareTitle(expectedName), p.getTitle());

        assertEquals("expecting one footnoteKey", 1, p.getHeterotypicalGroupFootNoteKeys(2).size());
        List<BaseElement> footNotes = p.getHeterotypicalGroupFootNotes(2);
        assertEquals("expecting one footnotw", 1, footNotes.size());
        String first100Chars = footNotes.get(0).getText().substring(0, 100);
        assertEquals("1. As has been noted first by Sennikov (in Bot Zhurn. 82(5): 114. 1997), the names Lactuca saxatilis", first100Chars);
    }

}
