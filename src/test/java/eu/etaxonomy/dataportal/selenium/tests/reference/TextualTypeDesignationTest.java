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
import org.junit.Test;
import org.openqa.selenium.By;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.TypeDesignationElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.NamePage;

/**
 * Issues to be covered by this TestClass:
 *
 * https://dev.e-taxonomy.eu/redmine/issues/8134
 *
 * @author a.kohlbecker
 *
 */
@DataPortalContexts( { DataPortalSite.reference })
public class TextualTypeDesignationTest extends CdmDataPortalTestBase{

    static final UUID equisetum_arvense_n_uuid = UUID.fromString("96ad7144-a898-4694-94a9-335d0cab9c24");


    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getBaseUri().toString());
    }

    @Test
    public void testPageTitles() throws MalformedURLException {
        NamePage p = new NamePage(driver, getContext(), equisetum_arvense_n_uuid);
        assertEquals(
                "Equisetum arvense | Integration test reference",
                driver.getTitle()
                );
        assertEquals(
                "Equisetum arvense",
                driver.findElement(By.id("page-title")).getText()
                );

    }

    @Test
    public void testNamePage() throws MalformedURLException {
        NamePage p = new NamePage(driver, getContext(), equisetum_arvense_n_uuid);
        List<TypeDesignationElement> typeDesignations = p.getTypeDesignations(0, "div");
        assertEquals("Type: Type specimen which may have beed assigned already: Jonsell & Jarvis, Nord. J. Bot. 14: 148 (1994)1,2", typeDesignations.get(0).getText());
        assertEquals("Type: \"LT: Clayton 341; (BM) LT designated by Jonsell & Jarvis, Nord. J. Bot. 14: 148 (1994)\"", typeDesignations.get(1).getText());
    }

}
