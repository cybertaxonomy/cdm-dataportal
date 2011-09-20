// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cyprus;

import static org.junit.Assert.*;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
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
public class Allium_guttatum_subsp_guttatum_TaxonProfileTest extends CdmDataPortalTestBase{

	static UUID taxonUuid = UUID.fromString("6d04598b-3852-4038-91c9-13c7581b21a6");

	TaxonProfilePage p = null;

	@Before
	public void setUp() throws MalformedURLException {

		p = new TaxonProfilePage(driver, getContext(), taxonUuid);

	}

	@After
	public void tearDown(){
		logger.debug("@After");
	}


	@Test
	public void testPage() {

		assertEquals(getContext().prepareTitle("Allium guttatum subsp. guttatum"), p.getTitle());
		assertNull("Authorship information should be hidden", p.getAuthorInformationText());

		List<LinkElement> primaryTabs = p.getPrimaryTabs();
		assertEquals("Expecting 3 tabs", 3, primaryTabs.size());
		assertEquals("General", primaryTabs.get(0).getText());
		assertEquals("Synonymy", primaryTabs.get(1).getText());
		assertEquals("Images", primaryTabs.get(2).getText());

		ImgElement profileImage = p.getProfileImage();
		assertNotNull("Expecting profile images to be switched on", profileImage);
//		assertEquals("http://media.bgbm.org/erez/erez?src=EditWP6/zypern/photos/Allium_guttatum_guttatum_A1.jpg", profileImage.getUrl().toString());
//		assertEquals(400, profileImage.getDimension().getHeight(), 0.5);
//		assertEquals(250, profileImage.getDimension().getWidth(), 0.5);

		assertEquals("Content", p.getTableOfContentHeader());
		List<LinkElement> links = p.getTableOfContentLinks();
		assertNotNull("Expecting a list of TOC links in the profile page.", links);
		p.testTableOfContentEntry(0, "Status", "status");
		p.testTableOfContentEntry(1, "Endemism", "endemism");
		p.testTableOfContentEntry(2, "Red Data Book category", "red_data_book_category");
		p.testTableOfContentEntry(3, "Systematics", "systematics");
		p.testTableOfContentEntry(4, "Distribution", "distribution");

		FeatureBlock featureBlock;

		featureBlock = p.getFeatureBlockAt(0, "status", "div", "div");
		assertEquals("Status\nIndigenous (IN)", featureBlock.getText());

		featureBlock = p.getFeatureBlockAt(2, "endemism", "div", "div");
		assertEquals("Endemism\nnot endemic", featureBlock.getText());

		featureBlock = p.getFeatureBlockAt(2, "red_data_book_category", "div", "div");
		assertEquals("Red Data Book category\nData deficient (DD)", featureBlock.getText());

		//FIXME
//		featureBlock = p.getFeatureBlockAt(3, "systematics", "div", null);
//		assertEquals("Systematics\nTaxonomy and nomenclature follow Mathew (1996).\nMathew B. 1996: A review of Allium section Allium . - Kew.", featureBlock.getText());

		featureBlock = p.getFeatureBlockAt(4, "distribution", "div", "span");
		assertEquals("Distribution\nDivision 21,2\n1. R. D. Meikle, Flora of Cyprus 1. 1977, 2. R. Hand, Supplementary notes to the flora of Cyprus VI. in Willdenowia 39. 2009", featureBlock.getText());
		assertEquals("Distribution", featureBlock.getHeader());
		assertEquals("expecting two footnote keys", 2, featureBlock.getFootNoteKeys().size());

		LinkElement footNoteKey_1 = featureBlock.getFootNoteKeys().get(0);
		BaseElement footNote_1 = featureBlock.getFootNotes().get(0);
		assertTrue("expecting one footnote 0 to be the footnote for key 0",footNote_1.getText().startsWith(footNoteKey_1.getText()));

		p.hover(footNoteKey_1.getElement());
		assertEquals("#ffff00", footNoteKey_1.getElement().getCssValue("background-color"));
		assertEquals("#ffff00", footNote_1.getElement().getCssValue("background-color"));

		assertEquals("1. R. D. Meikle, Flora of Cyprus 1. 1977", footNote_1.getText());

		WebElement distributionMapImage = featureBlock.getElement().findElement(By.className("distribution_map"));
//		assertEquals("http://edit.br.fgov.be/edit_wp5/v1.1/rest_gen.php?title=a:indigenous&ad=cyprusdivs:bdcode:a:2&as=z:ffffff,606060,,|y:1874CD,,|a:339966,,0.1,&l=background_gis:y,cyprusdivs:z&ms=500,380&bbox=32,34,35,36&label=1&img=true&legend=1&mlp=3&mc_s=Georgia,15,blue&mc=&recalculate=false", distributionMapImage.getAttribute("src"));

	}

}
