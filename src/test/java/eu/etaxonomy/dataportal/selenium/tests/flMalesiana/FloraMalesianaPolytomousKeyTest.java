/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.flMalesiana;

import java.util.UUID;

import org.junit.Assert;
import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage.KeyLineData;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage.LinkClass;
import eu.etaxonomy.dataportal.pages.PortalPage;
import eu.etaxonomy.dataportal.DrupalVars;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts({ DataPortalSite.floramalesiana })
public class FloraMalesianaPolytomousKeyTest extends CdmDataPortalTestBase {

    /**
     * NOTE:
     *
     * This test expects 'Show Taxon name display' different from the production site:
     *
     * <h6>Name render templates:</h6>
     *
     * <pre>
     "taxon_page_title,polytomousKey": {
        "nameAuthorPart": {
            "#uri": true
        },
        "referencePart": true
      },
    </pre>
     *
     *
     * @throws Exception
     */
    @Test
    public void key_to_Malaysian_Sapindaceae_Genera() throws Exception {

        UUID keyUuid = UUID.fromString("8427a8f5-17b8-4c2b-9fff-143248d18643");

        PolytomousKeyPage p = new PolytomousKeyPage(driver, getContext(), keyUuid);

        PortalPage targetPage;
        KeyLineData keyLineData;


        Assert.assertEquals(getContext().prepareTitle("KEY 1 TO THE MALESIAN GENERA (based on vegetative and flower characters)"), p.getTitle());

        // -------- //
        keyLineData = new KeyLineData("1",
                "Trees or shrubs, exceptionally lianas. Leaves simple, unifoliolate, (bi)pinnate or digitate. Inflorescences without basal tendrils",
                LinkClass.nodeLinkToNode, "2");
        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
            targetPage = p.followPolytomousKeyLine(0, keyLineData, true);
        }else{
            targetPage = p.followPolytomousKeyLine(0, keyLineData, false);
        }


        //FIXME:
        //Assert.assertTrue(targetPage.toString().startsWith(p.getPageURL().toString() + "#"));

        // -------- //
        keyLineData = new KeyLineData("1'", "Herbaceous or woody climbers. Leaves biternate. Inflorescences with basal tendrils",
                LinkClass.nodeLinkToTaxon, "Cardiospermum L.", ", Sp. Pl.: 366. 1753");
        p = new PolytomousKeyPage(driver, getContext(), keyUuid);
       if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
                    targetPage = p.followPolytomousKeyLine(1, keyLineData, true);
                }else{
                    targetPage = p.followPolytomousKeyLine(1, keyLineData, false);
                }
       // targetPage = p.followPolytomousKeyLine(1, keyLineData);
        Assert.assertEquals(getContext().prepareTitle(keyLineData.getLinkTextWithSuffix()), targetPage.getTitle());
        p.get();

        // -------- //
        keyLineData = new KeyLineData("2", "Leaves simple, unifoliolate, (im)paripinnate or digitate", LinkClass.nodeLinkToNode, "3");
        p = new PolytomousKeyPage(driver, getContext(), keyUuid);
        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
                    targetPage = p.followPolytomousKeyLine(2, keyLineData, true);
                }else{
                    targetPage = p.followPolytomousKeyLine(2, keyLineData, false);
                }
        //targetPage = p.followPolytomousKeyLine(2, keyLineData);
        //FIXME (same as above):
        //Assert.assertEquals(p, targetPage);

        // -------- //
        keyLineData = new KeyLineData("2'", "Leaves bipinnate", LinkClass.nodeLinkToTaxon, "Tristiropsis Radlk.", " in Dur., Index Gen. Phan.: 76. 1888");
        p = new PolytomousKeyPage(driver, getContext(), keyUuid);
        if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
                    targetPage = p.followPolytomousKeyLine(3, keyLineData, true);
                }else{
                    targetPage = p.followPolytomousKeyLine(3, keyLineData, false);
                }
        //targetPage = p.followPolytomousKeyLine(3, keyLineData);
        Assert.assertEquals(getContext().prepareTitle(keyLineData.getLinkTextWithSuffix()), targetPage.getTitle());
        p.get();

    }

}
