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

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.Assert;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.testutils.PolytomousKeyTestTool;
import eu.etaxonomy.dataportal.testutils.PolytomousKeyTestTool.KeyLineData;
import eu.etaxonomy.dataportal.testutils.PolytomousKeyTestTool.LinkClass;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.floramalesiana })
public class FloraMalesianaPolytomousKeyTest extends CdmDataPortalTestBase{

	/**
	 * see http://dev.e-taxonomy.eu/trac/ticket/2350
	 */
	@Test
	public void key_to_Malaysian_Sapindaceae_Genera(){
		driver.get(getBaseUrl() + "?q=cdm_dataportal/polytomousKey/40cf3253-ce7a-4ad6-9a32-27695c36eb5d");

		// Page title
		RenderedWebElement element = (RenderedWebElement)driver.findElement(By.xpath(".//*[@id='squeeze']/div/div/h2"));
		Assert.assertEquals("KEY I TO THE MALESIAN GENERA (based on vegetative and flower characters) (F. Adema)", element.getText());

		List<WebElement> keyTableList = driver.findElements(By.className("polytomousKey"));
		Assert.assertEquals("Only one polytomousKey table expected", 1, keyTableList.size());
		WebElement keyTable = keyTableList.get(0);

		List<WebElement> tableRows = keyTable.findElements(By.xpath("tbody/tr"));

		Map<Integer, PolytomousKeyTestTool.KeyLineData> keyLineDataMap = new HashMap<Integer, PolytomousKeyTestTool.KeyLineData>();

		keyLineDataMap.put(0, new KeyLineData(
				"0",
				"Trees or shrubs, exceptionally lianas. Leaves simple, unifoliolate, (bi)pinnate or digitate. Inflorescences without basal tendrils",
				LinkClass.nodeLinkToNode,
				"1"));

		keyLineDataMap.put(1, new KeyLineData("0'",
				"Herbaceous or woody climbers. Leaves biternate. Inflorescences with basal tendrils",
				LinkClass.nodeLinkToTaxon,
				"Cardiospermum"));

		keyLineDataMap.put(2, new KeyLineData("1",
				"Leaves simple, unifoliolate, (im)paripinnate or digitate",
				LinkClass.nodeLinkToNode,
				"2"));

		keyLineDataMap.put(3, new KeyLineData("1'",
				"Leaves bipinnate",
				LinkClass.nodeLinkToTaxon,
				"Tristiropsis"));

		keyLineDataMap.put(126, new KeyLineData("116",
				"Leaflets entire or crenulate, lower surface without small glands. Inflorescences axillary, sometimes together pseudoterminal; cymes dense, many-flowered",
				LinkClass.nodeLinkToTaxon,
				"Synima cordierorum"));

		keyLineDataMap.put(127, new KeyLineData("116'",
				"Leaflets entire, lower surface usually with small glands. Inflorescences axillary, together mostly pseudoterminal, by the shifting aside and suppression of the terminal bud sometimes seemingly truly terminal; cymes lax, 1- or few-flowered",
				LinkClass.nodeLinkToTaxon,
				"Trigonachras"));

		PolytomousKeyTestTool tester = new PolytomousKeyTestTool(tableRows, keyLineDataMap);
		tester.runTest();
	}


}
