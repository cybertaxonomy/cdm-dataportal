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
import java.util.Arrays;
import java.util.List;

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Jul 1, 2011
 *
 */
public class BaseElement {


	/**
	 * Default tolerance for testing sizes and positions
	 */
	protected static final double PIXEL_TOLERANCE = 0.5;

	private WebElement element;

	private List<String> classAttributes = null;

	private List<LinkElement> linksInElement = null;

	private String text = null;


	public WebElement getElement() {
		return element;
	}

	public String getText() {
		return text;
	}

	public List<String> getClassAttributes() {
		return classAttributes;
	}

	void setClassAttributes(List<String> classAttributes) {
		this.classAttributes = classAttributes;
	}

	public double getComputedFontSize(){
		return pxSizeToDouble(getElement().getCssValue("font-size") );
	}

	public static double pxSizeToDouble(String pxValue){
		return Double.valueOf( pxValue.replaceAll("[a-zA-Z]", "") );
	}

	public List<LinkElement> getLinksInElement() {
		if(linksInElement == null){
			linksInElement = new ArrayList<LinkElement>();

			if(getElement().getTagName().equals("a") && getElement().getAttribute("href") == null){
				// BaseElement is link itself
				linksInElement.add(new LinkElement(getElement()));
			} else {
				// look for links in subelements
				List<WebElement> links = getElement().findElements(By.xpath("./a[@href]"));
				for (WebElement link : links) {
					linksInElement.add(new LinkElement(link));
				}

			}
		}
		return linksInElement;
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

	/**
	 * @return
	 */
	public String toSting() {
		return this.getClass().getSimpleName() + "<" + this.getElement().getTagName() + ">" ;
	}


}