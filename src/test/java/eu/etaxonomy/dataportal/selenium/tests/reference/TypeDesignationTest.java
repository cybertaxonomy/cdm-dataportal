/**
 * Copyright (C) 2024 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.reference;

import java.util.List;
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.DrupalVars;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.TypeDesignationElement;
import eu.etaxonomy.dataportal.elements.TypeDesignationType;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 *
 * @author k.luther
 */
@DataPortalContexts( { DataPortalSite.reference })
public class TypeDesignationTest extends CdmDataPortalTestBase{

    private static final UUID typinium_Uuid = UUID.fromString("037eee62-ac1f-45eb-b4f2-ed9b77e472f1");

    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }

    @Test
    public void testTypeDesignationWithMultiAuthorCitation() throws Exception {
       TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), typinium_Uuid);
       assertEquals("Typinium Mill.", p.getAcceptedNameText());
       WebElement typeDesignationsContainer;
       List<TypeDesignationElement> typeDesignations;
       if (getDrupalVar(DrupalVars.CDM_DTO_PORTAL_PAGE).toString().equals("1")){
         typeDesignationsContainer = p.getNewHomotypicalGroupTypeDesignations();
         assertEquals("Lectotype (designated by Heidari & al. 1986: 271):", typeDesignationsContainer.getText());
       }else{
         typeDesignations = p.getHomotypicalGroupTypeDesignations();
         assertEquals(TypeDesignationType.nameTypeDesignation, typeDesignations.get(0).getTypeDesignationType());
         assertNotNull("Expecting at least one Typedesignation", typeDesignations);
       }

       List<BaseElement> footnotes = p.getHomotypicalGroupFootNotes();
       assertEquals(1, footnotes.size());
       assertEquals("1. Heidari, F., Zima, J., Riahi, H. & Hauer, T. 1986: Multiauthor", footnotes.get(0).getText());
    }
}