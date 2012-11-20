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
public class Cichorieae_CommonNamesTest extends CdmDataPortalTestBase{


    static UUID lactuca_serriola_uuid = UUID.fromString("85176c77-e4b6-4899-a08b-e257ab09350a");


    /**
     * regression test for issue ##3160 (Cichorieae Portal: Common names not correctly orderd)
     *
     * @throws MalformedURLException
     */
    @Test
    public void lactuca_serriola() throws MalformedURLException {
        TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), lactuca_serriola_uuid);
        String expectedName = "Lactuca serriola";
        assertEquals(getContext().prepareTitle(expectedName), p.getTitle());
        FeatureBlock commonNamesBlock = p.getFeatureBlockAt(3, "common_names", "div", "span");
        assertNotNull(commonNamesBlock);

        String expected = "Common names\nlechuguilla145, escariola145; Albanian (Albania): Ogrisht146; Arabic (Lebanon): خَسّ الزَّيْت147; Arabic (Saudi Arabia): Khass-al-Hammar148; Arabic (Syria): خَسّ الزَّيْت149; Armenian (Armenia): Կաթնուկ կողմնացույց150; Bulgarian (Bulgaria): Компасна салата151; Czech (Czech Republic): Locika kompasová152; Danish (Denmark): Tornet Salat153; English (Australia): compass plant154, Prickly Lettuce155, Prickly Lettuce156, Prickly Lettuce157, Prickly Lettuce158, compass plant159";
        String firstChars = commonNamesBlock.getText().substring(0, expected.length());
        assertEquals(expected, firstChars);
    }

}
