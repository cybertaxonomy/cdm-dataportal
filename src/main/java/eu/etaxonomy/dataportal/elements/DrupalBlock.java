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

import org.apache.log4j.Level;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;


/**
 * @author andreas
 * @date Jul 4, 2011
 *
 */
public class DrupalBlock extends BaseElement {

	private String header;

	protected WebElement content;

	/**
	 * @param element
	 */
	public DrupalBlock(WebElement element) {

		super(element);

		logger.setLevel(Level.TRACE);
        logger.trace("DrupalBlock() - constructor after super()");

		content = element.findElement(By.className("content"));

		logger.trace("DrupalBlock() - block content loaded");
		try {
			WebElement headerElement = element.findElement(By.tagName("h2"));
			header = headerElement.getText();
			logger.trace("DrupalBlock() - header text ready");
		} catch (NoSuchElementException e){
			// IGNORE //
		}
	}

	public String getHeader() {
		return header;
	}

	public WebElement getContent() {
		return content;
	}

}
