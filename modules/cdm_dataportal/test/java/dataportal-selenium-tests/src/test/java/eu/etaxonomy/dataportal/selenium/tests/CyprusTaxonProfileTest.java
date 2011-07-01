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

import java.util.List;

import org.junit.Assert;
import org.junit.Test;
import org.openqa.selenium.support.PageFactory;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.ImgElement;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cyprus })
public class CyprusTaxonProfileTest extends CdmDataPortalTestBase{

	@Test
	public void allium_guttatum_subsp_guttatum(){

		String pageUrlString = getBaseUrl() + "?q=cdm_dataportal/taxon/6d04598b-3852-4038-91c9-13c7581b21a6";
		driver.get(pageUrlString);
		TaxonProfilePage p = PageFactory.initElements(driver, TaxonProfilePage.class);

		Assert.assertEquals(prepareTitle("Allium guttatum subsp. guttatum"), p.getTitle());

		List<LinkElement> primaryTabs = p.getPrimaryTabs();
		Assert.assertEquals("Expecting 3 tabs", 3, primaryTabs.size());
		Assert.assertEquals("General", primaryTabs.get(0).getText());
		Assert.assertEquals("Synonymy", primaryTabs.get(1).getText());
		Assert.assertEquals("Images", primaryTabs.get(2).getText());

		ImgElement profileImage = p.getProfileImage();
		Assert.assertNotNull("Expecting profile images to be switched on", profileImage);
		Assert.assertEquals("http://media.bgbm.org/erez/erez?src=EditWP6/zypern/photos/Allium_guttatum_guttatum_A1.jpg", profileImage.getUrl().toString());
		Assert.assertEquals(400, profileImage.getDimension().getHeight(), 0.5);
		Assert.assertEquals(250, profileImage.getDimension().getWidth(), 0.5);


	}

}
