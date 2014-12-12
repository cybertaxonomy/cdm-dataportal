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

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cichorieae })
public class Cichorieae_CommonNamesTest extends CdmDataPortalTestBase{


    static UUID lactuca_serriola_uuid = UUID.fromString("85176c77-e4b6-4899-a08b-e257ab09350a");


    /**
     * regression test for issue ##3160 (Cichorieae Portal: Common names not correctly ordered)
     *
     * @throws MalformedURLException
     */
    @Test
    public void lactuca_serriola() throws MalformedURLException {
        TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), lactuca_serriola_uuid);
        String expectedName = "Lactuca serriola";
        assertEquals(getContext().prepareTitle(expectedName), p.getTitle());

        FeatureBlock distributionBlock = p.getFeatureBlockAt(2, "distribution", "div", "dd");
        assertNotNull(distributionBlock);

        List<BaseElement> footnotes = distributionBlock.getFootNotes();
        assertTrue("footnotes must not be empty", footnotes.size() > 0);
        // testing for the duplicate footnotes named below in the  FIXME (related to #4383 )
        assertEquals("AN. Komarov, V. L., Flora SSSR 29. 1964", footnotes.get(39).getText());
        assertEquals("AQ. Komarov, V. L., Flora SSSR 29. 1964 (as Lactuca altaica)", footnotes.get(42).getText());


        FeatureBlock commonNamesBlock = p.getFeatureBlockAt(3, "common_names", "div", "span");
        assertNotNull(commonNamesBlock);

