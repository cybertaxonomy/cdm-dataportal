/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.reference;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Assertions;

import java.net.MalformedURLException;
import java.util.UUID;

import org.junit.jupiter.api.BeforeEach;
import eu.etaxonomy.dataportal.junit.DataPortalTest;
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


    @BeforeEach
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }

    /**
     * related to https://dev.e-taxonomy.eu/redmine/issues/6709
     */
    @DataPortalTest
    public void testTitle_issue6709() throws MalformedURLException {

        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "media/" + ehrenberg_drawing_uuid.toString());
        assertEquals(
                "Media (PIA01466.jpg) | Integration test reference",
                driver.getTitle()
                );
        assertEquals(
                "Media (PIA01466.jpg)",
                driver.findElement(By.id("page-title")).getText()
                );
    }

    /**
     * related to https://dev.e-taxonomy.eu/redmine/issues/6316
     */
    @DataPortalTest
    public void testSources_issue6316() throws MalformedURLException {

        GenericPortalPage p = new GenericPortalPage(driver, getContext(), "media/" + ehrenberg_drawing_uuid.toString());
        WebElement mediaCaption = driver.findElement(By.className("media-caption"));
        assertEquals("Source:\nNaturkundemuseum Berlin (MFN) - Ehrenberg Collection – http://download.naturkundemuseum-berlin.de/Ehrenberg/ [761]", mediaCaption.getText());
    }

}
