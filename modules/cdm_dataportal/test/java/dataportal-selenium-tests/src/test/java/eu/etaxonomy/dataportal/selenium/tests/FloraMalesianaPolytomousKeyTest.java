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
import org.openqa.selenium.support.PageFactory;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage.KeyLineData;
import eu.etaxonomy.dataportal.pages.PolytomousKeyPage.LinkClass;
import eu.etaxonomy.dataportal.pages.PortalPage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts({ DataPortalContext.floramalesiana })
public class FloraMalesianaPolytomousKeyTest extends CdmDataPortalTestBase {

	/**
	 * see http://dev.e-taxonomy.eu/trac/ticket/2350
	 *
	 * @param WebDriver
	 */
	@Test
	public void key_to_Malaysian_Sapindaceae_Genera() {

		String pageUrlString = getBaseUrl() + "?q=cdm_dataportal/polytomousKey/40cf3253-ce7a-4ad6-9a32-27695c36eb5d";

		driver.get(pageUrlString);

		PolytomousKeyPage p = PageFactory.initElements(driver, PolytomousKeyPage.class);
		PortalPage targetPage;
		KeyLineData keyLineData;

		Assert.assertEquals("KEY I TO THE MALESIAN GENERA (based on vegetative and flower characters) (F. Adema)", p.getTitle());

		// -------- //
		keyLineData = new KeyLineData("0",
				"Trees or shrubs, exceptionally lianas. Leaves simple, unifoliolate, (bi)pinnate or digitate. Inflorescences without basal tendrils",
				LinkClass.nodeLinkToNode, "1");
		targetPage = p.followPolytomousKeyLine(0, keyLineData);
		Assert.assertEquals(p, targetPage);

		// -------- //
		keyLineData = new KeyLineData("0'", "Herbaceous or woody climbers. Leaves biternate. Inflorescences with basal tendrils",
				LinkClass.nodeLinkToTaxon, "Cardiospermum");
		targetPage = p.followPolytomousKeyLine(1, keyLineData);
		Assert.assertEquals(keyLineData.getLinkText(), targetPage.getTitle());
		p.goToInitialPage();

		// -------- //
		keyLineData = new KeyLineData("1", "Leaves simple, unifoliolate, (im)paripinnate or digitate", LinkClass.nodeLinkToNode, "2");
		p.followPolytomousKeyLine(2, keyLineData);
		Assert.assertEquals(p, targetPage);

		// -------- //
		keyLineData = new KeyLineData("1'", "Leaves bipinnate", LinkClass.nodeLinkToTaxon, "Tristiropsis");
		p.followPolytomousKeyLine(3, keyLineData);
		Assert.assertEquals(keyLineData.getLinkText(), targetPage.getTitle());
		p.goToInitialPage();

		// -------- //
		keyLineData = new KeyLineData(
				"116",
				"Leaflets entire or crenulate, lower surface without small glands. Inflorescences axillary, sometimes together pseudoterminal; cymes dense, many-flowered",
				LinkClass.nodeLinkToTaxon, "Synima cordierorum");
		Assert.assertEquals(keyLineData.getLinkText(), targetPage.getTitle());
		p.followPolytomousKeyLine(126, keyLineData);
		p.goToInitialPage();

		// -------- //
		keyLineData = new KeyLineData(
				"116'",
				"Leaflets entire, lower surface usually with small glands. Inflorescences axillary, together mostly pseudoterminal, by the shifting aside and suppression of the terminal bud sometimes seemingly truly terminal; cymes lax, 1- or few-flowered",
				LinkClass.nodeLinkToTaxon, "Trigonachras");
		Assert.assertEquals(keyLineData.getLinkText(), targetPage.getTitle());
		p.followPolytomousKeyLine(127, keyLineData);

	}

}
