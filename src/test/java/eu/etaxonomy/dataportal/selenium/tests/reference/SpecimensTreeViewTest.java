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
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.DescriptionList;
import eu.etaxonomy.dataportal.elements.DescriptionList.DescriptionElement;
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
public class SpecimensTreeViewTest extends CdmDataPortalTestBase {

    public static final Logger logger = Logger.getLogger(DrushExecuter.class);

    private static final UUID glenodinium_apiculatum_t = UUID.fromString("d245083e-3bda-435f-9bb3-bdc2249ff23c");

    private TaxonPage p;

    @Before
    public void switchToView() throws IOException, InterruptedException, DrushExecutionFailure {
        Logger.getLogger(DrushExecuter.class).setLevel(Level.DEBUG);
        setDrupalVar(DrupalVars.CDM_DATAPORTAL_TAXONPAGE_TABS, "1");
        setDrupalVar(DrupalVars.CDM_SPECIMEN_LIST_VIEW_MODE, "derivate_tree");
        loadPage();
    }

    // must be called after setting the drupal vars
    public void loadPage() throws MalformedURLException {
        p = new TaxonPage(driver, getContext(), glenodinium_apiculatum_t, "specimens");
    }

    @Test
    public void testPage() {
        WebElement specimensTable = p.getDataPortalContent().getElement().findElement(By.cssSelector("#specimens table.derivate_tree"));
        List<WebElement> summaryRows = specimensTable.findElements(By.cssSelector("tr.summary_row"));
        assertEquals(3, summaryRows.size());
        assertEquals("(B SP-99999).", summaryRows.get(0).getText());
        assertEquals("Germany, Berlin, 2 Apr 1835.", summaryRows.get(1).getText());
        assertEquals("Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047.", summaryRows.get(2).getText());
    }

    @Test
    public void testFieldUnit1() {

        WebElement specimensTable = p.getDataPortalContent().getElement().findElement(By.cssSelector("#specimens table.derivate_tree"));
        List<WebElement> summaryRows = specimensTable.findElements(By.cssSelector("tr.summary_row"));
        List<WebElement> detailRows = specimensTable.findElements(By.cssSelector("tr.detail_row"));

        WebElement descriptionListContainerElement;
        WebElement derivateTreeContainer;
        DescriptionList dl1, dl2;
        List<WebElement> dls;
        DescriptionElement specimenTypeDesignation_dd;
        BaseElement descriptionListContainer;
        List<LinkElement> specimenTypeDesignationLinks;
        LinkElement link1;
        // -------------------------------------------------------------------------------------------
        // Germany, Berlin, 2 Apr 1835.
        int row = 1;
        summaryRows.get(row).click(); // make the row visible
        descriptionListContainerElement = detailRows.get(row).findElement(By.cssSelector("div.description_list"));
        derivateTreeContainer = descriptionListContainerElement.findElement(By.xpath("./parent::li"));

        dls = detailRows.get(row).findElements(By.cssSelector("div.description_list"));
        assertEquals(2, dls.size());

        dl1 = new DescriptionList(dls.get(0).findElement(By.tagName("dl")));
        assertEquals("Field Unit", dl1.joinedDescriptionElementText("Record base:"));
        assertEquals("Germany", dl1.joinedDescriptionElementText("Country:"));
        assertEquals("Berlin", dl1.joinedDescriptionElementText("Locality:"));
        assertEquals("1835-04-02", dl1.joinedDescriptionElementText("Date:"));
        descriptionListContainer = new BaseElement(derivateTreeContainer);
        assertEquals(3, descriptionListContainer.getLinksInElement().size()); // other links in the derivate tree are also found
        link1 = descriptionListContainer.getLinksInElement().get(0);
        assertEquals("Detail page", link1.getText());
        assertTrue(link1.getUrl().endsWith("cdm_dataportal/occurrence/75b73483-7ee6-4c2c-8826-1e58a0ed18e0"));

        dl2 = new DescriptionList(dls.get(1).findElement(By.tagName("dl")));

        assertEquals("Still Image", dl2.joinedDescriptionElementText("Record base:"));
        assertEquals("671", dl2.joinedDescriptionElementText("Accession number:"));
        assertEquals("BHUPM", dl2.joinedDescriptionElementText("Collection:"));
        assertEquals("Gathering in-situ", dl2.joinedDescriptionElementText("Gathering type:"));
        assertEquals("Unpublished image", dl2.joinedDescriptionElementText("Kind of unit:"));
        assertEquals("Lectotype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20171)",
                dl2.joinedDescriptionElementText("Specimen type designations:"));
        specimenTypeDesignation_dd = dl2.getDescriptionGroups().get("Specimen type designations:").get(0);
        specimenTypeDesignationLinks = specimenTypeDesignation_dd.getLinksInElement();
        // TODO the link is a footnote key for which the footnote is missing
        assertEquals(1, specimenTypeDesignationLinks.size());
        // TODO fix below testing for the last link
//        List<LinkElement> descriptionListContainerLinks = dl2.getLinksInElement();
//        assertEquals("Detail page", descriptionListContainerLinks.get(1).getText());
//        assertTrue( descriptionListContainerLinks.get(1).getUrl().endsWith("cdm_dataportal/occurrence/eb729673-5206-49fb-b902-9214d8bdbb51"));

    }


