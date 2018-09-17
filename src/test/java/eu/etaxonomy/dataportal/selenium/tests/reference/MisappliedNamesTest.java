/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.reference;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.StringConstants;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * Issues to be covered by this TestClass:
 *
 * #5676
 * #5647
 * #5492
 * #7658 + #6682
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class MisappliedNamesTest extends CdmDataPortalTestBase{

    static final UUID miconia_cubacinerea_Uuid = UUID.fromString("c6716cee-2039-4ba8-a239-4b1b353f9c84");

    static final UUID trichocentrum_undulatum_Uuid = UUID.fromString("7e86b2a4-ba71-4494-b544-ae5656e02ed2");

    static final UUID nepenthes_abalata_Uuid = UUID.fromString("9b588d8a-c4fa-430a-b9c7-026bf715ecf6");



    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getBaseUri().toString());
    }

    /**
     * Test for correct sensu representation of misapplied names, see #5676, #5647 and #7658
     *
     * https://dev.e-taxonomy.eu/redmine/issues/5647
     * https://dev.e-taxonomy.eu/redmine/issues/7658
     *
     * NOTE: Species solaris has no authorship!!
     */
    @Test
    public void tesIssue5647() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), miconia_cubacinerea_Uuid);

        // no sensu but with Combination Authors:
        assertEquals("–\n\"Ossaea angustifolia\" auct., non Cheek", p.getMisappliedName(1).getText());
        //
        assertEquals("–\n\"Ossaea glomerata\" sensu A&S1; sensu A&S: 221; sensu A&S: 331; sensu Lem2; sensu Species solaris; auct.; auct. sensu A&S1; auctrs. afr.", p.getMisappliedName(2).getText());
        // with doubtful flag
        assertEquals("–\n" + StringConstants.DOUBTFULMARKER_SPACE +"\"Ossaea glomerata\" sensu A&S1", p.getMisappliedName(3).getText());

        assertEquals("p.p.MAN\nOssaea glomerata sec. A&S1; sec. Species solaris", p.getMisappliedName(4).getText());

        assertEquals("partial MAN\nOssaea glomerata sec. A&S: 331; auct. sec. A&S1", p.getMisappliedName(5).getText());
        // Test also invalid designation which is rendered with the misapplied names
        assertEquals("–\nOssaea maculata sec. Lem2, rel. sec. A&S1", p.getMisappliedName(6).getText());

        assertEquals("≜⊃\nTrichocentrum undulatum (Sw.) Ackerman & M. W. Chase sec. My Classification3", p.getTaxonRelationships(1).getText());
        assertEquals("∅ Achilllea santolina Lag. sec. Testor 2018+4", p.getTaxonRelationships(1).getText());


        List<BaseElement> footnotes = ElementUtils.findFootNotes(p.getTaxonRelationships());
        assertEquals(4, footnotes.size());
        assertEquals("1. A&S, Plantas vasculares de Oz", footnotes.get(0).getText());
        assertEquals("2. Lem, New Species in the solar system", footnotes.get(1).getText());
        assertEquals("3. Kohlbecker, .....TODO .... 2016-2018", footnotes.get(0).getText());
        assertEquals("4. Testor ... TODO ... 2018+", footnotes.get(1).getText());
        // "Species solaris" must not be in the footnotes as it has the same title as the short citation

    }

    /**
     * Test to reproduce issue https://dev.e-taxonomy.eu/redmine/issues/7766
     *
     * TODO merge with  tesIssue5647 () once fixed
     *
     */
    @Test
    @Ignore
    public void tesIssue7766() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), miconia_cubacinerea_Uuid);

        assertEquals("≜⊃\nTrichocentrum undulatum (Sw.) Ackerman & M. W. Chase sec. My Classification3", p.getTaxonRelationships(1).getText());
        assertEquals("∅ Achilllea santolina Lag. sec. Testor 2018+4", p.getTaxonRelationships(1).getText());


        List<BaseElement> footnotes = ElementUtils.findFootNotes(p.getTaxonRelationships());
        assertEquals(4, footnotes.size());
        assertEquals("1. A&S, Plantas vasculares de Oz", footnotes.get(0).getText());
        assertEquals("2. Lem, New Species in the solar system", footnotes.get(1).getText());
        assertEquals("3. Kohlbecker, .....TODO .... 2016-2018", footnotes.get(0).getText());
        assertEquals("4. Testor ... TODO ... 2018+", footnotes.get(1).getText());

    }


    /**
     * https://dev.e-taxonomy.eu/redmine/issues/5492
     */
    @Test
    public void testIssue5492() throws MalformedURLException {

        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), trichocentrum_undulatum_Uuid);

        WebElement misappliedName1 = p.getMisappliedName(1);
        assertNotNull(misappliedName1);
        assertEquals("–\n" + StringConstants.DOUBTFULMARKER_SPACE +"\"Oncidium carthaginense\" auct. sensu Greuter, W. & Rankin Rodríguez, R1", misappliedName1.getText());

        WebElement misappliedName2 = p.getMisappliedName(2);
        assertNotNull(misappliedName2);
        assertEquals("–\n\"Oncidium guttatum\" auct. sensu Greuter, W. & Rankin Rodríguez, R1", misappliedName2.getText());



        List<BaseElement> footnotes = ElementUtils.findFootNotes(p.getTaxonRelationships());
        assertEquals(1, footnotes.size());
        assertEquals("1. Greuter, W. & Rankin Rodríguez, R, Plantas vasculares de Cuba: inventario preliminar. Tercera edición, actualizada. Vascular plants of Cuba: a preliminary checklist. Third updated edition.", footnotes.get(0).getText());
    }

    /**
     * https://dev.e-taxonomy.eu/redmine/issues/7658
     * https://dev.e-taxonomy.eu/redmine/issues/6682
     *
     * TODO 1b) Der Buchlink verweist auf den Protolog von N. blancoi Blume – der hat ja aber gerade nichts mit
     * dieser Namensanwendung zu tun (also: Protologbildlinks müssen bei MAN weggelassen werden oder der Link
     * muss nach dem <non Blume> eingefügt werden)
     */
    @Test
    public void testIssue7658() throws MalformedURLException {


        TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), nepenthes_abalata_Uuid);

        WebElement misappliedName1 = p.getMisappliedName(1);
        assertNotNull(misappliedName1);
        assertEquals("–\n\"Nepenthes alata\" pro parte, sensu Cheek, M.R. & Jebb, M.H.P. 20011, non Blanco, err. sec. Cheek, M.R. & Jebb, M.H.P. 20132", misappliedName1.getText());

        WebElement misappliedName2 = p.getMisappliedName(2);
        assertNotNull(misappliedName2);
        assertEquals("–\n\"Nepenthes blancoi\" pro parte, sensu Macfarlane 19083, non Blume, err. sec. Cheek, M.R. & Jebb, M.H.P. 20132", misappliedName2.getText());

        List<BaseElement> footnotes = ElementUtils.findFootNotes(p.getTaxonRelationships());
        assertEquals(3, footnotes.size());
   }

}
