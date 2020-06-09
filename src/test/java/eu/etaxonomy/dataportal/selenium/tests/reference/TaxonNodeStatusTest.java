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

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.TaxonNodeStatusElement;
import eu.etaxonomy.dataportal.elements.TaxonNodeStatusElement.TaxonNodeStatusData;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonPage;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * Issues to be covered by this TestClass:
 *
 * Test for feature request https://dev.e-taxonomy.eu/redmine/issues/3616
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class TaxonNodeStatusTest extends CdmDataPortalTestBase{

    static final UUID casus_dubius_uuid = UUID.fromString("b6abcec5-d58e-4bf1-87c4-f7926525d1a6");
    static final UUID casus_noninsolor_uuid = UUID.fromString("6e6cc783-f0b6-49e4-b186-ed66bbda4d2a");
    static final UUID casus_admirabilis_uuid = UUID.fromString("1412a6af-c005-4a8a-a07a-036ea4255824");
    static final UUID casus_exosrs_uuid = UUID.fromString("411f37e2-c583-44bb-a40d-21b1eeb92034");

    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getBaseUri().toString());
    }

    @Test
    public void test_casus_dubius() throws MalformedURLException {

        TaxonPage p = new TaxonPage(driver, getContext(), casus_dubius_uuid);
        casus_dubius_assertions(p);
    }

    @Test
    public void test_casus_dubius_synonymy() throws MalformedURLException {

        TaxonPage p = new TaxonSynonymyPage(driver, getContext(), casus_dubius_uuid);
        casus_dubius_assertions(p);
    }

    private void casus_dubius_assertions(TaxonPage p) {

        assertTrue(p.getTaxonNodeStatusContainer().get(0).getText().startsWith("Placement status: "));

        List<TaxonNodeStatusElement> statusElements = p.getTaxonNodeStatus();
        assertEquals(1, statusElements.size());
        TaxonNodeStatusElement statusElement = statusElements.get(0);
        assertEquals(2, statusElement.getTaxonNodeStatus().size());

        TaxonNodeStatusData tnsData_0 = statusElement.getTaxonNodeStatus().get(0);
        assertEquals("TaxonNode", tnsData_0.getTaxonNodeRef().getCdmType());
        assertEquals("402ea023-07be-4335-9274-1c3e30a7df3f", tnsData_0.getTaxonNodeRef().getUuid().toString());
        assertEquals("doubtful", tnsData_0.getStatusText().trim());

        assertEquals("Classification", tnsData_0.getClassificationRef().getCdmType());
        assertEquals("2ab81d37-125d-47e6-8450-6aafd5f4b043", tnsData_0.getClassificationRef().getUuid().toString());
        assertEquals("[My Classification]", tnsData_0.getClassficationText());

        TaxonNodeStatusData tnsData_1 = statusElement.getTaxonNodeStatus().get(1);
        assertEquals("excluded", tnsData_1.getStatusText().trim());
        assertEquals("5b217667-d4f4-4ae7-8ab9-b2ceb599d7d0", tnsData_1.getTaxonNodeRef().getUuid().toString());
        assertEquals("TaxonNode", tnsData_1.getTaxonNodeRef().getCdmType());

        assertEquals("Classification", tnsData_1.getClassificationRef().getCdmType());
        assertEquals("41414d01-34f8-48de-9c2a-7c635167a23e", tnsData_1.getClassificationRef().getUuid().toString());
        assertEquals("[Alternative Classification]", tnsData_1.getClassficationText());
    }

    @Test
    public void test_casus_noninsolor() throws MalformedURLException {

        TaxonPage p = new TaxonPage(driver, getContext(), casus_noninsolor_uuid);

        assertTrue("Expecting sigular", p.getTaxonNodeStatusContainer().get(0).getText().startsWith("Placement status: "));

        List<TaxonNodeStatusElement> statusElements = p.getTaxonNodeStatus();
        assertEquals(1, statusElements.size());
        TaxonNodeStatusElement statusElement = statusElements.get(0);
        assertEquals(1, statusElement.getTaxonNodeStatus().size());

        TaxonNodeStatusData tnsData_0 = statusElement.getTaxonNodeStatus().get(0);
        assertEquals("TaxonNode", tnsData_0.getTaxonNodeRef().getCdmType());
        assertEquals("1d7ce8ab-9335-492b-a079-9f7d84a50cd0", tnsData_0.getTaxonNodeRef().getUuid().toString());
        assertEquals("unplaced", tnsData_0.getStatusText().trim());

        assertNull(tnsData_0.getClassificationRef());
        assertNull(tnsData_0.getClassficationText());
    }

    @Test
    public void test_casus_admirabilis() throws MalformedURLException {

        TaxonPage p = new TaxonPage(driver, getContext(), casus_admirabilis_uuid);

        assertTrue(p.getTaxonNodeStatusContainer().get(0).getText().startsWith("Placement status: "));

        List<TaxonNodeStatusElement> statusElements = p.getTaxonNodeStatus();
        assertEquals(1, statusElements.size());
        TaxonNodeStatusElement statusElement = statusElements.get(0);
        assertEquals(1, statusElement.getTaxonNodeStatus().size());

        TaxonNodeStatusData tnsData_0 = statusElement.getTaxonNodeStatus().get(0);
        assertEquals("excluded", tnsData_0.getStatusText().trim());

        assertNull(tnsData_0.getClassificationRef());
        assertNull(tnsData_0.getClassficationText());
    }

    @Test
    public void test_casus_exsors() throws MalformedURLException {

        TaxonPage p = new TaxonPage(driver, getContext(), casus_exosrs_uuid);

        assertTrue("Expecting sigular", p.getTaxonNodeStatusContainer().get(0).getText().startsWith("Placement status: "));

        List<TaxonNodeStatusElement> statusElements = p.getTaxonNodeStatus();
        assertEquals(1, statusElements.size());
        TaxonNodeStatusElement statusElement = statusElements.get(0);
        assertEquals(1, statusElement.getTaxonNodeStatus().size());

        TaxonNodeStatusData tnsData_0 = statusElement.getTaxonNodeStatus().get(0);
        assertEquals("TaxonNode", tnsData_0.getTaxonNodeRef().getCdmType());
        assertEquals("excluded", tnsData_0.getStatusText().trim());

        assertNull(tnsData_0.getClassificationRef());
        assertNull(tnsData_0.getClassficationText());
    }



}
