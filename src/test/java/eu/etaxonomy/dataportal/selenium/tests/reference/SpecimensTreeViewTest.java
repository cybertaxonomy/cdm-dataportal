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
import java.util.UUID;

import org.apache.logging.log4j.Level;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.cdm.common.LogUtils;
import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.DrupalVars;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.DerivedUnitTree;
import eu.etaxonomy.dataportal.elements.DerivedUnitTreeNode;
import eu.etaxonomy.dataportal.elements.DetailsTable;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonPage;
import eu.etaxonomy.drush.DrushExecutionFailure;

/**
 * @author a.kohlbecker
 * @since Aug 11, 2020
 */
@DataPortalContexts( { DataPortalSite.reference })
public class SpecimensTreeViewTest extends CdmDataPortalTestBase {

    private static final UUID glenodinium_apiculatum_t = UUID.fromString("d245083e-3bda-435f-9bb3-bdc2249ff23c");

    private TaxonPage p;

    private DerivedUnitTree duTree;

    @Before
    public void switchToView() throws IOException, InterruptedException, DrushExecutionFailure {
        LogUtils.setLevel(getClass(), Level.DEBUG);
        setDrupalVar(DrupalVars.CDM_DATAPORTAL_TAXONPAGE_TABS, "1");
        setDrupalVar(DrupalVars.CDM_SPECIMEN_LIST_VIEW_MODE, "derivate_tree");
        setDrupalVarJson(DrupalVars.CDM_SPECIMEN_DERIVATE_TREE_OPTIONS, "{\"field_unit_short_label\":1}");
        loadPage();
    }

    // must be called after setting the drupal vars
    public void loadPage() throws MalformedURLException {
        p = new TaxonPage(driver, getContext(), glenodinium_apiculatum_t, "specimens");
        WebElement treeElement = p.getDataPortalContent().getElement().findElement(By.cssSelector(".item-tree"));
        duTree = DerivedUnitTree.from(treeElement);
    }

    @Test
    public void testPage() {

        assertEquals(3, duTree.getRootNodes().size());
        BaseElement rootNodeHeader1 = duTree.getRootNodes().get(0).getHeader();
        assertEquals("B SP-99999", rootNodeHeader1.getText());
        BaseElement rootNodeHeader2 = duTree.getRootNodes().get(1).getHeader();
        assertEquals("Germany, Berlin, 2 Apr 1835", rootNodeHeader2.getText());
        BaseElement rootNodeHeader3 = duTree.getRootNodes().get(2).getHeader();
        assertEquals("Ehrenberg, C.G. D047", rootNodeHeader3.getText());
    }

    @Test
    public void testDerivationTree1() {

        DerivedUnitTreeNode rootNode = duTree.getRootNodes().get(0);
        DerivedUnitTreeNode subNode1 = rootNode.getSubNodes().get(0);
        DerivedUnitTreeNode subNode2 = rootNode.getSubNodes().get(1);
        DerivedUnitTreeNode subNode3 = rootNode.getSubNodes().get(2);

        assertEquals("B SP-99999", rootNode.getHeader().getText());
        assertFalse("sub node 1 initially invisible", subNode1.getElement().isDisplayed());
        rootNode.getTreeNodeSymbol().click();
        assertTrue("sub node 1 visible after click", subNode1.getElement().isDisplayed());
        assertEquals("B B-923845", subNode1.getHeader().getText());
        assertEquals("B DNA-9098080", subNode2.getHeader().getText());
        assertEquals("B_SP-99999", subNode3.getHeader().getText());

        // NOTE we are only testing subnode 1 here as all other details are tested in other methods

        // ---- sub node 1
        subNode1.getHeader().getElement().click(); // make the  content visible
        LinkElement pageLink = subNode1.getHeader().getLinksInElement().get(0);
        assertTrue(pageLink.getUrl().endsWith("cdm_dataportal/occurrence/2d424df6-f927-472a-8fb5-4c2d2eeb4484"));

        // THIS THE THE MOST IMORTANT detail to test here
        assertEquals("Preparation: Liebm., Botanic Garden and Botanical Museum Berlin-Dahlem (BGBM), 2020-01-03", subNode1.getDerivationEvent());

        DetailsTable tissueSampleTable = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Tissue Sample"));
        assertEquals("Tissue Sample", tissueSampleTable.getHeaderText());
        assertEquals("fruit", tissueSampleTable.getDetailsValueCellText("Kind of unit"));
        assertEquals("B-923845", tissueSampleTable.getDetailsValueCellText("Accession number"));
        assertEquals("B", tissueSampleTable.getDetailsValueCellText("Collection"));
    }

