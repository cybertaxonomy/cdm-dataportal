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
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.GenericPortalPage;

/**
 * Issues to be covered by this TestClass:
 *
 * #3616
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class MediaPageTest extends CdmDataPortalTestBase{

    static final UUID ehrenberg_drawing_uuid = UUID.fromString("d0bf650c-4cc6-4dfe-bb2f-a289b426af2f");


    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }

    /**
     * related to https://dev.e-taxonomy.eu/redmine/issues/6709
     */
    @Test
    public void testTitle_issue6709() throws MalformedURLException {

        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "media/" + ehrenberg_drawing_uuid.toString());
        assertEquals(
                "Media (ECdraw671.jpg) | Integration test reference",
                driver.getTitle()
                );
        assertEquals(
                "Media (ECdraw671.jpg)",
                driver.findElement(By.id("page-title")).getText()
                );
    }


    /**
     * related to https://dev.e-taxonomy.eu/redmine/issues/6316
     */
    @Test
    public void testSources_issue6316() throws MalformedURLException {


        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "media/" + ehrenberg_drawing_uuid.toString());
        WebElement mediaCaption = driver.findElement(By.className("media-caption"));
        assertEquals("Source:\nNaturkundemuseum Berlin (MFN) - Ehrenberg Collection [761]", mediaCaption.getText());
    }

}