//        no longer valid since 2012-11-19: String expected = "Common names\nlechuguilla145, escariola145; Albanian (Albania): Ogrisht146; Arabic (Lebanon): خَسّ الزَّيْت147; Arabic (Saudi Arabia): Khass-al-Hammar148; Arabic (Syria): خَسّ الزَّيْت149; Armenian (Armenia): Կաթնուկ կողմնացույց150; Bulgarian (Bulgaria): Компасна салата151; Czech (Czech Republic): Locika kompasová152; Danish (Denmark): Tornet Salat153; English (Australia): compass plant154, Prickly Lettuce155, Prickly Lettuce156, Prickly Lettuce157, Prickly Lettuce158, compass plant159";
//        no longer valid since 2013-06-27, sorting and merging implemented: String expected = "Common names\nlechuguilla144, escariola144; Albanian (Albania): Ogrisht145; Arabic (Lebanon): خَسّ الزَّيْت146; Arabic (Saudi Arabia): Khass-al-Hammar147; Arabic (Syria): خَسّ الزَّيْت148; Armenian (Armenia): Կաթնուկ կողմնացույց149; Bulgarian (Bulgaria): Компасна салата150; Czech (Czech Republic): Locika kompasová151; Danish (Denmark): Tornet Salat152; English (Australia): compass plant153, Prickly Lettuce154, Prickly Lettuce155, Prickly Lettuce156, Prickly Lettuce157, compass plant158";
//        no longer valid since 2013-10-08, display of nameUsedInSource implemented, leads to semi-duplication of footnotes since some are without name in source data other are with: String expected = "Common names\n(Mexico): escariola144, lechuguilla144; Albanian (Albania): Ogrisht145; Arabic (Lebanon): خَسّ الزَّيْت146; Arabic (Saudi Arabia): Khass-al-Hammar147; Arabic (Syria): خَسّ الزَّيْت148; Armenian (Armenia): Կաթնուկ կողմնացույց149; Bulgarian (Bulgaria): Компасна салата150; Czech (Czech Republic): Locika kompasová151; Danish (Denmark): Tornet Salat152; English (Australia): Prickly Lettuce153,154,155,156,157,158,159, Prickly lettuce160,161, compass plant160,161, milk thistle160,161" +
//        no longer valid since 2013-11-21 (#3475 fixed) String expected = "Common names\n(Mexico): escariola150, lechuguilla150; Albanian (Albania): Ogrisht151; Arabic (Lebanon): خَسّ الزَّيْت152; Arabic (Saudi Arabia): Khass-al-Hammar153; Arabic (Syria): خَسّ الزَّيْت154; Armenian (Armenia): Կաթնուկ կողմնացույց155; Bulgarian (Bulgaria): Компасна салата156; Czech (Czech Republic): Locika kompasová157; Danish (Denmark): Tornet Salat158; English (Australia): Prickly Lettuce159,160,161,162,163,164,165, Prickly lettuce166,167, compass plant166,167, milk thistle166,167";
//        no longer valid since 2013-12-06 implementing rule 2 and 3 for #3904 String expected = "Common names\n(Mexico): escariola156, lechuguilla156; Albanian (Albania): Ogrisht157; Arabic (Lebanon): خَسّ الزَّيْت158; Arabic (Saudi Arabia): Khass-al-Hammar159; Arabic (Syria): خَسّ الزَّيْت160; Armenian (Armenia): Կաթնուկ կողմնացույց161; Bulgarian (Bulgaria): Компасна салата162; Czech (Czech Republic): Locika kompasová163; Danish (Denmark): Tornet Salat164; English (Australia): Prickly Lettuce165,166,167,168,169,170,171, Prickly lettuce172,173, compass plant172,173, milk thistle172,173";
//        no longer valid since 2014-08-27 fixing #3915 String expected = "Common names\n(Mexico): escariola153, lechuguilla153; Albanian (Albania): Ogrisht154; Arabic (Lebanon): خَسّ الزَّيْت155; Arabic (Saudi Arabia): Khass-al-Hammar156; Arabic (Syria): خَسّ الزَّيْت157; Armenian (Armenia): Կաթնուկ կողմնացույց158; Bulgarian (Bulgaria): Компасна салата159; Czech (Czech Republic): Locika kompasová160; Danish (Denmark): Tornet Salat161; English (Australia): Prickly Lettuce162,163,164,165,166,167,168, Prickly lettuce169,170, compass plant169,170, milk thistle169,170";

        // FIXME the distribution entries have duplicate foonote key for source references, see #4383 (references in bibliography need de-duplication)
        //       for example "40. Komarov, V. L., Flora SSSR 29. 1964", and "43. Komarov, V. L., Flora SSSR 29. 1964 (as Lactuca altaica)"
        //       this is why the Common name footnote keys are having an offset, the below out-commented string should be the correct one:
        //  String expected = "Common names\n(Mexico): escariola155, lechuguilla155; Albanian (Albania): Ogrisht156,157; Arabic (Lebanon): خَسّ الزَّيْت156,158; Arabic (Saudi Arabia): Khass-al-Hammar159; Arabic (Syria): خَسّ الزَّيْت156,160; Armenian (Armenia): Կաթնուկ կողմնացույց156,161; Bulgarian (Bulgaria): Компасна салата156,162; Czech (Czech Republic): Locika kompasová156,163; Danish (Denmark): Tornet Salat156,164; English (Australia): Prickly Lettuce165,166,167,168,169,170,171, Prickly lettuce172,173, compass plant";
        String expected = "Common names\n(Mexico): escariola168, lechuguilla168; Albanian (Albania): Ogrisht169,170; Arabic (Lebanon): خَسّ الزَّيْت169,171; Arabic (Saudi Arabia): Khass-al-Hammar172; Arabic (Syria): خَسّ الزَّيْت169,173; Armenian (Armenia): Կաթնուկ կողմնացույց169,174; Bulgarian (Bulgaria): Компасна салата169,175; Czech (Czech Republic): Locika kompasová169,176; Danish (Denmark): Tornet Salat169,177; English (Australia): Prickly Lettuce178,179,180,181,182,183,184, Prickly lettuce185,186, compass plant";
        String firstChars = commonNamesBlock.getText().substring(0, expected.length());

        assertEquals(expected, firstChars);
    }

}