    @Test
    public void testDerivationTree2() {

        DerivedUnitTreeNode rootNode = duTree.getRootNodes().get(1);
        DerivedUnitTreeNode subNode1 = rootNode.getSubNodes().get(0);

        assertEquals("Germany, Berlin, 2 Apr 1835", rootNode.getHeader().getText());
        assertFalse("sub node 1 initially invisible", subNode1.getElement().isDisplayed());
        rootNode.getTreeNodeSymbol().click();
        assertTrue("sub node 1 visible after click", subNode1.getElement().isDisplayed());
        assertEquals("BHUPM 671", subNode1.getHeader().getText());

        // ---- root node
        rootNode.getHeader().getElement().click(); // make the  content visible
        LinkElement pageLink = rootNode.getHeader().getLinksInElement().get(0);
        assertTrue(pageLink.getUrl().endsWith("cdm_dataportal/occurrence/75b73483-7ee6-4c2c-8826-1e58a0ed18e0"));

        DetailsTable fieldUnitTable = rootNode.getDetailsTable(DetailsTable.tableClassAttrFrom("Field Unit"));
        assertEquals("Field Unit", fieldUnitTable.getHeaderText());
        assertEquals("field note 1", fieldUnitTable.getDetailsValueCellText("Field notes"));

        DetailsTable gatheringTable = rootNode.getDetailsTable(DetailsTable.tableClassAttrFrom("Gathering & Location"));
        assertEquals("Gathering & Location", gatheringTable.getHeaderText());
        assertEquals("Germany", gatheringTable.getDetailsValueCellText("Country"));
        assertEquals("Berlin", gatheringTable.getDetailsValueCellText("Locality"));
        assertEquals("1835-04-02", gatheringTable.getDetailsValueCellText("Date"));

        // ---- sub node 1
        subNode1.getHeader().getElement().click(); // make the  content visible
        pageLink = subNode1.getHeader().getLinksInElement().get(0);
        assertTrue(pageLink.getUrl().endsWith("cdm_dataportal/occurrence/eb729673-5206-49fb-b902-9214d8bdbb51"));

        assertEquals("Gathering in-situ", subNode1.getDerivationEvent());

        DetailsTable imageTable = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Still Image"));
        assertEquals("Still Image", imageTable.getHeaderText());
        assertEquals("Unpublished image", imageTable.getDetailsValueCellText("Kind of unit"));
        assertEquals("671", imageTable.getDetailsValueCellText("Accession number"));
        assertEquals("BHUPM", imageTable.getDetailsValueCellText("Collection"));

        DetailsTable typeDesignationsTable = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Type designations"));
        assertEquals("Lectotype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20171)",
                typeDesignationsTable.getBodyCellText(0, 0));

        DetailsTable mediaTable = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Media"));
    }

