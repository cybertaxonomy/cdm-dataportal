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
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.DescriptionList;
import eu.etaxonomy.dataportal.elements.Dynabox;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonPage;
import eu.etaxonomy.drush.DrushExecuter;
import eu.etaxonomy.drush.DrushExecutionFailure;

/**
 * @author a.kohlbecker
 * @since Aug 11, 2020
 */
@DataPortalContexts( { DataPortalSite.reference })
public class SpecimensTopDownViewTest extends CdmDataPortalTestBase {

    private static final UUID glenodinium_apiculatum_t = UUID.fromString("d245083e-3bda-435f-9bb3-bdc2249ff23c");

    @Before
    public void switchToView() throws IOException, InterruptedException, DrushExecutionFailure {
        Logger.getLogger(DrushExecuter.class).setLevel(Level.DEBUG);
        setDrupalVar(DrupalVars.CDM_DATAPORTAL_TAXONPAGE_TABS, "1");
        setDrupalVar(DrupalVars.CDM_SPECIMEN_LIST_VIEW_MODE, "derivate_path");
    }

    @Test
    public void test1() throws MalformedURLException {
        TaxonPage p = new TaxonPage(driver, getContext(), glenodinium_apiculatum_t, "specimens");
        WebElement specimensTable = p.getDataPortalContent().getElement().findElement(By.cssSelector("#specimens table.specimens"));
        List<WebElement> rows = specimensTable.findElements(By.xpath("./tbody/tr"));
        assertEquals(4, rows.size());
        assertEquals("Epitype: Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047; D. Veloper (CEDiT 2017E68).\nopen media", rows.get(0).getText());
        assertEquals("Isolectotype: Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047 (M M-0289351).\nopen media", rows.get(1).getText());
        assertEquals("Lectotype: BHUPM 671", rows.get(2).getText());

        Dynabox dynabox1 = new Dynabox(rows.get(0).findElement(By.className("dynabox")), driver);
        BaseElement dynaboxContent = dynabox1.click();
        List<WebElement> contentElements = dynaboxContent.getElement().findElements(By.xpath("./child::div"));
        assertEquals(3, contentElements.size());
        // 1
        DescriptionList dl1 = new DescriptionList(contentElements.get(0).findElement(By.tagName("dl")));
        assertEquals("Preserved Specimen", dl1.joinedDescriptionElementText("Record basis:"));
        assertEquals("Specimen", dl1.joinedDescriptionElementText("Kind of unit:"));
        assertEquals("CEDiT at Botanic Garden and Botanical Museum Berlin-Dahlem (BGBM)", dl1.joinedDescriptionElementText("Collection:"));
        assertEquals("2017E68", dl1.joinedDescriptionElementText("Most significant identifier:"));
        assertEquals("2017E68", dl1.joinedDescriptionElementText("Accession number:"));
        assertEquals("Epitype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20172)",
                dl1.joinedDescriptionElementText("Specimen type designations:"));
        // 2
        assertEquals("gathering in-situ from:", contentElements.get(1).getText());
        // 3
        DescriptionList dl3 = new DescriptionList(contentElements.get(2).findElement(By.tagName("dl")));
        assertEquals("Field Unit", dl3.joinedDescriptionElementText("Record basis:"));
        assertEquals("D047", dl3.joinedDescriptionElementText("Collecting number:"));
        assertEquals("2016-03-28", dl3.joinedDescriptionElementText("Gathering date"));
        assertEquals("Germany", dl3.joinedDescriptionElementText("Country:"));
        assertEquals("Berlin", dl3.joinedDescriptionElementText("Locality:"));
        assertEquals("52°31'1.2\"N, 13°21'E +/-20 m (WGS84)", dl3.joinedDescriptionElementText("Exact location:"));


    }

}
