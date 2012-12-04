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

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Aug 29, 2011
 *
 */
public class TaxonListElement extends BaseElement {

	private TaxonType type;

	private String fullTaxonName;


	/**
	 * @param element
	 */
	public TaxonListElement(WebElement element) {
		super(element);
		type = TaxonType.valueOf( element.getAttribute("class") );
		fullTaxonName = element.findElement(By.tagName("span")).getText();
	}


	public TaxonType getType() {
		return type;
	}


	public String getFullTaxonName() {
		return fullTaxonName;
	}




}
