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

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

public class DescriptionElementRepresentation extends BaseElement{

	List sources = new ArrayList(); // may be LinkElement or BaseElement

	public List getSources() {
		return sources;
	}

	/**
	 * @param element
	 */
	public DescriptionElementRepresentation(WebElement element) {
		super(element);

		for (WebElement source : element.findElements(By.className("sources"))) {
			if(source.getTagName().equals("a")){
				sources.add(new LinkElement(source));
			} else {
				sources.add(new BaseElement(source));
			}
		}
	}



}