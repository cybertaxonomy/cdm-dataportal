/**
* Copyright (C) 2011 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import org.apache.log4j.Level;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;


/**
 * @author andreas
 * @since Jul 4, 2011
 *
 */
public class DrupalBlock extends BaseElement {

	WebElement titleElement;


	public DrupalBlock(WebElement element) {
	    this(element, false);
	}

	public DrupalBlock(WebElement element, boolean hasHiddenTitle) {

		super(element);

		logger.setLevel(Level.TRACE);
        logger.trace("DrupalBlock() - constructor after super()");

		try {
		    titleElement = element.findElement(By.className("block-title"));
		} catch (NoSuchElementException e){
		    try {
		        titleElement = element.findElement(By.className("title"));
		    } catch  (NoSuchElementException e2){
		        if(!hasHiddenTitle){
		            throw e2;
		        }
		    }
		}
	}

	public String getHeaderText() {
		return titleElement.getText();
	}

	public String getContentText() {
	    String titleText = getHeaderText();
	    String elementText = getElement().getText();
	    if(elementText.startsWith(titleText)){
	        elementText = elementText.substring(titleText.length(), elementText.length());
	    }
		return elementText;
	}

}