    @Test
    public void testDerivationTree3() {

        DerivedUnitTreeNode rootNode = duTree.getRootNodes().get(2);
        DerivedUnitTreeNode subNode1 = rootNode.getSubNodes().get(0);
        DerivedUnitTreeNode subNode1_1 = subNode1.getSubNodes().get(0);
        DerivedUnitTreeNode subNode1_2 = subNode1.getSubNodes().get(1);
        DerivedUnitTreeNode subNode1_3 = subNode1.getSubNodes().get(2);
        DerivedUnitTreeNode subNode1_4 = subNode1.getSubNodes().get(3);
        DerivedUnitTreeNode subNode2 = rootNode.getSubNodes().get(1);

        assertEquals("Ehrenberg, C.G. D047", rootNode.getHeader().getText());

        assertFalse("sub node 1 initially invisible", subNode1.getElement().isDisplayed());
        rootNode.getTreeNodeSymbol().click();
        assertTrue("sub node 1 visible after click", subNode1.getElement().isDisplayed());
        assertEquals("CEDiT 2017E68", subNode1.getHeader().getText());

        assertFalse("sub node 1 initially invisible", subNode1_1.getElement().isDisplayed());
        subNode1.getTreeNodeSymbol().click();
        assertTrue("sub node 1 visible after click", subNode1_1.getElement().isDisplayed());
        assertEquals("10.5555 (JSTOR image viewer)", subNode1_1.getHeader().getText());
        assertEquals("B IMG 99999", subNode1_2.getHeader().getText());
        assertEquals("M M0093531 (Erigeron annus)", subNode1_3.getHeader().getText());
        assertEquals("XKCD MASKS 2X (Masks)", subNode1_4.getHeader().getText());
        assertEquals("M M-0289351", subNode2.getHeader().getText());

        // --- Root note
        rootNode.getHeader().getElement().click(); // make the  content visible
        LinkElement pageLink = rootNode.getHeader().getLinksInElement().get(0);
        assertTrue(pageLink.getUrl().endsWith("cdm_dataportal/occurrence/89d36e79-3e80-4468-986e-411ca391452e"));

        DetailsTable fieldUnitTable = rootNode.getDetailsTable(DetailsTable.tableClassAttrFrom("Field Unit"));
        assertEquals("Field Unit", fieldUnitTable.getHeaderText());
        assertEquals("D047", fieldUnitTable.getDetailsValueCellText("Collecting number"));

        DetailsTable gatheringTable = rootNode.getDetailsTable(DetailsTable.tableClassAttrFrom("Gathering & Location"));
        assertEquals("Gathering & Location", gatheringTable.getHeaderText());
        assertEquals("2016-03-28", gatheringTable.getDetailsValueCellText("Date"));
        assertEquals("Ehrenberg, C.G.", gatheringTable.getDetailsValueCellText("Collector"));
        assertEquals("Berlin", gatheringTable.getDetailsValueCellText("Locality"));
        assertEquals("Germany", gatheringTable.getDetailsValueCellText("Country"));
        assertEquals("52°31'1.2\"N, 13°21'E +/-20 m (WGS84)", gatheringTable.getDetailsValueCellText("Exact location"));
        assertEquals("165 m", gatheringTable.getDetailsValueCellText("Altitude"));

        // --- node 1

        subNode1.getHeader().getElement().click();
        assertTrue(subNode1.getHeader().getLinksInElement().get(0).getUrl().endsWith("cdm_dataportal/occurrence/8585081c-b73b-440b-b349-582845cf3fb4"));

        assertEquals("Gathering in-situ", subNode1.getDerivationEvent());

        DetailsTable preserverdSpecimenTable_1 = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Preserved Specimen"));
        assertEquals("Preserved Specimen", preserverdSpecimenTable_1.getHeaderText());
        assertEquals("Specimen", preserverdSpecimenTable_1.getDetailsValueCellText("Kind of unit"));
        assertEquals("2017E68", preserverdSpecimenTable_1.getDetailsValueCellText("Accession number"));
        assertEquals("destroyed", preserverdSpecimenTable_1.getDetailsValueCellText("Status"));
        assertEquals("http://testid.org/2017E68", preserverdSpecimenTable_1.getDetailsValueCellText("Preferred stable uri"));
        assertEquals("http://testid.org/2017E68", preserverdSpecimenTable_1.getDetailsValueCell("Preferred stable uri").getLinksInElement().get(0).getUrl());
        assertEquals("CEDiT at Botanic Garden and Botanical Museum Berlin-Dahlem (BGBM)", preserverdSpecimenTable_1.getDetailsValueCellText("Collection"));
        assertEquals("Glenodinium apiculatum", preserverdSpecimenTable_1.getDetailsValueCellText("Stored under"));
        assertTrue(preserverdSpecimenTable_1.getDetailsValueCell("Stored under").getLinksInElement().get(0).getUrl().endsWith("cdm_dataportal/name/758a9b10-6817-496b-b5a3-dd66b38c13b0/null/null/"));
        assertEquals("D. Veloper", preserverdSpecimenTable_1.getDetailsValueCellText("Exsiccatum"));
        assertEquals("CE_2017E68", preserverdSpecimenTable_1.getDetailsValueCellText("Catalog number"));
        assertEquals("E2017E68", preserverdSpecimenTable_1.getDetailsValueCellText("Barcode"));

        DetailsTable typeDesignationsTable_1 = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Type designations"));
        assertEquals("Epitype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20171)",
                typeDesignationsTable_1.getBodyCellText(0, 0));

        DetailsTable identificationsTable_1 = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Identification"));
        assertEquals("Glenodinium apiculatum, 2016-12-01, Kohlbecker, A.",
                identificationsTable_1.getBodyCellText(0, 0));
        assertTrue(identificationsTable_1.getBodyCell(0, 0).getLinksInElement().get(0).getUrl().endsWith("cdm_dataportal/taxon/d245083e-3bda-435f-9bb3-bdc2249ff23c/general"));

        // --- --- node_1_2

        subNode1_2.getHeader().getElement().click();
        assertTrue(subNode1_2.getHeader().getLinksInElement().get(0).getUrl().endsWith("cdm_dataportal/occurrence/a825bdad-6854-4868-98f5-7e6ebe3b6271"));

        assertEquals("Accessioning", subNode1_2.getDerivationEvent());

        DetailsTable imageTable_1_2 = subNode1_2.getDetailsTable(DetailsTable.tableClassAttrFrom("Still Image"));
        assertEquals("Still Image", imageTable_1_2.getHeaderText());
        assertEquals("Specimen scan", imageTable_1_2.getDetailsValueCellText("Kind of unit"));
        assertEquals("IMG 99999", imageTable_1_2.getDetailsValueCellText("Accession number"));
        assertEquals("B", imageTable_1_2.getDetailsValueCellText("Collection"));

        DetailsTable mediaTable_1_2 = subNode1_2.getDetailsTable(DetailsTable.tableClassAttrFrom("Media"));
        assertTrue(mediaTable_1_2.getBodyCell(0, 0).getText().contains("Sisymbrium_aegyptiacum_C1.jpg&mo=file"));


        // --- --- node_1_3

        subNode1_3.getHeader().getElement().click();
        assertTrue(subNode1_3.getHeader().getLinksInElement().get(0).getUrl().endsWith("cdm_dataportal/occurrence/04936f1c-41be-47db-99ed-33ed30bd7c01"));

        assertEquals("Accessioning", subNode1_3.getDerivationEvent());

        DetailsTable imageTable_1_3 = subNode1_3.getDetailsTable(DetailsTable.tableClassAttrFrom("Still Image"));
        assertEquals("Still Image", imageTable_1_3.getHeaderText());
        assertEquals("Specimen scan", imageTable_1_3.getDetailsValueCellText("Kind of unit"));
        assertEquals("M0093531", imageTable_1_3.getDetailsValueCellText("Accession number"));
        assertEquals("M", imageTable_1_3.getDetailsValueCellText("Collection"));

        DetailsTable mediaTable_1_3 = subNode1_3.getDetailsTable(DetailsTable.tableClassAttrFrom("Media"));
        assertTrue(mediaTable_1_3.getBodyCell(0, 0).getText().contains("Erigeron annus"));

        // --- --- node_1_4

        subNode1_4.getHeader().getElement().click();
        assertTrue(subNode1_4.getHeader().getLinksInElement().get(0).getUrl().endsWith("cdm_dataportal/occurrence/c2495af1-251b-42e9-b5ab-2e3e0df9ea3f"));

        assertEquals("Accessioning", subNode1_4.getDerivationEvent());

        DetailsTable imageTable_1_4 = subNode1_4.getDetailsTable(DetailsTable.tableClassAttrFrom("Still Image"));
        assertEquals("Still Image", imageTable_1_4.getHeaderText());
        assertEquals("Detail image", imageTable_1_4.getDetailsValueCellText("Kind of unit"));
        assertEquals("MASKS 2X", imageTable_1_4.getDetailsValueCellText("Accession number"));
        assertEquals("XKCD", imageTable_1_4.getDetailsValueCellText("Collection"));

        DetailsTable mediaTable_1_4 = subNode1_4.getDetailsTable(DetailsTable.tableClassAttrFrom("Media"));
        assertTrue(mediaTable_1_4.getBodyCell(0, 0).getText().contains("Ink drawing"));


        // --- node_2

        subNode2.getHeader().getElement().click();
        assertTrue(subNode2.getHeader().getLinksInElement().get(0).getUrl().endsWith("cdm_dataportal/occurrence/e86c5acd-de55-44af-99f7-484207657264"));

        assertEquals("Gathering in-situ", subNode2.getDerivationEvent());

        DetailsTable preserverdSpecimenTable_2 = subNode2.getDetailsTable(DetailsTable.tableClassAttrFrom("Preserved Specimen"));
        assertEquals("Preserved Specimen", preserverdSpecimenTable_2.getHeaderText());
        assertEquals("http://herbarium.bgbm.org/object/B400042045", preserverdSpecimenTable_2.getDetailsValueCellText("Preferred stable uri"));
        assertEquals("Specimen", preserverdSpecimenTable_2.getDetailsValueCellText("Kind of unit"));

        DetailsTable typeDesignationsTable_2 = subNode2.getDetailsTable(DetailsTable.tableClassAttrFrom("Type designations"));
        assertEquals("Isolectotype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20171)",
                typeDesignationsTable_2.getBodyCellText(0, 0));
        assertEquals("expecting one footnote key link", 1, typeDesignationsTable_2.getBodyCell(0, 0).getLinksInElement().size());

    }
}