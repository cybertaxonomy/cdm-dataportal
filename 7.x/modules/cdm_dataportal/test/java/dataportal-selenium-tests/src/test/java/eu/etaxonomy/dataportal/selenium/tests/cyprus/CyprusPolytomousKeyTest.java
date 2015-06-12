// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cyprus;

import java.util.List;
import java.util.UUID;

import org.junit.Assert;
import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage.KeyLineData;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage.LinkClass;
import eu.etaxonomy.dataportal.pages.PortalPage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts({ DataPortalContext.cyprus })
public class CyprusPolytomousKeyTest extends CdmDataPortalTestBase {


    @Test
    public void key_to_Achillea() throws Exception {

        UUID keyUuid = UUID.fromString("55527688-92b4-4750-85ed-5808ff0a265e");

        PolytomousKeyPage p = new PolytomousKeyPage(driver, getContext(), keyUuid);

        PortalPage targetPage;
        KeyLineData keyLineData;


        Assert.assertEquals(getContext().prepareTitle("Achillea"), driver.getTitle());

        Assert.assertEquals("but modified, supplemented and names used differing", p.getKeyAnnotationsText());

        List<BaseElement> sourceRefs = p.getSources();

        Assert.assertEquals( "G. N. Hadjikyriakou, Aromatic and spicy plants in Cyprus. 2007", sourceRefs.get(0).getText());
        Assert.assertEquals(1, sourceRefs.get(0).getLinksInElement().size());

        Assert.assertEquals("R. D. Meikle, Flora of Cyprus 2. 1985", sourceRefs.get(1).getText());
        Assert.assertEquals(1, sourceRefs.get(1).getLinksInElement().size());

        // -------- //
        keyLineData = new KeyLineData("1",
                "Capitula without ligulate ray-florets; leaves entire or subentire",
                LinkClass.nodeLinkToTaxon, "Achillea maritima subsp. maritima");
        targetPage = p.followPolytomousKeyLine(0, keyLineData);
        Assert.assertEquals(getContext().prepareTitle(keyLineData.getLinkText()), driver.getTitle());
        // This page should also have a key tab
        Assert.assertEquals("Keys", targetPage.getPrimaryTabs().get(2).getText());
        p.get();

        // -------- //
        keyLineData = new KeyLineData("1'", "Capitula with ligulate ray-florets; leaves pinnatisect",
                LinkClass.nodeLinkToNode, "2");
//        targetPage = p.followPolytomousKeyLine(1, keyLineData);
//        Assert.assertEquals(p.getPageURL().getPath(), targetPage.getPageURL().getPath()); // TODO
//        p.get();


        keyLineData = new KeyLineData("2", "Ray-florets yellow", LinkClass.nodeLinkToNode, "3");
        keyLineData = new KeyLineData("2'", "Ray-florets white", LinkClass.nodeLinkToNode, "4");


    }

}
