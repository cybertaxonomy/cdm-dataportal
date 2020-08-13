/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.selenium.tests.reference;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.DrupalVars;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonPage;
import eu.etaxonomy.drush.DrushExecuter;

/**
 * @author a.kohlbecker
 * @since Aug 11, 2020
 */
@DataPortalContexts( { DataPortalSite.reference })
public class SpecimensTopDownViewTest extends CdmDataPortalTestBase {

    private static final UUID glenodinium_apiculatum_t = UUID.fromString("d245083e-3bda-435f-9bb3-bdc2249ff23c");

    @Before
    public void switchToView() throws IOException, InterruptedException {
        Logger.getLogger(DrushExecuter.class).setLevel(Level.DEBUG);
        setDrupalVar(DrupalVars.CDM_DATAPORTAL_TAXONPAGE_TABS, "1");
        setDrupalVar(DrupalVars.CDM_SPECIMEN_LIST_VIEW_MODE, "derivate_path");
    }

    @Test
    public void test1() throws MalformedURLException {
        TaxonPage p = new TaxonPage(driver, getContext(), glenodinium_apiculatum_t, "specimens");
        WebElement specimensTable = p.getDataPortalContent().getElement().findElement(By.cssSelector("#specimans table.specimens"));
        List<WebElement> rows = specimensTable.findElements(By.tagName("tr"));
        assertEquals(3, rows.size());
        assertEquals("Epitype: Germany, Berlin, 52°31'1.2\"N, 13°21'E, 28.3.2016, D047 (CEDiT 2017E68).", rows.get(0).getText());
        assertEquals("Isolectotype: Germany, Berlin, 52°31'1.2\"N, 13°21'E, 28.3.2016, D047 (M M-0289351).", rows.get(1).getText());
        assertEquals("Lectotype: -title cache generation not implemented-", rows.get(2).getText());




    }

}