    @Test
    public void testFieldUnit2() {

        WebElement specimensTable = p.getDataPortalContent().getElement().findElement(By.cssSelector("#specimens table.derivate_tree"));
        List<WebElement> summaryRows = specimensTable.findElements(By.cssSelector("tr.summary_row"));
        List<WebElement> detailRows = specimensTable.findElements(By.cssSelector("tr.detail_row"));

        WebElement descriptionListContainerElement;
        WebElement derivateTreeContainer;
        DescriptionList dl1, dl3, dl2;
        List<WebElement> dls;
        DescriptionElement specimenTypeDesignation_dd;
        BaseElement descriptionListContainer;
        List<LinkElement> specimenTypeDesignationLinks;
        LinkElement link1;
        int row = 2;

        // -------------------------------------------------------------------------------------------
        // Germany, Berlin, alt. 165 m, 52°31'1.2"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047.
        summaryRows.get(row).click(); // make the row visible

        descriptionListContainerElement = detailRows.get(row).findElement(By.cssSelector("div.description_list"));
        derivateTreeContainer = descriptionListContainerElement.findElement(By.xpath("./parent::li"));
        logger.debug("derivateTreeContainer: " + ElementUtils.webElementTagToMarkup(derivateTreeContainer));

        dls = detailRows.get(row).findElements(By.cssSelector("div.description_list"));
        assertEquals(9, dls.size());
        dl1 = new DescriptionList(dls.get(0).findElement(By.tagName("dl")));
        assertEquals("2016-03-28", dl1.joinedDescriptionElementText("Date:"));
        assertEquals("Field Unit", dl1.joinedDescriptionElementText("Record base:"));
        assertEquals("Ehrenberg, C.G.", dl1.joinedDescriptionElementText("Collector:"));
        assertEquals("Germany", dl1.joinedDescriptionElementText("Country:"));
        assertEquals("Berlin", dl1.joinedDescriptionElementText("Locality:"));
        assertEquals("52°31'1.2\"N, 13°21'E +/-20 m (WGS84)", dl1.joinedDescriptionElementText("Exact location:"));
        // TODO test Exact location link
        descriptionListContainer = new BaseElement(derivateTreeContainer);
        assertEquals(24, descriptionListContainer.getLinksInElement().size()); // other links in the derivate tree are also found
        // TODO one of the links is a footnote key for which the footnote is missing
        link1 = descriptionListContainer.getLinksInElement().get(1);
        assertEquals("Detail page", link1.getText());
        assertTrue(link1.getUrl().endsWith("/cdm_dataportal/occurrence/89d36e79-3e80-4468-986e-411ca391452e"));

        dl2 = new DescriptionList(dls.get(3).findElement(By.tagName("dl")));

        assertEquals("Preserved Specimen", dl2.joinedDescriptionElementText("Record base:"));
        assertEquals("2017E68", dl2.joinedDescriptionElementText("Accession number:"));
        assertEquals("CEDiT at Botanic Garden and Botanical Museum Berlin-Dahlem (BGBM)", dl2.joinedDescriptionElementText("Collection:"));
        assertEquals("Specimen", dl2.joinedDescriptionElementText("Kind of unit:"));
        assertEquals("Gathering in-situ", dl2.joinedDescriptionElementText("Gathering type:"));
        assertEquals("Epitype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20172)",
                dl2.joinedDescriptionElementText("Specimen type designations:"));
        specimenTypeDesignation_dd = dl2.getDescriptionGroups().get("Specimen type designations:").get(0);
        specimenTypeDesignationLinks = specimenTypeDesignation_dd.getLinksInElement();
        assertEquals("expecting one footnote key link", 1, specimenTypeDesignationLinks.size());

        dl3 = new DescriptionList(dls.get(6).findElement(By.tagName("dl")));

        assertEquals("Preserved Specimen", dl3.joinedDescriptionElementText("Record base:"));
        assertEquals("M-0289351", dl3.joinedDescriptionElementText("Accession number:"));
        assertEquals("M", dl3.joinedDescriptionElementText("Collection:"));
        assertEquals("Specimen", dl3.joinedDescriptionElementText("Kind of unit:"));
        assertEquals("Gathering in-situ", dl3.joinedDescriptionElementText("Gathering type:"));
        assertEquals("Isolectotype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20173)",
                dl3.joinedDescriptionElementText("Specimen type designations:"));
        specimenTypeDesignationLinks = dl3.getDescriptionGroups().get("Specimen type designations:").get(0).getLinksInElement();
        assertEquals("expecting one footnote key link", 1, specimenTypeDesignationLinks.size());

    }

}
