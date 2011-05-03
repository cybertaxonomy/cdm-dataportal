// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy 
 * http://www.e-taxonomy.eu
 * 
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests;

import org.junit.Assert;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DataPortalContexts;
import eu.etaxonomy.dataportal.selenium.CdmDataPortalTestBase;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cyprus })
public class CyprusSearchTest extends CdmDataPortalTestBase{
	
	/**
	 * @param context
	 */
	public CyprusSearchTest(DataPortalContext context) {
		super(context);
	}

	/**
	 * see http://dev.e-taxonomy.eu/trac/ticket/2350
	 */
	@Test
	public void searchResultsWithoutAnnotationFootnotes(){
		driver.get(getBaseUrl() + "?query=Genis*&search[tree]=0c2b5d25-7b15-4401-8b51-dd4be0ee5cab&q=cdm_dataportal%2Fsearch%2Ftaxon&search[pageSize]=25&search[pageNumber]=0&search[doTaxa]=1&search[doSynonyms]=1&search[doTaxaByCommonNames]=0");
		RenderedWebElement element = (RenderedWebElement)driver.findElement(By.xpath("/html/body/div/div/div[2]/div[2]/div/div/div/ul/li/span[@class='footnote-key footnote-key-1 member-of-footnotes--annotations']"));
		Assert.assertNull("result set entries must not have footnote keys", element);
	}

}
