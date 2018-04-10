/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cyprus;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.TaxonListElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.GenericPortalPage;
import eu.etaxonomy.dataportal.pages.TaxonSearchResultPage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.cyprus })
public class CyprusSearchTest extends CdmDataPortalTestBase{

	private GenericPortalPage homePage;

	@Before
	public void setUp() throws Exception {

		driver.get(getContext().getBaseUri().toString());
		homePage = new GenericPortalPage(driver, getContext());

	}

	/**
	 * see http://dev.e-taxonomy.eu/trac/ticket/2350
	 */
	@Test
	public void searchResultsWithoutAnnotationFootnotes() throws Exception{

		TaxonSearchResultPage searchResultPage = homePage.submitQuery("Genis*");

		for (TaxonListElement item : searchResultPage.getResultItems()){
			 assertNull("result set entries must not have footnote keys", item.getElement().findElements(By.className("footnote-key")));
		}

	}

}
