/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.reference;

import java.util.UUID;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 * Issues to be covered by this TestClass:
 *
 * #9265
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.reference })
public class NomenclaturalSourceTest extends CdmDataPortalTestBase{

    static final UUID taxon_nepenthes_gracilis_uuid = UUID.fromString("5d9af5a8-c8ad-45e8-85df-ce6a01011fb9");

    TaxonSynonymyPage p = null;

    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
        if(p == null) {
            p = new TaxonSynonymyPage(driver, getContext(), taxon_nepenthes_gracilis_uuid);
        }
    }

    @Test
    public void testNomenclaturalReference() {

        WebElement acceptedName = p.getAcceptedName();
        assertEquals("Nepenthes gracilis Korth., Verh. Nat. Gesch. Ned. Bezitt., Bot.: 22, t. 1 & 4", acceptedName.getText());
        WebElement referenceElement = acceptedName.findElement(By.cssSelector(".reference"));
        assertEquals("Verh. Nat. Gesch. Ned. Bezitt., Bot.: 22, t. 1 & 4", referenceElement.getText());
    }

    @Test
    public void testProtologueAcceptedName() {

        WebElement acceptedName = p.getAcceptedName();
        BaseElement protologueElement = new BaseElement(acceptedName.findElement(By.cssSelector(".protologue")));
        assertEquals(1, protologueElement.getLinksInElement().size());
        assertEquals("http://media.test/protologue/1234", protologueElement.getLinksInElement().get(0).getUrl());
    }

    @Test
    public void testProtologueHeterotypicSynonym() {

        WebElement synonym = p.getHeterotypicalGroupSynonym(1, 1);
        BaseElement protologueElement = new BaseElement(synonym.findElement(By.cssSelector(".protologue")));
        assertEquals(1, protologueElement.getLinksInElement().size());
        assertEquals("http://media.test/protologue/5678", protologueElement.getLinksInElement().get(0).getUrl());
    }

    @Test
    public void testProtologueHopmotypicSynonym() {

        WebElement synonym = p.getHeterotypicalGroupSynonym(1, 2);
        BaseElement protologueElement = new BaseElement(synonym.findElement(By.cssSelector(".protologue")));
        assertEquals(1, protologueElement.getLinksInElement().size());
        assertEquals("http://media.test/98765", protologueElement.getLinksInElement().get(0).getUrl());
    }

}
