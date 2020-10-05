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
import java.util.List;
import java.util.Optional;
import java.util.UUID;

import org.apache.commons.lang3.StringUtils;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.DrupalVars;
import eu.etaxonomy.dataportal.elements.LinkElement;
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
public class SpecimenTableViewTest extends CdmDataPortalTestBase {

    /**
     *
     */
    private static final String DETAIL_IMAGE_DERIVATE_ICON = "cdm_dataportal/images/detail_image_derivate-16x16-32.png";

    /**
     *
     */
    private static final String STEP_DONE_ICON = "modules/cdm_dataportal/images/step_done.gif";

    public static final Logger logger = Logger.getLogger(DrushExecuter.class);

    private static final UUID glenodinium_apiculatum_t = UUID.fromString("d245083e-3bda-435f-9bb3-bdc2249ff23c");

    TaxonPage p = null;

    @Before
    public void switchToView() throws IOException, InterruptedException, DrushExecutionFailure {
        Logger.getLogger(DrushExecuter.class).setLevel(Level.DEBUG);
        setDrupalVar(DrupalVars.CDM_DATAPORTAL_TAXONPAGE_TABS, "1");
        setDrupalVar(DrupalVars.CDM_SPECIMEN_LIST_VIEW_MODE, "derivate_table");
        p = new TaxonPage(driver, getContext(), glenodinium_apiculatum_t, "specimens");
    }

    @Test
    public void testPageTitle()  {
        assertEquals(getContext().prepareTitle("Glenodinium apiculatum"), p.getTitle());
    }

    // @Test no point testing for the tab, the reference portal is set up being tab-less
    public void testPageTab()  {
        Optional<LinkElement> activeTab = p.getActivePrimaryTab();
        assertTrue(activeTab.isPresent());
        assertEquals("Specimens\n(active tab)", activeTab.get().getText());
    }

    @Test
    public void testTableHaders() {

        WebElement specimensTable = p.getDataPortalContent().getElement().findElement(By.cssSelector("#derivate_hierarchy_table"));
        List<WebElement> headerRows = specimensTable.findElements(By.cssSelector("tr th"));
        // Country  Date    Collector + collecting number   Herbaria    Type    Scan    Derivatives
        // first row is empty, this is the "expand row"
        assertEquals("Country", headerRows.get(1).getText());
        assertEquals("Date", headerRows.get(2).getText());
        assertEquals("Collector + collecting number", headerRows.get(3).getText());
        assertEquals("Herbaria", headerRows.get(4).getText());
        assertEquals("Type", headerRows.get(5).getText());
        assertEquals("Scan", headerRows.get(6).getText());
        assertEquals("Derivatives", headerRows.get(7).getText());

    }

    @Test
    public void testTable() {

        WebElement specimensTable = p.getDataPortalContent().getElement().findElement(By.cssSelector("#derivate_hierarchy_table"));

        List<WebElement> rows = specimensTable.findElements(By.cssSelector("tr"));
        assertEquals(5, rows.size());
        // expected rows:
        // 0: header row
        // 1: summary row
        // 2: detail row
        // 3: summary row
        // 4: detail row

        // summary row 1
        int rowId = 1;
        assertTrue(rows.get(rowId).getAttribute("class").contains("summary_row"));
        List<WebElement> cells = rows.get(rowId).findElements(By.tagName("td"));
        assertEquals("Germany", cells.get(1).getText());
        assertEquals("1835-04-02", cells.get(2).getText());
        assertEquals("", cells.get(3).getText());
        // all other empty, TODO check if this is correct or if some data is being missed here

        // details row 1
        rowId = 2;
        assertTrue(rows.get(rowId).getAttribute("class").contains("detail_row"));
        assertEquals("Should be initially invisible", "none", rows.get(rowId).getCssValue("display"));
        rows.get(rowId - 1).click();
        assertEquals("The click should make it visible", "table-row", rows.get(rowId).getCssValue("display"));
        cells = rows.get(rowId).findElements(By.tagName("td"));
        String detailsText = cells.get(1).getText();
        String[] detailsLines = StringUtils.split(detailsText, "\n");
        assertEquals(1, detailsLines.length);
        assertEquals("Citation: Germany, Berlin, 2 Apr 1835. (BHUPM 671)", detailsLines[0]);

        // summary row 1
        rowId = 3;
        assertTrue(rows.get(rowId).getAttribute("class").contains("summary_row"));
        cells = rows.get(rowId).findElements(By.tagName("td"));
        assertEquals("Germany", cells.get(1).getText());
        assertEquals("2016-03-28", cells.get(2).getText());
        assertEquals("Ehrenberg, C.G. D047", cells.get(3).getText());
        assertEquals("CEDiT, M", cells.get(4).getText());
        assertTrue(cells.get(5).findElement(By.tagName("img")).getAttribute("src")
                .endsWith(STEP_DONE_ICON));
        assertTrue(cells.get(6).findElement(By.tagName("img")).getAttribute("src")
                .endsWith(STEP_DONE_ICON));
        assertTrue(cells.get(7).findElement(By.tagName("img")).getAttribute("src")
                .endsWith(DETAIL_IMAGE_DERIVATE_ICON));


        // details row 1
        rowId = 4;
        assertTrue(rows.get(rowId).getAttribute("class").contains("detail_row"));
        assertEquals("Should be initially invisible", "none", rows.get(rowId).getCssValue("display"));
        rows.get(rowId - 1).click();
        assertEquals("The click should make it visible", "table-row", rows.get(rowId).getCssValue("display"));
        cells = rows.get(rowId).findElements(By.tagName("td"));
        detailsText = cells.get(1).getText();
        detailsLines = StringUtils.split(detailsText, "\n");
        assertEquals(7, detailsLines.length);
        assertEquals("Citation: Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047. (M M-0289351, CEDiT 2017E68)", detailsLines[0]);

        assertEquals("Specimen summary: CEDiT (2017E68)", detailsLines[1]);
        assertEquals("Epitype of Glenodinium apiculatum Ehrenb. Specimen Scans: CEDiT (2017E68)", detailsLines[2]);
        assertEquals("Detail Images: masks_2x.png", detailsLines[3]);

        assertEquals("Specimen summary: M (M-0289351)", detailsLines[4]);
        assertEquals("Preferred stable URI: http://herbarium.bgbm.org/object/B400042045", detailsLines[5]);
        assertEquals("Isolectotype of Glenodinium apiculatum Ehrenb.", detailsLines[6]);

    }

}
