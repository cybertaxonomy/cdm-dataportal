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



    @Before
    public void switchToView() throws IOException, InterruptedException, DrushExecutionFailure {
        Logger.getLogger(DrushExecuter.class).setLevel(Level.DEBUG);
        setDrupalVar(DrupalVars.CDM_DATAPORTAL_TAXONPAGE_TABS, "1");
        setDrupalVar(DrupalVars.CDM_SPECIMEN_LIST_VIEW_MODE, "derivate_tree");
    }

    @Test
    public void test1() throws MalformedURLException {
        TaxonPage p = new TaxonPage(driver, getContext(), glenodinium_apiculatum_t, "specimens");
        WebElement specimensTable = p.getDataPortalContent().getElement().findElement(By.cssSelector("#specimens table.derivate_tree"));
        List<WebElement> summaryRows = specimensTable.findElements(By.cssSelector("tr.summary_row"));
        List<WebElement> detailRows = specimensTable.findElements(By.cssSelector("tr.detail_row"));
        assertEquals(3, summaryRows.size());
        assertEquals("Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047.", summaryRows.get(0).getText());
        assertEquals("Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047.", summaryRows.get(1).getText());
        assertEquals("Germany, Berlin, 2 Apr 1835.", summaryRows.get(2).getText());

        // -------------------------------------------------------------------------------------------
        // Germany, Berlin, alt. 165 m, 52°31'1.2"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047.
        summaryRows.get(0).click(); // make the row visible

        WebElement descriptionListContainerElement = detailRows.get(0).findElement(By.cssSelector("div.description_list"));
        WebElement derivateTreeContainer = descriptionListContainerElement.findElement(By.xpath("./parent::li"));
        logger.debug("derivateTreeContainer: " + ElementUtils.webElementTagToMarkup(derivateTreeContainer));

        List<WebElement> dls = detailRows.get(0).findElements(By.cssSelector("div.description_list"));
        assertEquals(3, dls.size());
        DescriptionList dl1 = new DescriptionList(dls.get(0).findElement(By.tagName("dl")));
        assertEquals("Field Unit", dl1.joinedDescriptionElementText("Record base:"));
        assertEquals("Ehrenberg, C.G.", dl1.joinedDescriptionElementText("Collector:"));
        assertEquals("Germany", dl1.joinedDescriptionElementText("Country:"));
        assertEquals("Berlin", dl1.joinedDescriptionElementText("Locality:"));
        BaseElement descriptionListContainer = new BaseElement(derivateTreeContainer);
        assertEquals(7, descriptionListContainer.getLinksInElement().size()); // other links in the derivate tree are also found
        // TODO one of the links is a footnote key for which the footnote is missing
        LinkElement link1 = descriptionListContainer.getLinksInElement().get(0);
        assertEquals("Detail page", link1.getText());
        assertTrue(link1.getUrl().endsWith("/cdm_dataportal/occurrence/89d36e79-3e80-4468-986e-411ca391452e"));

        DescriptionList dl2 = new DescriptionList(dls.get(1).findElement(By.tagName("dl")));

        assertEquals("Preserved Specimen", dl2.joinedDescriptionElementText("Record base:"));
        assertEquals("2017E68", dl2.joinedDescriptionElementText("Accession number:"));
        assertEquals("\nCode:\nCEDiT", dl2.joinedDescriptionElementText("Collection"));
        assertEquals("Specimen", dl2.joinedDescriptionElementText("Kind of unit:"));
        assertEquals("Gathering in-situ", dl2.joinedDescriptionElementText("Gathering type:"));
        assertEquals("Epitype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20171): Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047; D. Veloper (CEDiT 2017E68). http://testid.org/2017E68",
                dl2.joinedDescriptionElementText("Specimen type designations:"));
        DescriptionElement specimenTypeDesignation_dd = dl2.getDescriptionGroups().get("Specimen type designations:").get(0);
        List<LinkElement> specimenTypeDesignationLinks = specimenTypeDesignation_dd.getDescriptionElementContent().getLinksInElement();
        assertEquals(3, specimenTypeDesignationLinks.size());
        // TODO one of the links is a footnote key for which the footnote is missing
        assertEquals("CEDiT 2017E68", specimenTypeDesignationLinks.get(1).getText());
        assertTrue(specimenTypeDesignationLinks.get(1).getUrl().endsWith("cdm_dataportal/occurrence/8585081c-b73b-440b-b349-582845cf3fb4"));


        // -------------------------------------------------------------------------------------------
        // Germany, Berlin, alt. 165 m, 52°31'1.2"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047.
        summaryRows.get(1).click(); // make the row visible
        descriptionListContainerElement = detailRows.get(1).findElement(By.cssSelector("div.description_list"));
        derivateTreeContainer = descriptionListContainerElement.findElement(By.xpath("./parent::li"));

        dls = detailRows.get(1).findElements(By.cssSelector("div.description_list"));
        assertEquals(3, dls.size());
        dl1 = new DescriptionList(dls.get(0).findElement(By.tagName("dl")));
        assertEquals("Field Unit", dl1.joinedDescriptionElementText("Record base:"));
        assertEquals("Germany", dl1.joinedDescriptionElementText("Country:"));
        assertEquals("Berlin", dl1.joinedDescriptionElementText("Locality:"));
        descriptionListContainer = new BaseElement(derivateTreeContainer);
        assertEquals(6, descriptionListContainer.getLinksInElement().size()); // other links in the derivate tree are also found
        link1 = descriptionListContainer.getLinksInElement().get(0);
        assertEquals("Detail page", link1.getText());
        assertTrue(link1.getUrl().endsWith("/cdm_dataportal/occurrence/89d36e79-3e80-4468-986e-411ca391452e"));

        dl2 = new DescriptionList(dls.get(1).findElement(By.tagName("dl")));

        assertEquals("Preserved Specimen", dl2.joinedDescriptionElementText("Record base:"));
        assertEquals("M-0289351", dl2.joinedDescriptionElementText("Accession number:"));
        assertEquals("\nCode:\nM", dl2.joinedDescriptionElementText("Collection"));
        assertEquals("Specimen", dl2.joinedDescriptionElementText("Kind of unit:"));
        assertEquals("Gathering in-situ", dl2.joinedDescriptionElementText("Gathering type:"));
        assertEquals("Isolectotype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20172): Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047 (M M-0289351). http://herbarium.bgbm.org/object/B400042045",
                dl2.joinedDescriptionElementText("Specimen type designations:"));
        specimenTypeDesignationLinks = dl2.getDescriptionGroups().get("Specimen type designations:").get(0).getDescriptionElementContent().getLinksInElement();
        assertEquals(3, specimenTypeDesignationLinks.size());
        assertEquals("M M-0289351", specimenTypeDesignationLinks.get(1).getText());
        assertTrue(specimenTypeDesignationLinks.get(1).getUrl().endsWith("cdm_dataportal/occurrence/e86c5acd-de55-44af-99f7-484207657264"));
        assertEquals("http://herbarium.bgbm.org/object/B400042045", specimenTypeDesignationLinks.get(2).getText());
        assertEquals("http://herbarium.bgbm.org/object/B400042045", specimenTypeDesignationLinks.get(2).getUrl().toString());

    }

}
