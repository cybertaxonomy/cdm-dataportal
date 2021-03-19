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
import java.util.stream.Collectors;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;

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

    List<DerivedUnitTree> duTrees;

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
        duTrees = p.getDataPortalContent().getElement().findElements(By.cssSelector(".derived-unit-tree")).stream()
                .map(el -> DerivedUnitTree.from(el))
                .collect(Collectors.toList());
    }

    //@Test
    public void testPage() {

        assertEquals(3, duTrees.size());
        BaseElement rootNodeHeader1 = duTrees.get(0).getRootNode().getHeader();
        assertEquals("(B SP-99999).", rootNodeHeader1.getText());
        BaseElement rootNodeHeader2 = duTrees.get(1).getRootNode().getHeader();
        assertEquals("Germany, Berlin, 2 Apr 1835.", rootNodeHeader2.getText());
        BaseElement rootNodeHeader3 = duTrees.get(2).getRootNode().getHeader();
        assertEquals("Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047.", rootNodeHeader3.getText());
    }


    //@Test
    public void testDerivationTree1() {

        DerivedUnitTree tree1 = duTrees.get(0);
        DerivedUnitTreeNode rootNode = tree1.getRootNode();
        DerivedUnitTreeNode subNode1 = rootNode.getSubNodes().get(0);
        DerivedUnitTreeNode subNode2 = rootNode.getSubNodes().get(1);
        DerivedUnitTreeNode subNode3 = rootNode.getSubNodes().get(2);

        assertEquals("(B SP-99999).", rootNode.getHeader().getText());

        assertEquals("(B B-923845).", subNode1.getHeader().getText());
        assertEquals("(B DNA-9098080).", subNode2.getHeader().getText());
        assertEquals("B_SP-99999.png", subNode3.getHeader().getText());
    }

    //@Test
    public void testDerivationTree2() {

        DerivedUnitTree tree2 = duTrees.get(1);
        DerivedUnitTreeNode rootNode = tree2.getRootNode();
        DerivedUnitTreeNode subNode1 = rootNode.getSubNodes().get(0);

        assertEquals("Germany, Berlin, 2 Apr 1835.", rootNode.getHeader().getText());
        assertEquals("BHUPM 671 (ECdraw671.jpg)", subNode1.getHeader().getText());

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

        // TODO check all links
        // descriptionListContainer = new BaseElement(derivateTreeContainer);
        // assertEquals(7, descriptionListContainer.getLinksInElement().size()); // other links in the derivate tree are also found

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

    }


    @Test
    public void testDerivationTree3() {

        DerivedUnitTree tree3 = duTrees.get(2);
        DerivedUnitTreeNode rootNode = tree3.getRootNode();
        DerivedUnitTreeNode subNode1 = rootNode.getSubNodes().get(0);
        DerivedUnitTreeNode subNode2 = rootNode.getSubNodes().get(1);

        assertEquals("Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047.", rootNode.getHeader().getText());
        assertEquals("Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047; D. Veloper (CEDiT 2017E68).", subNode1.getHeader().getText());
        assertEquals("Germany, Berlin, alt. 165 m, 52°31'1.2\"N, 13°21'E (WGS84), 28 Mar 2016, Ehrenberg D047 (M M-0289351).", subNode2.getHeader().getText());

        // --- Root note
        rootNode.getHeader().getElement().click(); // make the  content visible
        LinkElement pageLink = rootNode.getHeader().getLinksInElement().get(0);
        assertTrue(pageLink.getUrl().endsWith("/cdm_dataportal/occurrence/89d36e79-3e80-4468-986e-411ca391452e"));

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

        //TODO test all links
        // assertEquals(17, descriptionListContainer.getLinksInElement().size()); // other links in the derivate tree are also found
        // TODO one of the links is a footnote key for which the footnote is missing
        // link1 = descriptionListContainer.getLinksInElement().get(1);

        // --- node 1

        subNode1.getHeader().getElement().click();
        assertTrue(subNode1.getHeader().getLinksInElement().get(0).getUrl().endsWith("/cdm_dataportal/occurrence/8585081c-b73b-440b-b349-582845cf3fb4"));

        assertEquals("Gathering in-situ", subNode1.getDerivationEvent());

        DetailsTable preserverdSpecimenTable_1 = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Preserved Specimen"));
        assertEquals("Preserved Specimen", preserverdSpecimenTable_1.getHeaderText());
        assertEquals("Specimen", preserverdSpecimenTable_1.getDetailsValueCellText("Kind of unit"));
        assertEquals("2017E68", preserverdSpecimenTable_1.getDetailsValueCellText("Accession number"));
        assertEquals("http://testid.org/2017E68", preserverdSpecimenTable_1.getDetailsValueCellText("Preferred stable uri"));
        assertEquals("CEDiT at Botanic Garden and Botanical Museum Berlin-Dahlem (BGBM)", preserverdSpecimenTable_1.getDetailsValueCellText("Collection"));
        assertEquals("Glenodinium apiculatum", preserverdSpecimenTable_1.getDetailsValueCellText("Stored under"));
        assertEquals("D. Veloper", preserverdSpecimenTable_1.getDetailsValueCellText("Exsiccatum"));
        assertEquals("CE_2017E68", preserverdSpecimenTable_1.getDetailsValueCellText("Catalog number"));
        assertEquals("E2017E68", preserverdSpecimenTable_1.getDetailsValueCellText("Barcode"));

        DetailsTable typeDesignationsTable_1 = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Type designations"));
        assertEquals("Epitype (designated by Kretschmann, J., Žerdoner ?alasan, A. & Kusber, W.-H. 20171)",
                typeDesignationsTable_1.getBodyCellText(0, 0));

        DetailsTable identificationsTable_1 = subNode1.getDetailsTable(DetailsTable.tableClassAttrFrom("Identification"));
        assertEquals("Glenodinium apiculatum",
                identificationsTable_1.getBodyCellText(0, 0));

        // FIXME:
        // Link is missing!!!
        // specimenTypeDesignation_dd = dl2.getDescriptionGroups().get("Specimen type designations:").get(0);
        // specimenTypeDesignationLinks = specimenTypeDesignation_dd.getLinksInElement();
        // assertEquals("expecting one footnote key link", 1, specimenTypeDesignationLinks.size());

        subNode2.getHeader().getElement().click();
        assertTrue(subNode2.getHeader().getLinksInElement().get(0).getUrl().endsWith("/cdm_dataportal/occurrence/e86c5acd-de55-44af-99f7-484207657264"));

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
