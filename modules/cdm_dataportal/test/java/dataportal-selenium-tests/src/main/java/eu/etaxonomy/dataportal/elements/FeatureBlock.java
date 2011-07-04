// $Id$
/**
* Copyright (C) 2011 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.ArrayList;
import java.util.List;

import static junit.framework.Assert.*;

import org.openqa.selenium.By;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Jul 4, 2011
 *
 */
public class FeatureBlock extends DrupalBlock {

	private List<String> footNoteKeys = new ArrayList<String>();

	private List<String> footNotes = new ArrayList<String>();

	private List<DescriptionElementRepresentation> descriptionElements = new ArrayList<DescriptionElementRepresentation>();

	private String featureType = null;


	public List<String> getFootNoteKeys() {
		return footNoteKeys;
	}

	public List<String> getFootNotes() {
		return footNotes;
	}

	public List<DescriptionElementRepresentation> getDescriptionElements() {
		return descriptionElements;
	}

	public String getFeatureType() {
		return featureType;
	}


	/**
	 * @param element
	 */
	public FeatureBlock(RenderedWebElement element, String enclosingTag, String elementTag) {
		super(element);

		List<WebElement> fnkList = element.findElements(By.className("footnote-key"));
		for(WebElement fnk : fnkList) {
			footNoteKeys.add(fnk.getText());
		}

		List<WebElement> fnList = element.findElements(By.className("footnote"));
		for(WebElement fn : fnList) {
			footNotes.add(fn.getText());
		}

		RenderedWebElement descriptionElementsRepresentation = (RenderedWebElement) element.findElement(By.className("description"));
		featureType = descriptionElementsRepresentation.getAttribute("id");
		assertEquals("Unexpected tag enclosing description element representations", enclosingTag, descriptionElementsRepresentation.getTagName());

		for(WebElement el : descriptionElementsRepresentation.findElements(By.tagName(elementTag))){
			descriptionElements.add(new DescriptionElementRepresentation((RenderedWebElement)el));
		}

	}



}
