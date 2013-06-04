// $Id$
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.cichorieae;

import static org.junit.Assert.*;
import static org.junit.Assert.assertEquals;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;
import eu.etaxonomy.dataportal.pages.TaxonSynonymyPage;

/**
 *
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalContext.cichorieae })
public class Cichorieae_TechnicalAnnnotationsTest extends CdmDataPortalTestBase{


	static UUID soroseris_hookeriana_uuid = UUID.fromString("adaabef1-02f9-41a4-8a39-bf13564559f7");

	static UUID pilosella_uuid = UUID.fromString("f42a07d1-d959-4838-b8ea-192b523ad5cc");


	@Test
	public void soroseris_hookeriana() throws MalformedURLException {
		TaxonProfilePage p = new TaxonProfilePage(driver, getContext(), soroseris_hookeriana_uuid);
		String expectedName = "Soroseris hookeriana";
		assertEquals(getContext().prepareTitle(expectedName), p.getTitle());

		List<LinkElement> tocLinks = p.getTableOfContentLinks();
		// Description mus not be included !!!
		assertEquals("Distribution", tocLinks.get(0).getText());
		assertEquals("Credits", tocLinks.get(1).getText());

		// Credits contains an technical annotation but this mus not be displayed
		FeatureBlock creditsBlock = p.getFeatureBlockAt(1, "credits", "div", "div");
		//testing this is not possible due to SCHROTT-CODE // assertEquals("expecting 1 DescriptionElements in citation", 1, creditsBlock.getDescriptionElements().size());
		assertEquals("Credits\nBoufford D. E. 2009: Images (12 added)\nSmalla M. 2009: Images (1 added)\nSun H. 2009: Images (3 added)\nYue J. 2009: Images (1 added)\nZhang J. 2009: Images (1 added).", creditsBlock.getText());
		assertEquals("expecting no footnoteKeys", 0, creditsBlock.getFootNoteKeys().size());
		assertEquals("expecting no footnotes", 0, creditsBlock.getFootNotes().size());
	}

	@Test
	public void pilosella() throws MalformedURLException {
		TaxonSynonymyPage p = new TaxonSynonymyPage(driver, getContext(), pilosella_uuid);
		String expectedName = "Pilosella";
		assertEquals(getContext().prepareTitle(expectedName), p.getTitle());

		assertEquals("Pilosella Vaill. in Königl. Akad. Wiss. Paris Phys. Abh. 5: 703. 1754", p.getAcceptedNameText());
		assertEquals("Expecting one footnote key", 1, p.getAcceptedNameFootNoteKeys().size());
		assertEquals("Expecting one footnote", 1, p.getHomotypicalGroupFootNotes().size());
		String expectetToStartWith = "1. As has been discovered by Greuter & al. in Taxon 54: 166 (2005),";
		assertTrue("Expecting footnote to start with: '" + expectetToStartWith + "' but was '" + p.getHomotypicalGroupFootNotes().get(0).getText() + "'"
				, p.getHomotypicalGroupFootNotes().get(0).getText().startsWith(expectetToStartWith));
	}

}
