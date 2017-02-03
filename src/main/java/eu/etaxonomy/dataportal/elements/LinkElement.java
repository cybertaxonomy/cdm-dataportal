/**
 * Copyright (C) 2011 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */

package eu.etaxonomy.dataportal.elements;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Jul 1, 2011
 *
 */
public class LinkElement extends BaseElement {

	private String href = null;

	public String getUrl() {
		return href;
	}

	public void setHref(String href) {
		this.href = href;
	}


	public LinkElement(WebElement a) {

		super(a);

		if(!a.getTagName().equals("a")){
			throw new RuntimeException("Invalid WebElement: <" + a.getTagName() + "> given, but must be <a>");
		}

		// read src url
		if (a.getAttribute("href") != null) {
				setHref(a.getAttribute("href"));
		}

	}

	@Override
	public String toString(){
		return super.toString() + ": " + (getUrl() != null ? getUrl() : "NO URL") + "";
	}

	public static boolean testIfLinkElement(BaseElement element, String text, String href) {
		return testIfLinkElement(element.getElement(), text, href);
	}

	/**
	 * @param element
	 * @param text
	 * @param hrefEndsWith
	 *   The actual link is must end with this. The full URL works of course also.
	 * @return
	 */
	public static boolean testIfLinkElement(WebElement element, String text, String hrefEndsWith) {
		assertEquals("a", element.getTagName());
		assertTrue(element.getAttribute("href").endsWith(hrefEndsWith));
		assertEquals(text, element.getText());
		return true;
	}

}
