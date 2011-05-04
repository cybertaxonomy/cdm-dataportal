// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium;

import junit.framework.Assert;

import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.DataPortalContexts;

@SuppressWarnings("deprecation")
@DataPortalContexts( { DataPortalContext.cichorieae })
public class ExampleTest extends CdmDataPortalSeleniumRCTestBase {

	@Test
	public void testSearchLCommunis() throws Exception {
		driver.get(getBaseUrl()
						+ "?query=Lapsana+com*&search[tree]=534e190f-3339-49ba-95d9-fa27d5493e3e&q=cdm_dataportal%2Fsearch%2Ftaxon&search[pageSize]=25&search[pageNumber]=0&search[doTaxa]=1&search[doSynonyms]=1&search[doTaxaByCommonNames]=0");
		WebElement taxonElement = driver.findElement(By
				.xpath("/html/body/div/div/div[2]/div[2]/div/div/div/ul/li/span[@ref='/name/f280f79f-5903-47b0-8352-53e4204c6cf1']"));

		WebElement nameElement = taxonElement.findElement(By.className("BotanicalName"));

		RenderedWebElement namePart1 = (RenderedWebElement) nameElement.findElement(By.xpath("span[1]"));
		Assert.assertEquals("Lapsana", namePart1.getText());
		Assert.assertEquals("italic", namePart1.getValueOfCssProperty("font-style"));

		RenderedWebElement namePart2 = (RenderedWebElement) nameElement.findElement(By.xpath("span[2]"));
		Assert.assertEquals("communis", namePart2.getText());
		Assert.assertEquals("italic", namePart2.getValueOfCssProperty("font-style"));

		RenderedWebElement authorPart = (RenderedWebElement) nameElement.findElement(By.xpath("span[3]"));
		Assert.assertEquals("L.", authorPart.getText());
		Assert.assertEquals("normal", authorPart.getValueOfCssProperty("font-style"));

		RenderedWebElement referenceElement = (RenderedWebElement) taxonElement.findElement(By.className("reference"));
		Assert.assertEquals("Sp. Pl.: 811. 1753", referenceElement.findElement((By.className("reference"))).getText());
	}

	/**
	 * This test emulates the Selenium RC API
	 *
	 * @throws Exception
	 */
	@Test
	public void testSearchLCommunisUsingSeleniumRC() throws Exception {
		selenium.open("?query=Lapsana+com*&search[tree]=534e190f-3339-49ba-95d9-fa27d5493e3e&q=cdm_dataportal%2Fsearch%2Ftaxon&search[pageSize]=25&search[pageNumber]=0&search[doTaxa]=1&search[doSynonyms]=1&search[doTaxaByCommonNames]=0");
		selenium.isTextPresent("Lapsana");
		selenium.isTextPresent("communis");
		selenium.isTextPresent("L.");
		selenium.isTextPresent("Sp. Pl.: 811. 1753");
	}

}
