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

import java.util.Arrays;
import java.util.List;

import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Jul 1, 2011
 *
 */
public class BaseElement {


	private WebElement element;

	private List<String> classAttributes = null;

	private String text = null;


	public WebElement getElement() {
		return element;
	}

	public String getText() {
		return text;
	}

	public void setText(String text) {
		this.text = text;
	}

	public List<String> getClassAttributes() {
		return classAttributes;
	}

	public void setClassAttributes(List<String> classAttributes) {
		this.classAttributes = classAttributes;
	}

	/**
	 * @param element
	 */
	public BaseElement(WebElement element) {

		this.element = element;

		// read text
		text = element.getText();

		// read and tokenize the class attribute
		if (element.getAttribute("class") != null) {
			String[] classTokens = element.getAttribute("class").split("\\s");
			setClassAttributes(Arrays.asList(classTokens));
		}
	}


}